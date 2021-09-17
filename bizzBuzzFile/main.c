#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>

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
    if(arr == NULL){
        printf("File is too big\n");
        exit(1);
    }
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

bool is_integer(char* word, int wordLen){
    if(wordLen == 1){
        if(word[0] >= '0' && word[0] <= '9')
            return true;
        return false;
    }

    if (!((word[0] >= '0' && word[0] <= '9') ||
        word[0] == '-' || word[0] == '+'))
        return false;

    int i = 1;
    for (; i < wordLen; ++i) {
        if(word[i] == '.')
            break;

        if(word[i] < '0' || word[i] > '9')
            return false;
    }

    if (i == wordLen - 1)
        return false;

    if(i < wordLen){
        for (int j = i + 1; j < wordLen; ++j) {
            if(word[j] != '0')
                return false;
        }
    }

    return true;
}

void printBizzBuzz(int fd, char* word, int wordLen){
    int sum = 0;
    int i = 0;
    if(wordLen > 1 && (word[0] == '+' || word[0] == '-'))
        i = 1;

    int numOfDot = i;
    while (numOfDot < wordLen){
        if(word[numOfDot] == '.')
            break;
        numOfDot++;
    }

    for (; i < numOfDot; ++i) {
        sum += (word[i] - '0');
    }

    int last_digit = word[numOfDot - 1] - '0';
    if(sum % 3 == 0){
        write(fd, "bizz", 4);
        return;
    }
    if(last_digit % 5 == 0){
        write(fd, "buzz", 4);
        return;
    }

    write(fd, word, wordLen);
}

int main(int argc, char** argv)
{
    if(argc != 3){
        printf("Wrong format\n");
        exit(1);
    }

    int fd_from = open(argv[1], O_RDONLY);
    if(fd_from == -1){
        printf("Can not open the file\n");
        exit(-1);
    }

    int fd_to = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fd_to == -1){
        printf("Can not write to file \"%s\"\n", argv[2]);
        exit(-1);
    }

    //FILE* fp_from = fdopen(fd_from, "r");
    //fp = fopen(argv[1], "r");
    int arrLen = 0;
    char* arr = file2arr(fd_from, &arrLen);
    //printf("File: %s\n", arr);
    int pozition = 0;
    int wordLen = 0;
    //printf("word: %s\n", getWord(arr, &pozition, arrLen, &wordLen));

    while (pozition < arrLen){
        char *word= getWord(arr, &pozition, arrLen, &wordLen);
        //int is_write = write(fd_to, word, wordLen);
        if (is_integer(word, wordLen))
            printBizzBuzz(fd_to, word, wordLen);
        else {
            int is_write = write(fd_to, word, wordLen);
        }
        free(word);
            //printf("<%s> is number.\n", word);
        //printf("is_write %i\n", is_write);
    }

    free(arr);
    close(fd_to);
    close(fd_from);
    return 0;
}