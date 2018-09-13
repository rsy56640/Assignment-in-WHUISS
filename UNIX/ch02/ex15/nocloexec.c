#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
        int fd,pid;
        char buffer[20];
        fd=open("wo.txt",O_RDONLY);
        printf("%d\n",fd);
        int val=fcntl(fd,F_GETFD);
        val &=~FD_CLOEXEC;
        fcntl(fd,F_SETFD,val);

        pid=fork();
        if(pid==0){
                //子进程中，此描述符并不关闭，仍可使用
                char child_buf[2];
                memset(child_buf,0,sizeof(child_buf) );
                ssize_t bytes = read(fd,child_buf,sizeof(child_buf)-1 );
                printf("child, bytes:%d,%s\n\n",bytes,child_buf);

                //execl执行的程序里，此描述符并没有被关闭，仍然可以使用它
                char fd_str[5];
                memset(fd_str,0,sizeof(fd_str));
                sprintf(fd_str,"%d",fd);
                int ret = execl("./exe1","exe1",fd_str,NULL);
                if(-1 == ret)
                        perror("ececl fail:");
        }        

        waitpid(pid,NULL,0);
        memset(buffer,0,sizeof(buffer) );
        ssize_t bytes = read(fd,buffer,sizeof(buffer)-1 );
        printf("parent, bytes:%d,%s\n\n",bytes,buffer);
}


