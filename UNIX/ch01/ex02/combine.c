#include <stdio.h>
int main(){
    int i=8,j;
    __asm__ ("movl %1,%0" : "=r" (j) : "m" (i));
    printf("%d\n",j);
}
