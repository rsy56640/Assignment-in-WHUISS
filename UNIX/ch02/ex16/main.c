//以下程序的函数来设置或清除标志FD_CLOEXEC而不改变其他任何标志。
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int set_cloexec_flag(int filedes,int value)
{
  int oldflags=fcntl(filedes,F_GETFD,0);
/*若读文件标签失败，错误返回*/
  if(oldflags<0)
       return oldflags;
   /*设置我们需要的标志.*/
   if(value!=0)
       oldflags|=FD_CLOEXEC;
   else
       oldflags&=~FD_CLOEXEC;
    /*储存修改后的标志至描述字*/
   return fcntl(filedes,F_SETFD,oldflags);
}

int main(){
    int fd = open("abc.txt",O_WRONLY|O_CREAT,0644);
    if(fd<0){
        printf("open file failed\n");
        exit(1);
    }

    int v=set_cloexec_flag(fd,1);
    printf("%d\n",v);
    return 0;
}
