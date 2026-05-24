#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int tickets[] = {10, 20, 30};
  int pids[3];

  printf("Lottery Scheduler Test - Proportional CPU Allocation\n");
  printf("Creating 3 processes with tickets: 10, 20, 30\n");
  printf("Expected CPU ratio: ~1:2:3\n\n");

  for (int i = 0; i < 3; i++) {
    // Temporarily raise parent tickets so the child inherits a high priority.
    // This guarantees the child runs almost immediately to set its own tickets.
    settickets(100);
    int pid = fork();
    if (pid == 0) {
      // Child process
      settickets(tickets[i]);

      // Infinite loop to consume CPU
      while (1) {
        // Busy loop
      }
    }
    pids[i] = pid;
  }

  // Restore parent tickets to default
  settickets(1);

  // Parent: sleep for 50 ticks to let children compete
  pause(50);

  struct pstat ps;
  if (getpinfo(&ps) < 0) {
    printf("getpinfo failed!\n");
    // Kill children before exit
    for (int i = 0; i < 3; i++) {
      kill(pids[i]);
    }
    exit(1);
  }

  printf("PID\tTickets\tTicks (times scheduled)\n");
  printf("---\t-------\t-----------------------\n");
  for (int i = 0; i < 64; i++) {  // NPROC = 64
    if (ps.inuse[i]) {
      for (int j = 0; j < 3; j++) {
        if (ps.pid[i] == pids[j]) {
          printf("%d\t%d\t%d\n", ps.pid[i], ps.tickets[i], ps.ticks[i]);
        }
      }
    }
  }

  // Clean up: kill all child processes
  for (int i = 0; i < 3; i++) {
    kill(pids[i]);
    wait(0);
  }

  printf("\nTest complete.\n");
  exit(0);
}
