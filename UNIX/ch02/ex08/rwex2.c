/*
 * This trivial program illustrates how we might normally read from and
 * write to a file.  Here, we simply open the source code file, read the
 * first few bytes, then append a comment.
 */

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE	10
#define COMMENT		"abcdefghij"
#define SOURCE 		"./test.txt"

int
main(int argc, char **argv) {
	int fd, n;
	int len;
	char buf[BUFFSIZE];

	if ((fd = open(SOURCE, O_RDWR /*| O_APPEND*/)) == -1) {
		fprintf(stderr, "Unable to open '%s': %s\n",
				SOURCE, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if ((n = read(fd, buf, BUFFSIZE)) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n) {
			fprintf(stderr, "Unable to write: %s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	printf("\n\nOk, we read the first %d bytes. Now let's write something.\n", BUFFSIZE);

	len = sizeof(COMMENT) - 1;
	if (write(fd, COMMENT, len) != len) {
			fprintf(stderr, "Unable to write: %s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
	}

	if ((n = read(fd, buf, BUFFSIZE)) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n) {
			fprintf(stderr, "Unable to write: %s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	printf("\n\nOk, we read the second %d bytes. Now let's exit.\n", BUFFSIZE);

	close(fd);
	return EXIT_SUCCESS;
}
