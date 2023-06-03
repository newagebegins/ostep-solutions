#include "types.h"
#include "user.h"

int fork_safe() {
  int pid = fork();
  if (pid < 0) {
    printf(1, "fork error\n");
    exit();
  }
  return pid;
}

void mprotect_safe(void *addr, int len) {
  if (mprotect(addr, len) < 0) {
    printf(1, "mprotect error\n");
    exit();
  }
}

void munprotect_safe(void *addr, int len) {
  if (munprotect(addr, len) < 0) {
    printf(1, "munprotect error\n");
    exit();
  }
}

int main(int argc, char* argv[]) {
  uchar* m = malloc(0x1000 * 10);
  printf(1, "m = %x\n", m);

  char* p1 = (char*) ((uint)m & ~0xFFF);
  char* p2 = (char*) ((uint)p1 + 0x1000);

  if (fork_safe() == 0) {
    printf(1, "(pid %d) Test 1\n", getpid());
    *p1 = 0x12;
    printf(1, "1 OK! *p1 = 0x%x\n", (uint) *p1);
    *p2 = 0x34;
    printf(1, "2 OK! *p2 = 0x%x\n", (uint) *p2);
    mprotect_safe(p1, 1);
    *p2 = 0x56;
    printf(1, "3 OK! *p2 = 0x%x\n", (uint) *p2);
    printf(1, "expected fail:\n");
    *p1 = 0x78;
    printf(1, "ERROR! *p1 = 0x%x\n", (uint) *p1);
    exit();
  }
  wait();

  if (fork_safe() == 0) {
    printf(1, "(pid %d) Test 2\n", getpid());
    *p1 = 0x12;
    *p2 = 0x34;
    printf(1, "OK! *p1 = 0x%x, *p2 = 0x%x\n", (uint) *p1, (uint) *p2);
    mprotect_safe(p1, 2);
    printf(1, "expected fail:\n");
    *p2 = 0x56;
    printf(1, "ERROR! *p1 = 0x%x, *p2 = 0x%x\n", (uint) *p1, (uint) *p2);
    exit();
  }
  wait();

  if (fork_safe() == 0) {
    printf(1, "(pid %d) Test 3\n", getpid());
    *p1 = 0x12;
    *p2 = 0x34;
    printf(1, "1 OK! *p1 = 0x%x, *p2 = 0x%x\n", (uint) *p1, (uint) *p2);
    mprotect_safe(p1, 2);
    munprotect_safe(p1, 2);
    *p1 = 0x88;
    printf(1, "2 OK! *p1 = 0x%x\n", (uint) *p1);
    *p2 = 0x99;
    printf(1, "3 OK! *p2 = 0x%x\n", (uint) *p2);
    exit();
  }
  wait();

  // Test that forked processes inherit page permissions
  mprotect_safe(p1, 1);
  if (fork_safe() == 0) {
    printf(1, "(pid %d) Test 4\n", getpid());
    *p2 = 0x12;
    printf(1, "1 OK! *p2 = 0x%x\n", (uint) *p2);
    printf(1, "expected fail:\n");
    *p1 = 0x34;
    printf(1, "ERROR! *p1 = 0x%x\n", (uint) *p1);
    exit();
  } 
  wait();

  exit();
}
