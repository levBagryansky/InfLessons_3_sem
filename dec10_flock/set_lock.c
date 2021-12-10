#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>

int set_lock (int fd, long pos, long size);

int main (int argc, char *argv[]) {
	if (argc != 4) {
		perror ("Look at the desk");
	}

	errno = 0;
	const char *file_name = argv[1];
	const long  pos       = strtol (argv[2], NULL, 10);
	const long  size      = strtol (argv[3], NULL, 10);
	//printf ("pos = %ld\nsize = %ld\n", pos, size);
	if (errno) {
		perror("argument parsing failed");
	}

	if (pos < 0 || size < 0) {
		perror ("Invalid argiments");
	}

	int fd = open (file_name, O_RDWR);
	if (fd == -1) {
		perror ("bad file");
	}

	struct flock fl = {
			.l_type     = F_WRLCK,
			.l_whence   = SEEK_SET,
			.l_start    = pos,
			.l_len      = size,
			.l_pid      = getpid()
	};

	int locked = fcntl (fd, F_SETLK, &fl);

	if (locked == -1) {
		close (fd);
		perror ("Locking failed");
	}
	while (1){
		sleep(1);
	}

	return 0;
}

int set_lock (int fd, long pos, long size) {
	struct flock fl = {
			.l_type     = F_WRLCK,
			.l_whence   = SEEK_SET,
			.l_start    = pos,
			.l_len      = size,
			.l_pid      = getpid()
	};

	return fcntl (fd, F_SETLK, &fl);
}