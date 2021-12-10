#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv) {
	if(argc != 2){
		printf("There must be 1 arguments\n");
		exit(1);
	}

	int fd = open(argv[1], O_RDONLY);
	printf("fd = %d\n", fd);
	if(fd == -1){
		printf("Can not open file\n");
		exit(1);
	}

	struct flock read_lock = {};
	memset(&read_lock, 0, sizeof(struct flock));

	int fcntl_ed = fcntl(fd, F_GETLK, &read_lock);
	if(fcntl_ed == -1){
		printf("fcntl returned -1, errno = %d\n", errno);
		perror("");
	}

	printf("whence = %d, type = %d, start = %ld, pid = %d\n", read_lock.l_whence, read_lock.l_type, read_lock.l_start, read_lock.l_pid);
	return 0;
}
