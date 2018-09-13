#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//file is updated even if close(fd) is not invoked
int main(){
    int fd = open("./test.txt",O_WRONLY|O_CREAT,S_IRUSR | S_IWUSR);
    if(fd<0){
        printf("file open error!\n");
        return 1;
    }
    char *s ="hello,world\n";
    write(fd,s,strlen(s));
    char c;
    printf("press any key to continue......");
    scanf("%c",&c);
    close(fd);
    return 0;
}
