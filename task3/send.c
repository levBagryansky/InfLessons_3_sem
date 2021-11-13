#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int GetFileSize(int fd);
int ArrToInt(char* arr);
void OnSygUsr1(int n){;}

int main(int argc, char** argv){

    printf("Sender's pid - %i\n", getpid());
    struct sigaction act;
    act.sa_handler = OnSygUsr1;
    sigset_t set;
    sigemptyset(&set);
    for (int i = 1; i < 32; ++i) {
        sigaddset(&set, i);
    }
    act.sa_mask = set;
    sigaction(SIGUSR1, &act, 0);
    sigaction(15, &act, 0);
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
    char* arr = (char*) calloc(len + 1, sizeof (char));
    read(fd, arr, len);
    int pid_get = ArrToInt(argv[2]);
    sigset_t set_usr15;
    sigemptyset(&set_usr15);
    sigaddset(&set_usr15, 15);
    char c = arr[0];
    char buf;
    int sig = 0;
    kill(pid_get, 31);

    for (int i = 1; i < len + 1; ++i) {
        for (int j = 0; j < 8; ++j) {
            sigwait(&set_usr15, &sig);
            buf = (c >> j) & 1U;
            usleep(1000);
            if(buf == 0){
                kill(pid_get, 10);
            } else if(buf == 1){
                kill(pid_get, 12);
            }
            //printf("i = %i, j = %i\n", i, j);
        }
        c = arr[i];
    }

    /*
    for (int j = 0; j < 8; ++j) {
        sigwait(&set_usr15, &sig);
        buf = (c >> j) & 1U;
        if(buf == 0){
            kill(pid_get, 10);
        } else if(buf == 1){
            kill(pid_get, 12);
        }
        printf("j = %j\n", j);
    }
    c = 'B';
    //sigwait(&set_usr15, &sig);
    for (int i = 0; i < 8; ++i) {
        sigwait(&set_usr15, &sig);
        buf = (c >> i) & 1U;
        if(buf == 0){
            kill(pid_get, 10);
        } else if(buf == 1){
            kill(pid_get, 12);
        }
        printf("i = %i\n", i);
    }
    //sigwait(&set_usr15, &sig);
    c = 'C';
    for (int i = 0; i < 8; ++i) {
        sigwait(&set_usr15, &sig);
        buf = (c >> i) & 1U;
        if(buf == 0){
            kill(pid_get, 10);
        } else if(buf == 1){
            kill(pid_get, 12);
        }
        printf("i = %i\n", i);
    }
    c = 'D';
    for (int i = 0; i < 8; ++i) {
        sigwait(&set_usr15, &sig);
        buf = (c >> i) & 1U;
        if(buf == 0){
            kill(pid_get, 10);
        } else if(buf == 1){
            kill(pid_get, 12);
        }
        printf("i = %i\n", i);
    } */
    sigwait(&set_usr15, &sig);

    kill(pid_get, 2);

    free(arr);
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
