#include <stdio.h>
#include <stdlib.h>

void foo2();
int gd=6;
int gu;

void foo1(){
    int jj=5;
    printf("the address of jj in foo1  is:%p\n",&jj);
    foo2();
}

void foo2(){
    int kk=5;
    printf("the address of kk in foo2  is:%p\n",&kk);
}

void foo3(){
    int *p;
    p = (int*)malloc(sizeof(int));
    printf("the address of *p in foo3  is:%p\n",p);
    int *q;
    q = (int*)malloc(sizeof(int));
    printf("the address of *q in foo3  is:%p\n",q);
    free(p);
    free(q);
}

int main(int argc,char **argv){
    printf("argv is:%p\n",argv);
    printf("the 1st cl argument argv[0] is:%p\n",argv[0]);
    printf("the 2nd cl argument argv[1] is:%p\n",argv[1]);
    printf("address of argc is:%p\n",&argc);
    int ii=0;
    printf("the address of ii in main  is:%p\n",&ii);
    foo1();
    foo3();
    printf("addr of uninitialized gvar is:%p\n",&gu);
    printf("addr of initialized g-var  is:%p\n",&gd);
    printf("address of function foo1   is:%p\n",&foo1);
}
