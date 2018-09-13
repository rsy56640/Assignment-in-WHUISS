#include <stdio.h>

void examine1(){
    int x=1;
    int y=7;
    int *p;
    p=&x;
    printf("%d",*p);
    p++;
    printf("  %d\n",*p);
    p=&y;
    printf("%d",*p);
    p++;
    //printf("  %p  ",p);
    printf("  %d\n",*p);
    printf("\n");
}

void examine2(){
    int A[2]={1,2};
    int *p=A;
    printf("%p--%d\n",p,*p);
    p++;
    printf("%p--%d\n",p,*p);
}

int main(){
    int i=5;
    int j=6;
    int *p;
    p=&i;
    printf("%p\n",p);
    p=&j;
    printf("%p\n",p);
    p++;
    printf("%p\n\n",p);
    examine1();
    examine2();
}
