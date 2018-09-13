

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct opaque_ds  ods;
ods * opaque_func();
void display_ods(ods * ods);

int main(int argc, char **argv) {

    ods *dp;
    dp = opaque_func();
    display_ods(dp);
	return(0);
}
