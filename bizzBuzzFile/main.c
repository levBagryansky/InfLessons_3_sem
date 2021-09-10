#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/stat.h>

int get_file_size(int fd){
    if(fd == -1)
        return -1;

    struct stat buf;
    fstat(fd, &buf);
    off_t file_size = buf.st_size;
    return file_size;
}

char* file2arr(int fd){
    int fileLen = get_file_size(fd);
    char* arr = calloc(fileLen, sizeof(char));
    read(fd, arr, fileLen);
    return arr;
}

char* getWord(char* arr, int pozition, int arrSize){
    if(pozition >= arrSize)
        return 0;
    int i = pozition;
    while (i < arrSize && arr[i] != ' ' && i != '\n')
        i++;

    char* word = calloc(i - pozition, sizeof(char));
    for (int j = 0; j < i - pozition; ++j) {
        word[j] = arr[pozition + j]
    }
}

int main(int argc, char** argv)
{
    int fd_from = open(argv[1], O_RDONLY);
    if(fd_from == -1){
        printf("Can not \"%s\"\n", argv[1]);
        return 0;
    }

    int fd_to = open(argv[2], O_WRONLY | O_CREAT, 0666);
    if(fd_to == -1){
        printf("Can not write to file \"%s\"\n", argv[2]);
        return 0;
    }
    FILE* fp_from = fdopen(fd_from, "r");
    //fp = fopen(argv[1], "r");

    return 0;
}