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

char* file2arr(int fd, int* arrLen){
    int fileLen = get_file_size(fd);
    char* arr = calloc(fileLen, sizeof(char));
    read(fd, arr, fileLen);
    *arrLen = fileLen;
    return arr;
}

char* getWord(char* arr, int* ppozition, int arrSize, int* pWordLen){
    if(*ppozition >= arrSize)
        return 0;
    if(arr[*ppozition] == ' ' || arr[*ppozition] == '\n'){
        *pWordLen = 1;
        char* word = calloc(1, sizeof(char));
        word[0] = arr[*ppozition];
        (*ppozition)++;
        return word;
    }
    int i = *ppozition;
    while (i < arrSize && arr[i] != ' ' && arr[i] != '\n')
        i++;

    *pWordLen = i - *ppozition;
    char* word = calloc(*pWordLen, sizeof(char));
    for (int j = 0; j < *pWordLen; ++j) {
        word[j] = arr[*ppozition + j];
    }
    *ppozition = i;
    return word;
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
    int arrLen = 0;
    char* arr = file2arr(fd_from, &arrLen);
    printf("File: %s\n", arr);
    int pozition = 0;
    int wordLen = 0;
    //printf("word: %s\n", getWord(arr, &pozition, arrLen, &wordLen));

    return 0;
}