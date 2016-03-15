#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <string.h>

static pid_t child_pid = 0;
static int sighup_fired = 0;

void sighandler(int signum, siginfo_t *info, void *ptr) {
  printf("Got sighup \n");
  printf("Signal originates from process %lu\n", (unsigned long)info->si_pid);
  fflush(stdout);
  exit(0);
}

void sigchld_handler(int signum, siginfo_t *info, void *ptr) {
  printf("Received sigchld signal %d\n", signum);
  printf("Signal originates from process %lu\n",
      (unsigned long)info->si_pid);
  printf("Handling death of child...");
  pid_t p;
  int status;
  while ((p=waitpid(-1, &status, WNOHANG)) != -1) {
    printf("child process %d died\n", p);
  }
  printf("exiting now...\n");
  fflush(stdout);
  exit(0);
}

void sigkill_handler(int signum, siginfo_t *info, void *ptr) {
  printf("Received sigkill signal %d\n", signum);
  printf("Sending sighull to child");
  kill(child_pid, SIGHUP);
  fflush(stdout);
  exit(0);
}

int main(void) {
  struct sigaction sa;
  int pid;
  int pipefd[2];
  
  memset(&sa, 0, sizeof(sa));
  
  //~ pipe (pipefd);
  
  pid = fork();
  if(pid == 0) {

    printf("child(pid:%d)...sleeping forever ", getpid()); 
    printf("w/ PR_SET_PDEATHSIG v5\n");
    fflush(stdout);
    
    if(prctl(PR_SET_PDEATHSIG, SIGHUP) == -1) {
      perror("prctl failed"); // Should not happen
    }
    
    //~ close(1);
    //~ dup (pipefd[1]);
    //~ close(pipefd[0]);
    
    sa.sa_sigaction = &sighandler;
    sa.sa_flags = SA_SIGINFO;
    sigfillset(&sa.sa_mask);
    if (sigaction(SIGHUP, &sa, NULL) == -1) {
      perror("Error: cannot handle SIGHUP"); // Should not happen
    }
    
    while(!sighup_fired) {
      sleep(10);
    }
    printf("pid (%d) got sighup%s\n", getpid());
    printf("exiting\n");
    fflush(stdout);
    exit(0);
  } else if(pid > 0) {
    child_pid = pid;
    printf("parent (pid:%d)...sleeping forever\n", getpid());
    fflush(stdout);

    sa.sa_sigaction = &sigchld_handler;
    sa.sa_flags = SA_SIGINFO;
    sigfillset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
      perror("Error: cannot handle SIGCHLD"); // Should not happen
    }
    
    while(1) {
      sleep(10);
    }
  } else {
    printf("\n Fork failed, quitting!!!!!!\n");
    fflush(stdout);
    return 1;
  }
  
  
  return 0;
}
