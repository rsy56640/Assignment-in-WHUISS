#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>

int main(){
    char name[30];
    printf("login:");
    scanf("%s",name);
    char* passwd=getpass("password:");
    printf("%s\n",passwd);
    struct passwd *pwd;
    pwd=getpwnam(name);
    printf("%s\n",pwd->pw_passwd);
    if(strcmp(passwd,pwd->pw_passwd)==0)
        printf("succeed\n");
    else
        printf("fail\n");
}
