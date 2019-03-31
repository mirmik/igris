#include <unistd.h>
#include <stdio.h>

int fdputc(int c, int fd) {
	return write(fd, &c, 1);
}