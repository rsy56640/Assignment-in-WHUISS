#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// if fclose(fp) is not invoked, the file will not be updated
int main(){
    FILE *fp;
    fp = fopen("./test.txt","w");
    if(!fp){
        printf("file open error!\n");
        return 1;
    }
    char *s ="hello,world\n";
    fprintf(fp,"%s",s);
    char c;
    printf("press any key to continue......");
    scanf("%c",&c);
    fclose(fp);
    return 0;
}
