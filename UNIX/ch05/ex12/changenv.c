#include <stdio.h>
#include <stdlib.h>

int main(int arc, char* argv[]){
    printf("the environment var PATH is:%s\n",getenv("PATH"));
    char * home = getenv("HOME");
    printf("the environment var HOME is:'%s', and the address is %p\n",home,home);
    char * newhome="HOME=/root";
    putenv(newhome);
    home = getenv("HOME");
    printf("the new environment var HOME is:'%s' and the address is %p\n",home,home);
    printf("the pointer newhome is %p\n",newhome);
    char * newhome1="HOME=/tmp";
    setenv("HOME",newhome1,1);
    home = getenv("HOME");
    printf("the new environment var HOME is:'%s' and the address is %p\n",home,home);
    printf("the pointer newhome1 is %p\n",newhome1);
}
