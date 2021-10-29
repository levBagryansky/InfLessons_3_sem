#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// copy file

int GetFileSize(int fd);

int main(int argc, char **argv) {
    if(argc != 3){
        printf("Wrong format\n");
    }
    int fd_in = open(argv[1], O_RDONLY);
    if(fd_in == -1){
        printf("Cannot open file %s\n", argv[1]);
        exit(1);
    }
    int fd_out = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
    if(fd_out == -1){
        printf("Cannot open file %s\n", argv[2]);
        exit(1);
    }
    int size = GetFileSize(fd_in);
    char* src = mmap(0, size, PROT_READ, MAP_SHARED, fd_in, 0);
    lseek(fd_out, size - 1, SEEK_SET);
    write(fd_out, "", 1);
    char* dst = mmap(0, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd_out, 0);
    memcpy(dst, src, size);
    return 0;
}

int GetFileSize(int fd){
    if(fd == -1)
        return -1;

    struct stat buf;
    fstat(fd, &buf);
    off_t file_size = buf.st_size;
    return file_size;
}
