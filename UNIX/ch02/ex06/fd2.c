#include <stdio.h>
#include <fcntl.h>

int main(){
    int fd = open("./fd2.c",O_RDONLY);
    printf("%d\n",fd);
    printf("press return to continue");
    char c;
    scanf("%c",&c);
    close(fd);
    return 0;
}
