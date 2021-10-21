#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

int get_file_size(int fd);
char* file2arr(int fd, int* arrLen);
int numWords(char* arr,int lenArr);
int getWordLen(char* arr, int position, int arrLen);
int nextWord(char* arr, int position, int arrLen){
    while (position < arrLen){
        if (!isspace(arr[position])){
            break;
        }
        position++;
    }

    return position;
}

char* getWord(char* arr, int position, int arrLen){
    ;
}
char** arr2matrix(char* arr,int lenArr, int* pSize){
    int nWords = numWords(arr, lenArr);
    *pSize = nWords;
    char **matrix = (char**) calloc(nWords, sizeof(char*));
    int position = 0;
    for(int i = 0; i < nWords; i++){
        int lenWord = getWordLen(arr, position, lenArr);
        matrix[i] = (char*) calloc(lenWord + 1, sizeof(char));
        position = nextWord(arr, position, lenArr);
        for (int j = 0; j < lenWord; ++j) {
            matrix[i][j] = arr[position];
            position ++;
        }
    }

    return matrix;
}

void printMatrix(char** matrix, int size);

int numOfBar(char *arr, int *plen); //возвращает ближайший |
int lastBar(char *arr, int len);
void doBar(char *arr, int len){
    if (numOfBar(arr, &len) == 0);
}

int main(int argc, char** argv) {
    int fd_from = open(argv[1], O_RDONLY);
    if(fd_from == -1){
        printf("Can not open the file\n");
        exit(-1);
    }
    int len;
    char *arr = file2arr(fd_from, &len);
    //printf("Next word on the %i position\n", nextWord(arr, 31, len));
    int sizeMatrix;
    char** matrix = arr2matrix(arr, len, &sizeMatrix);
    printMatrix(matrix, sizeMatrix);
    return 0;
}

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

int numWords(char* arr,int lenArr){
    int numOfWords = 0;
    if(!isspace(arr[0])){
        numOfWords++;
    }
    for (int i = 0; i < lenArr - 1; ++i) {
        if(isspace(arr[i]) && !isspace(arr[i+1])){
            numOfWords++;
        }

        if(arr[i] == '|'){
            if(!isspace(arr[i+1])){
                numOfWords++;
            }
            if(i != 0 && !isspace(arr[i - 1])){
                numOfWords++;
                if (arr[i - 1] == '|'){
                    numOfWords --;
                }
            }
        }
    }
    if(arr[lenArr - 1] == '|'){
        if(lenArr - 1 != 0 && !isspace(arr[lenArr - 1 - 1])){
            numOfWords++;
            if (arr[lenArr - 1 - 1] == '|'){
                numOfWords --;
            }
        }
    }

    return numOfWords;
}

int getWordLen(char* arr, int position, int arrLen){
    while (position < arrLen && isspace(arr[position])){
        //printf( "Hello there\n");
        position++;
    }

    if (position == arrLen){
        return 0;
    }

    if(arr[position] == '|'){
        return 1;
    }

    int result = 0;
    while (position < arrLen && !isspace(arr[position]) && arr[position] != '|'){
        position++;
        result++;
    }

    return result;
}

void printMatrix(char** matrix, int size){
    for (int i = 0; i < size; ++i) {
        printf("%s\n", matrix[i]);
    }
}

int numOfBar(char *arr, int *plen){
    int result = 0;
    for (int i = 0; i < *plen; ++i) {
        if(arr[i] == '|'){
            result++;
        }
    }

    return result;
}

int lastBar(char *arr, int len){
    int i = len - 1;
    while (arr[i] != '|'){
        i--;
    }

    return i;
}
