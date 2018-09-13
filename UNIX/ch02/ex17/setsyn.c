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
	int flags;

	int fd = open("./out",O_WRONLY);

	if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
		perror("Can't get file descriptor flags");
		exit(1);
	}

	flags |= O_SYNC;

	if (fcntl(fd, F_SETFL, flags) < 0) {
		perror("Can't set file descriptor flags");
		exit(1);
	}

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
