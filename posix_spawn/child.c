
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <signal.h>
#include <string.h>

void sighandler(int signum, siginfo_t *info, void *ptr) {
  printf("Received signal %d\n", signum);
  printf("Signal originates from process %lu\n", (unsigned long)info->si_pid);
}

int main(void) {
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.sa_sigaction = sighandler;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGHUP, &act, NULL);
  
  prctl(PR_SET_PDEATHSIG, SIGHUP);
  printf("\n*** Child Running ***\n");
  fflush(stdout);
  fclose(stdin);
  sleep(100);
  return EXIT_SUCCESS;
}
