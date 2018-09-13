#include <stdio.h>

extern char **environ;
int main(int arc, char* argv[],char* env[]){
    printf("%p,%p\n",environ,env);
    int i=0;
    while(environ[i])
        printf("%s\n",environ[i++]);
    printf("----------------------------\n");
    printf("%d\n",env[0]==environ[0]);
    printf("%d\n",env[1]==environ[1]);
    printf("----------------------------\n");
    printf("%d\n",&env==&environ);
}
