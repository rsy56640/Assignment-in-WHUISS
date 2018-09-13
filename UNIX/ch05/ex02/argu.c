/* This program can be used to illustrates how the kernel sets up the
 * startup routine.  Call with "-e" to specify any of the functions as the
 * entry point instead of 'main'; compare otool(1)/objdump(1) output and
 * exit codes.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct big{
    char msp[120];
    int y;
};

int foo(int x1,int x2, int x3, int x4, int x5, int x6, int x7, struct big x8){
    int a= x1+x2+x3+x4;
    int b= x5+x6+x7*x8.y;
    return a*b;
}
int main(int argc,char *argv[]) {
    struct  big z;
    z.y=8;
    printf("foo value is:%d\n",foo(1,2,3,4,5,6,7,z));
	printf("%p\n",argv[argc]);
	/* Note that we do explicitly _not_ return an error here, nor call
	 * any of the exit(3) functions.  Your compiler will warn you
	 * about this. */
}
