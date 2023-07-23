#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define stat xv6_stat      // avoid clash with host struct stat
#define dirent xv6_dirent  // avoid clash with host struct dirent
#include "types.h"
#include "fs.h"
#include "stat.h"
#undef stat
#undef dirent

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

  struct superblock* sb = (struct superblock*) (fs + BSIZE);

  uint num_inode_blocks = (sb->ninodes + IPB - 1)/IPB; // round up
  uint num_bitmap_blocks = (sb->size + BPB - 1)/BPB; // round up
  uint data_start = 2 + num_inode_blocks + num_bitmap_blocks;
  uint data_end = sb->size - 1;

  struct dinode* inodes = (struct dinode*) (fs + IBLOCK(0)*BSIZE);

  uchar* bitmap = malloc(sb->size * sizeof(uchar));
  for (uint b = 0; b < sb->size; ++b) {
    uchar* bp = fs + BBLOCK(b, sb->ninodes)*BSIZE;
    int bi = b % BPB;
    int m = 1 << (bi % 8);
    bitmap[b] = !!(bp[bi/8] & m);
  }

  for (ushort inum = 0; inum < sb->ninodes; ++inum) {
    struct dinode* ip = &inodes[inum];
    if (ip->type != 0 && ip->type != T_DIR && ip->type != T_FILE && ip->type != T_DEV) {
      fprintf(stderr, "ERROR: bad inode\n");
      return 1;
    }
    if (ip->type != 0) {
      for (int i = 0; i < NDIRECT; ++i) {
        uint block = ip->addrs[i];
        if (block) {
          if (block < data_start || block > data_end) {
            fprintf(stderr, "ERROR: bad direct address in inode\n");
            return 1;
          }
          if (!bitmap[block]) {
            fprintf(stderr, "ERROR: address used by inode but marked free in bitmap\n");
            return 1;
          }
        }
      }
      if (ip->addrs[NDIRECT]) {
        if (ip->addrs[NDIRECT] < data_start || ip->addrs[NDIRECT] > data_end) {
          fprintf(stderr, "ERROR: bad indirect address in inode\n");
          return 1;
        }
        uint* indirect_addrs = (uint*) (fs + ip->addrs[NDIRECT]*BSIZE);
        for (uint i = 0; i < BSIZE/sizeof(uint); ++i) {
          uint block = indirect_addrs[i];
          if (block) {
            if (block < data_start || block > data_end) {
              fprintf(stderr, "ERROR: bad indirect address in inode\n");
              return 1;
            }
            if (!bitmap[block]) {
              fprintf(stderr, "ERROR: address used by inode but marked free in bitmap\n");
              return 1;
            }
          }
        }
      }
    }
    if (ip->type == T_DIR) {
      if (!ip->addrs[0]) {
        fprintf(stderr, "ERROR: directory not properly formatted\n");
        return 1;
      }
      struct xv6_dirent* dir = (struct xv6_dirent*) (fs + ip->addrs[0]*BSIZE);
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
  struct xv6_dirent* root_dir = (struct xv6_dirent*) (fs + root_inode->addrs[0]*BSIZE);
  if (root_dir->inum != ROOTINO || strcmp(root_dir->name, ".") != 0) {
    fprintf(stderr, "ERROR: root directory does not exist\n");
    return 1;
  }
  if ((root_dir+1)->inum != ROOTINO || strcmp((root_dir+1)->name, "..") != 0) {
    fprintf(stderr, "ERROR: root directory does not exist\n");
    return 1;
  }

  return 0;
}
