#include <stdio.h>
#include <stdlib.h>

int main(){
    const int bsize=1024;
    int i=0,*p=NULL;
    p = (int*)alloca(bsize);
    while(p){
        printf("block %d\n",++i);
        p=(int*)alloca(bsize);
    }
    exit(0);
}
