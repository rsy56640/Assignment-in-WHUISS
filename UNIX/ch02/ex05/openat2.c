#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <dirent.h>  
#include <stdio.h>  
#include <unistd.h>  
  
int main()  
{  
    DIR *dir;  
    int dirfd2;  
    int fd;  
    int n;  
  
    dir = opendir("./open");  
    if(NULL == dir)  
    {  
        perror("open dir error");  
        return -1;  
    }  
    dirfd2 = dirfd(dir);  
    if(-1 == dirfd2)  
    {  
        perror("dirfd error");  
        return -1;  
    }  
  
    fd = openat(dirfd2,"output.log",O_CREAT|O_RDWR|O_TRUNC);  
    if(-1 == fd)  
    {  
        perror("opeat error");  
        return -1;  
    }  
    n = write(fd,"Hello world!\n",15);  
      
    close(fd);  
    closedir(dir);  
  
    return 0;  
  
} 
