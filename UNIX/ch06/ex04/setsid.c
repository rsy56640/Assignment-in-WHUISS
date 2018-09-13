#include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 
 int main() {
     pid_t pid;
 
     if ((pid=fork())<0) {
         printf("fork error!");
         exit(1);
     }else if (pid==0) {
         sleep(0);
         printf("The child process PID is %d.\n",getpid());
         printf("The Group ID of child is %d.\n",getpgid(0));
         printf("The Session ID of child is %d.\n",getsid(0));
         sleep(5);
         setsid(); // 子进程非组长进程，故其成为新会话首进程，且成为组长进程。该进程组id即为会话进程
         printf("Changed:\n");
         printf("The child process PID is %d.\n",getpid());
         printf("The Group ID of child is %d.\n",getpgid(0));
         printf("The Session ID of child is %d.\n",getsid(0));
         sleep(10);
         exit(1);
     }
     printf("The Session ID of parent is %d.\n",getsid(0)); 
     setsid(); // 父进程是组长进程，故让其成为新会话首进程时会出错
     printf("The Session ID of parent is %d.\n\n",getsid(0));
     sleep(2);
     return 2;
 }
