#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define stat xv6_stat      // avoid clash with host struct stat
#define dirent xv6_dirent  // avoid clash with host struct dirent
#include "types.h"
#include "fs.h"
#include "stat.h"
#undef stat
#undef dirent

typedef uchar dblock[BSIZE];

static dblock* blocks = 0;
static uint data_start = 0;
static uint data_end = 0;
static struct dinode* inodes = 0;
static uchar* bitmap = 0;
static ushort* block_to_inode = 0;
static short* inode_to_nlink = 0;

static void check_data_block(ushort inum, uint block, bool is_direct) {
  if (!block) {
    return;
  }
  char* type = is_direct ? "direct" : "indirect";
  if (block < data_start || block > data_end) {
    fprintf(stderr, "ERROR: bad %s address in inode\n", type);
    exit(1);
  }
  if (!bitmap[block]) {
    fprintf(stderr, "ERROR: address used by inode but marked free in bitmap\n");
    exit(1);
  }
  if (block_to_inode[block]) {
    fprintf(stderr, "ERROR: %s address used more than once\n", type);
    exit(1);
  }
  block_to_inode[block] = inum;
  if (inodes[inum].type == T_DIR) {
    struct xv6_dirent* dir = (struct xv6_dirent*) blocks[block];
    for (int j = 0; j < BSIZE/sizeof(dir[0]); ++j) {
      struct xv6_dirent* entry = &dir[j];
      if (!entry->inum) {
        continue;
      }
      if (!inodes[entry->inum].type) {
        fprintf(stderr, "ERROR: inode referred to in directory but marked free\n");
        exit(1);
      }
      if (strcmp(entry->name, ".") != 0 && strcmp(entry->name, "..") != 0) {
        inode_to_nlink[entry->inum] += 1;
        if (inodes[entry->inum].type == T_DIR && inode_to_nlink[entry->inum] > 1) {
          fprintf(stderr, "ERROR: directory appears more than once in file system\n");
          exit(1);
        }
      }
    }
  }
}

int main(int argc, char* argv[argc+1]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: xcheck <file_system_image>\n");
    return 1;
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    fprintf(stderr, "image not found\n");
    return 1;
  }

  struct stat st;
  if (fstat(fd, &st) != 0) {
    perror("fstat");
    return 1;
  }

  uchar* fs = mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (fs == MAP_FAILED) {
    perror("mmap");
    return 1;
  }

  blocks = (dblock*) fs;

  struct superblock* sb = (struct superblock*) blocks[1];

  data_start = BBLOCK(sb->size, sb->ninodes) + 1;
  data_end = sb->size - 1;

  inodes = (struct dinode*) (fs + IBLOCK(0)*BSIZE);

  bitmap = malloc(sb->size * sizeof(uchar));
  for (uint b = 0; b < sb->size; ++b) {
    uchar* bp = fs + BBLOCK(b, sb->ninodes)*BSIZE;
    int bi = b % BPB;
    int m = 1 << (bi % 8);
    bitmap[b] = !!(bp[bi/8] & m);
  }

  block_to_inode = calloc(sb->nblocks, sizeof(ushort));
  inode_to_nlink = calloc(sb->ninodes, sizeof(short));

  for (ushort inum = 0; inum < sb->ninodes; ++inum) {
    struct dinode* ip = &inodes[inum];
    if (ip->type != 0 && ip->type != T_DIR && ip->type != T_FILE && ip->type != T_DEV) {
      fprintf(stderr, "ERROR: bad inode\n");
      return 1;
    }
    if (ip->type != 0) {
      for (int i = 0; i < NDIRECT; ++i) {
        check_data_block(inum, ip->addrs[i], true);
      }
      if (ip->addrs[NDIRECT]) {
        if (ip->addrs[NDIRECT] < data_start || ip->addrs[NDIRECT] > data_end) {
          fprintf(stderr, "ERROR: bad indirect address in inode\n");
          return 1;
        }

        if (block_to_inode[ip->addrs[NDIRECT]]) {
          fprintf(stderr, "ERROR: indirect address used more than once\n");
          return 1;
        }
        block_to_inode[ip->addrs[NDIRECT]] = inum;

        uint* indirect_addrs = (uint*) blocks[ip->addrs[NDIRECT]];
        for (uint i = 0; i < NINDIRECT; ++i) {
          check_data_block(inum, indirect_addrs[i], false);
        }
      }
    }
    if (ip->type == T_DIR) {
      if (!ip->addrs[0]) {
        fprintf(stderr, "ERROR: directory not properly formatted\n");
        return 1;
      }
      struct xv6_dirent* dir = (struct xv6_dirent*) blocks[ip->addrs[0]];
      if (dir->inum != inum || strcmp(dir->name, ".") != 0) {
        fprintf(stderr, "ERROR: directory not properly formatted\n");
        return 1;
      }
      if (!(dir+1)->inum || strcmp((dir+1)->name, "..") != 0) {
        fprintf(stderr, "ERROR: directory not properly formatted\n");
        return 1;
      }
    }
  }

  struct dinode* root_inode = &inodes[ROOTINO];
  if (root_inode->type != T_DIR || !root_inode->addrs[0]) {
    fprintf(stderr, "ERROR: root directory does not exist\n");
    return 1;
  }
  struct xv6_dirent* root_dir = (struct xv6_dirent*) blocks[root_inode->addrs[0]];
  if (root_dir->inum != ROOTINO || strcmp(root_dir->name, ".") != 0) {
    fprintf(stderr, "ERROR: root directory does not exist\n");
    return 1;
  }
  if ((root_dir+1)->inum != ROOTINO || strcmp((root_dir+1)->name, "..") != 0) {
    fprintf(stderr, "ERROR: root directory does not exist\n");
    return 1;
  }

  for (uint b = data_start; b < sb->size; ++b) {
    if (bitmap[b] && !block_to_inode[b]) {
      fprintf(stderr, "ERROR: bitmap marks block in use but it is not in use\n");
      return 1;
    }
  }

  for (ushort inum = 0; inum < sb->ninodes; ++inum) {
    if (inum != ROOTINO && inodes[inum].type && !inode_to_nlink[inum]) {
      fprintf(stderr, "ERROR: inode marked use but not found in a directory\n");
      return 1;
    }
    if (inodes[inum].type == T_FILE && inodes[inum].nlink != inode_to_nlink[inum]) {
      fprintf(stderr, "ERROR: bad reference count for file\n");
      return 1;
    }
  }

  return 0;
}
