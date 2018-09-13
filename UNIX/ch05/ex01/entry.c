/* This program can be used to illustrates how the kernel sets up the
 * startup routine.  Call with "-e" to specify any of the functions as the
 * entry point instead of 'main'; compare otool(1)/objdump(1) output and
 * exit codes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int foo(void) {
	printf("Foo for the win!\n");
	return 1;
	/* Note: this will cause a segfault, because this function
	 * returns, but there is nothing to return to: the routines set up
	 * by the kernel remain set up for 'main', we just told the linker
	 * to jump into 'foo' at program start.  Compare objdump(1)
	 * output. */
}

int bar(void) {
	printf("bar rules!\n");
	exit(1);
	/* Unlike foo(), this will not cause a segfault, since we are not
	 * returning; we explicitly call exit(3). */
}

int main(int argc, char **argv) {
	printf("Hooray main!\n");
	/* Note that we do explicitly _not_ return an error here, nor call
	 * any of the exit(3) functions.  Your compiler will warn you
	 * about this. */
}
