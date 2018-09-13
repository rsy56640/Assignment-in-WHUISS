/* open.c
 * open a file with O_SYNC for writing
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFSIZE 512

int
main(void)
{
	int n;
	char buf[BUFFSIZE];

	int fd = open("./out",O_WRONLY|O_SYNC);

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0 )
		if ( write(fd, buf, n) != n ) {
			fprintf(stderr, "write error\n");
			exit(1);
		}

	if (n < 0) {
		fprintf(stderr, "read error\n");
		exit(1);
	}
        close(fd);
	exit(0);
}
