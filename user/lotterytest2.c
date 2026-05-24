#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("Lottery Fairness Test\n");
  printf("Process A: 10 tickets, Process B: 30 tickets\n");
  printf("Expected ratio B/A: ~3.0\n\n");

  int pid_a, pid_b;

  // Temporarily raise parent tickets so Child A inherits it
  settickets(100);
  pid_a = fork();
  if (pid_a == 0) {
    settickets(10);
    while (1) {
      // Infinite busy loop
    }
  }

  // Temporarily raise parent tickets so Child B inherits it
  settickets(100);
  pid_b = fork();
  if (pid_b == 0) {
    settickets(30);
    while (1) {
      // Infinite busy loop
    }
  }

  // Restore parent tickets to default
  settickets(1);

  // Parent sleeps for 50 ticks to let children compete
  pause(50);

  struct pstat ps;
  if (getpinfo(&ps) < 0) {
    printf("getpinfo failed!\n");
    kill(pid_a);
    kill(pid_b);
    exit(1);
  }

  int ticks_a = 0, ticks_b = 0;
  for (int i = 0; i < 64; i++) {
    if (ps.inuse[i] && ps.pid[i] == pid_a) ticks_a = ps.ticks[i];
    if (ps.inuse[i] && ps.pid[i] == pid_b) ticks_b = ps.ticks[i];
  }

  printf("Process A (10 tickets): scheduled %d times\n", ticks_a);
  printf("Process B (30 tickets): scheduled %d times\n", ticks_b);

  if (ticks_a > 0) {
    // Integer approximation of ratio * 100
    int ratio_x100 = (ticks_b * 100) / ticks_a;
    int whole = ratio_x100 / 100;
    int frac = ratio_x100 % 100;
    if (frac < 10) {
      printf("Ratio B/A: %d.0%d (expected ~3.0)\n", whole, frac);
    } else {
      printf("Ratio B/A: %d.%d (expected ~3.0)\n", whole, frac);
    }
  }

  // Clean up
  kill(pid_a);
  kill(pid_b);
  wait(0);
  wait(0);

  printf("\nTest complete.\n");
  exit(0);
}
