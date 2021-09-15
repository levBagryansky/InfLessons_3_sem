#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#define buf_size 2

int get_file_size(int fd){
    if(fd == -1)
        return -1;

    struct stat buf;
    fstat(fd, &buf);
    off_t file_size = buf.st_size;
    return file_size;
}

int clean_file(char* input){

}

int main(int argc, char** argv) {
    FILE *fp = fopen(argv[2], "w");
    fclose(fp);
    int fd_from = open(argv[1], O_RDONLY);
    int fd_to = open(argv[2], O_WRONLY | O_CREAT, 0666);

    int buf_arr[buf_size] = {0};

    int file_size = get_file_size(fd_from);

    int is_read = 0;
    int is_write = 0;

    for (int i = 0; i < (file_size / buf_size); ++i){
        is_read = read(fd_from, buf_arr, buf_size);
        is_write = write(fd_to, buf_arr, buf_size);
        printf("is_write is: %i\nis_read is: %i\n", is_write, is_read);
    }
    is_read = read(fd_from, buf_arr, file_size % buf_size);
    is_write = write(fd_to, buf_arr, file_size % buf_size);

    close(fd_from);
    close(fd_to);
    return 0;
}
