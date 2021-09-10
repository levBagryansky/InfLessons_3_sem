#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

int get_file_size(int fd){
    if(fd == -1)
        return -1;

    struct stat buf;
    fstat(fd, &buf);
    off_t file_size = buf.st_size;
    return file_size;
}

int main(int argc, char** argv) {
    int fd_from = open(argv[1], O_RDONLY);
    int fd_to = open(argv[2], O_WRONLY | O_CREAT, 0666);
    printf("fd_to is: %i\n", fd_to);

    int file_size = get_file_size(fd_from);
    //printf("size is: %i\n", file_size);

    char* file_arr = calloc(file_size, sizeof(char));
    int is_read = read(fd_from, file_arr, file_size);

    int is_write = write(fd_to, file_arr, file_size);
    printf("is_write is: %i\n", is_write);
    close(fd_from);
    close(fd_to);
    free(file_arr);
    return 0;
}
