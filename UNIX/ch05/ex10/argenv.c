#include <stdio.h>

int main(int arc, char* argv[], char* env[]){
    int i=0;
    while(env[i])
        printf("%s\n",env[i++]);
}
