#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
 int main() {
     pid_t pid;
 
     if ((pid=fork())<0) {
         printf("fork error!");
         exit(1);
     }else if (pid==0) {
         printf("The child process PID is %d.\n",getpid());
         printf("The Group ID of child is %d.\n",getpgid(0)); // 返回组id
         sleep(5);
         printf("The Group ID of child is changed to %d.\n",getpgid(0));
         exit(0);
     }
 
     sleep(1);
     setpgid(pid,pid); // 改变子进程的组id为子进程本身
     
     sleep(5);
     printf("The parent process PID is %d.\n",getpid());
     printf("The parent of parent process PID is %d.\n",getppid());
     printf("The Group ID of parent is %d.\n",getpgid(0));
     setpgid(getpid(),getppid()); // 改变父进程的组id为父进程的父进程
     printf("The Group ID of parent is changed to %d.\n",getpgid(0));
 
     return 0;
 }
