
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
  pid_t Childpid = fork();
     
  if(Childpid == 0) {
    //Running in Child process

    //exec() to replace the child process executable
    char execFileName[] = "./tmp/child";

    execl(execFileName, execFileName, (char *) NULL);
  } else {
    //Running in parent process

    //Wait for the child process to terminate
    waitpid(Childpid,NULL,0);

    printf("\n*** Child process finished ***\n");
  }  
  return EXIT_SUCCESS;
}
