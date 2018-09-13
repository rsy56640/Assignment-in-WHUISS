#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <unistd.h>
int main(void)
{   int fd;
    if((fd=open("myoutput",O_WRONLY|O_CREAT,0644))==-1)
    {
         perror("open myoutput failed.\n");
         exit(1);

    }

    /*重复标准输出至描述字fd相连的文件myoutput*/
    if(dup2(fd,STDOUT_FILENO)==-1)
         perror("Redirect standar output failed");
    printf("this is a test program for redirect \n");
    close(fd);
    return 0;
}
