#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
     pid_t pid;
 
     if ((pid=fork())<0) {
         printf("fork error!");
     }else if (pid==0) {
         printf("The child process PID is %d.\n",getpid());
         printf("The Group ID is %d.\n",getpgrp());
         printf("The Group ID is %d.\n",getpgid(0));
         printf("The Group ID is %d.\n",getpgid(getpid()));
         exit(0);
     }
 
     sleep(3);
     printf("The parent process PID is %d.\n",getpid());
     printf("The Group ID is %d.\n",getpgrp());
 
     return 0;
 }


