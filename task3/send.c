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
    sigset_t set_usr1;
    sigemptyset(&set_usr1);
    sigaddset(&set_usr1, SIGUSR1);
    char c = 'A';
    char buf;
    int sig = 0;
    kill(pid_get, 31);

    for (int i = 0; i < 8; ++i) {
        sigwait(&set_usr1, &sig);
        buf = (c >> i) & 1U;
        if(buf == 0){
            kill(pid_get, 10);
        } else if(buf == 1){
            kill(pid_get, 12);
        }
    }
    sigwait(&set_usr1, &sig);
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
