#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int signals_to_use[8] = {SIGCLD};

int GetFileSize(int fd);
int ArrToInt(char* arr);

int main(int argc, char** argv){
    if(argc != 3){
        printf("Wrong format\n");
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd == -1){
        printf("Cannot open file");
        exit(1);
    }

    int len = GetFileSize(fd);
    char* arr = (char*) calloc(len, sizeof (char));
    read(fd, arr, len);
    int pid_get = ArrToInt(argv[2]);

    /*for(int i = 4; i < 9; i++) {
        kill(pid_get, i);
        printf("i = %i\n", i);
        sleep(1);
    } */
    /*
    for(int i = 10; i < 19; i++) {
        kill(pid_get, i);
        printf("i = %i\n", i);
        //sleep(1);
    } */

    for (int i = 33; i < 35; ++i) {
        kill(pid_get, i);
        printf("i = %i\n", i);
    }


   // printf("i = %i\n", 255);
   // kill(pid_get, 255);

}

int GetFileSize(int fd){
    if(fd == -1)
        return -1;

    struct stat buf;
    fstat(fd, &buf);
    off_t file_size = buf.st_size;
    return file_size;
}

int ArrToInt(char* arr){
    int result = 0;
    int i = 0;
    while (arr[i] != 0){
        result = result * 10 + arr[i] - '0';
        i++;
    }
    return result;
}
