#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>

int getFileSize(int fd);
char* file2arr(int fd, int* arrLen);
int numWords(char* arr,int lenArr);
int getWordLen(char* arr, int position, int arrLen);
int nextWord(char* arr, int position, int arrLen);
char** arr2matrix(char* arr,int lenArr, int* pSize); // Также добавляет Null перед каждой |
void printMatrix(char** matrix, int size);
void freeMatrix(char ** matrix, int size);
int numOfBarArr(char *arr, int len); //возвращает количество |
int numOfBarMatrix(char** matrix, int size);
int lastBarArr(char *arr, int len); //Находит последний | в массиве
int lastBarMatrix(char ** matrix, int size);

void doBar(char** matrix, int size){
    int numBar = numOfBarMatrix(matrix, size);
    if (numBar == 0){ // Значит, здесь находится какая-то программа, которую следует запустить
        int forked = fork();
        if(forked == 0){// потомок
            execvp(matrix[0], matrix);
            printf("Command \"%s\" returned error.\n", matrix[0]);
            perror("Error: ");
        }
        if(forked){ //родитель
            int status = 1;
            waitpid(forked, &status, 0);
        }
    }
    if(numBar== 1){ //
        int fd[2];
        pipe(fd);
        pid_t forked = fork();
        if (forked == 0){ // Выполняет процесс слева от |
            //printf("Hello there\n");
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            execvp(matrix[0], matrix);
            dup2(10, STDOUT_FILENO);
            printf("Command \"%s\" returned error.\n", matrix[0]);
            perror("Error2");
            exit(1);
        }
        else if(forked > 0){ // Родитель выполняет справа от |
            int status;
            waitpid(forked, &status, 0);
            if (status != 0) {
                close(fd[1]);
                close(fd[2]);
                printf("Ex\n");
                exit(status);
            }
            forked = fork();
            if(forked == 0) { // следующий потомок
                close(fd[1]);
                dup2(fd[0], STDIN_FILENO);
                int barI = lastBarMatrix(matrix, size);
                execvp(matrix[barI + 1], matrix + barI + 1);
                dup2(10, STDOUT_FILENO);
                printf("Command \"%s\" returned error.\n", matrix[barI + 1]);
                perror("Error1");
                close(fd[0]);
                close(fd[1]);
                exit(1);
            }
            else{ // родитель
                close(fd[0]);
                close(fd[1]);
                //waitpid(forked, NULL, WCONTINUED);
                waitpid(forked, &status, 0);
                if (status != 0) {
                    close(fd[1]);
                    close(fd[2]);
                    exit(status);
                }
                exit(1);
            }
        }
    }
    if(numBar > 1){
        int fd[2];
        pipe(fd);
        int forked = fork();
        if (forked == 0){ //потомок выполняет слева от последнего |
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            doBar(matrix, lastBarMatrix(matrix, size));
            close(fd[0]);
            close(fd[1]);
            exit(0);
        } else{ // родитель выполняет справа от последнего |
            close(fd[1]);
            close(fd[2]);
            int status;
            waitpid(forked, &status, WCONTINUED);
            if(status != 0) {
                printf("Exit\n");
                exit(status);
            }

            forked = fork();
            if(forked == 0) { // следующий потомок
                close(fd[1]);
                dup2(fd[0], STDIN_FILENO);
                int barI = lastBarMatrix(matrix, size);
                execvp(matrix[barI + 1], matrix + barI + 1);
                dup2(10, STDOUT_FILENO);
                printf("Command \"%s\" returned error.\n", matrix[barI + 1]);
                perror("Error");
                close(fd[0]);
                close(fd[1]);
                exit(1);
            }
            else{ // родитель
                close(fd[0]);
                close(fd[1]);

                waitpid(forked, &status, WCONTINUED);
                exit(1);
            }
        }
    }
}

int main(int argc, char** argv) {
    dup2(STDOUT_FILENO, 10);
    int fd_from = open(argv[1], O_RDONLY);
    if(fd_from == -1){
        printf("Can not open the file\n");
        exit(-1);
    }
    int len;
    char *arr = file2arr(fd_from, &len);
    close(fd_from);
    int sizeMatrix;
    char** matrix = arr2matrix(arr, len, &sizeMatrix);
    doBar(matrix, sizeMatrix);

    freeMatrix(matrix, sizeMatrix);
    free(arr);
    return 0;
}

int getFileSize(int fd){
    if(fd == -1)
        return -1;

    struct stat buf;
    fstat(fd, &buf);
    off_t file_size = buf.st_size;
    return file_size;
}

char* file2arr(int fd, int* arrLen){
    int fileLen = getFileSize(fd);
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

int nextWord(char* arr, int position, int arrLen){
    while (position < arrLen){
        if (!isspace(arr[position])){
            break;
        }
        position++;
    }

    return position;
}

char** arr2matrix(char* arr,int lenArr, int* pSize){
    int nWords = numWords(arr, lenArr) + numOfBarArr(arr, lenArr);
    *pSize = nWords + 1;
    char **matrix = (char**) calloc(nWords + 1, sizeof(char*));
    int position = 0;
    for(int i = 0; i < nWords; i++){
        int lenWord = getWordLen(arr, position, lenArr);
        matrix[i] = (char*) calloc(lenWord + 1, sizeof(char));
        position = nextWord(arr, position, lenArr);
        for (int j = 0; j < lenWord; ++j) {
            matrix[i][j] = arr[position];
            position ++;
        }
        if(matrix[i][0] == '|'){
            i++;
            matrix[i] = NULL;
            char* buf = matrix[i];
            matrix[i] = matrix[i - 1];
            matrix[i - 1] = buf;
        }
    }
    matrix[nWords] = NULL;
    return matrix;
}

void printMatrix(char** matrix, int size){
    printf("Size of matrix: %i\n", size);
    for (int i = 0; i < size; ++i) {
        printf("%i) %s\n", i, matrix[i]);
    }
}

void freeMatrix(char ** matrix, int size){
    for (int i = 0; i < size; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

int numOfBarArr(char *arr, int len){
    int result = 0;
    for (int i = 0; i < len; ++i) {
        if(arr[i] == '|'){
            result++;
        }
    }

    return result;
}

int numOfBarMatrix(char** matrix, int size){
    int result = 0;
    for (int i = 0; i < size; ++i) {
        if(matrix[i] != NULL)
        if(matrix[i][0] == '|'){
            result++;
        }
    }
    return result;
}

int lastBarArr(char *arr, int len){
    int i = len - 1;
    while (arr[i] != '|'){
        i--;
    }

    return i;
}

int lastBarMatrix(char ** matrix, int size){
    for (int i = size - 1; i >= 0; --i) {
        if (matrix[i] != NULL && matrix[i][0] == '|'){
            return i;
        }
    }

    return -1;
}
