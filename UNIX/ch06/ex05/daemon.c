#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
    pid_t pid;
    int i,fd;
    char *buf="This is a daemon program.\n";

    if ((pid=fork())<0) {
        printf("fork error!");
        exit(1);
    }else if (pid>0)  // fork且退出父进程
        exit(0);
    
    setsid();    // 在子进程中创建新会话。
    chdir("/");  // 设置工作目录为根
    umask(0);    // 设置权限掩码
    for(i=0;i<getdtablesize();i++)  //getdtablesize返回子进程文件描述符表的项数
        close(i);                // 关闭这些不将用到的文件描述符

    while(1) {// 死循环表征它将一直运行
// 以读写方式打开"/tmp/daemon.log"，返回的文件描述符赋给fd
        if ((fd=open("/tmp/daemon.log",O_CREAT|O_WRONLY|O_APPEND,0600))<0) {
            printf("Open file error!\n");
            exit(1);
        }
        // 将buf写到fd中
        write(fd,buf,strlen(buf)+1);
        close(fd);
        sleep(10);
        printf("Never output!\n");
    }

    return 0;
}
