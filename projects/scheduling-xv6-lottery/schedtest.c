#include "types.h"
#include "user.h"
#include "pstat.h"

#define DURATION 1000
#define UPDATE_PERIOD 10

void quit(char* str) {
  printf(2, str);
  exit();
}

int fork_safe() {
  int pid = fork();
  if (pid < 0)
    quit("fork error\n");
  return pid;
}

void settickets_safe(int tickets) {
  if (settickets(tickets) < 0)
    quit("settickets error\n");
}

int spin(int tickets) {
  int pid = fork_safe();
  if (pid > 0)
    return pid;
  settickets_safe(tickets);
  int start = uptime();
  int x = 0;
  while (uptime() - start < DURATION)    
    x++;
  exit();
  return x;
}

int main(void) {
  settickets_safe(30);
  int pid1 = spin(10);
  int pid2 = spin(20);
  int pid3 = spin(30);
  struct pstat ps;
  int start = uptime();
  for (;;) {
    int elapsed = uptime() - start;
    if (elapsed >= DURATION)
      break;
    if (getpinfo(&ps) < 0)
      quit("getpinfo error\n");
    printf(1, "%d\t", elapsed);
    for (uint i = 0; i < NPROC; i++) {
      if (ps.pid[i] == pid1 || ps.pid[i] == pid2 || ps.pid[i] == pid3)
        printf(1, "%d\t", ps.ticks[i]);
    }
    printf(1, "\n");
    sleep(UPDATE_PERIOD);
  }
  wait();
  wait();
  wait();
  exit();
}
