#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

int var_for_15 = 0;

int GetFileSize(int fd);
int ArrToInt(char* arr);
void OnSygUsr15(int n){var_for_15++;}

int main(int argc, char** argv){
    struct timespec begin,end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);

    printf("Sender's pid - %i\n", getpid());
    struct sigaction act;
    act.sa_handler = OnSygUsr15;
    act.sa_flags = SA_RESTART;
    sigset_t set;
    sigemptyset(&set);
    for (int i = 1; i < 32; ++i) {
        sigaddset(&set, i);
    }
    act.sa_mask = set;
    sigaction(SIGTERM, &act, 0);
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
    int new_sim_num = 0;
    int old_sim_num = 0;
    var_for_15 = 0;

    kill(pid_get, SIGSYS);
    while (var_for_15 == 0){;}
    var_for_15 = 0;

    for (int i = 1; i < len + 1; ++i) {
        for (int j = 0; j < 2; ++j) {
            buf = (c >> (4*j)) & 15U; // & 00001111

            switch (buf) {
                case 0:
                    kill(pid_get, SIGUSR1);
                    break;
                case 1:
                    kill(pid_get, SIGUSR2);
                    break;
                case 2:
                    kill(pid_get, SIGPIPE);
                    break;
                case 3:
                    kill(pid_get, SIGALRM);
                    break;
                case 4:
                    kill(pid_get, SIGTERM);
                    break;
                case 5:
                    kill(pid_get, SIGSTKFLT);
                    break;
                case 6:
                    kill(pid_get, SIGCHLD);
                    break;
                case 7:
                    kill(pid_get, SIGCONT);
                    break;
                case 8:
                    kill(pid_get, SIGTSTP);
                    break;
                case 9:
                    kill(pid_get, SIGTTIN);
                    break;
                case 10:
                    kill(pid_get, SIGTTOU);
                    break;
                case 11:
                    kill(pid_get, SIGURG);
                    break;
                case 12:
                    kill(pid_get, SIGXCPU);
                    break;
                case 13:
                    kill(pid_get, SIGXFSZ);
                    break;
                case 14:
                    kill(pid_get, SIGVTALRM);
                    break;
                case 15:
                    kill(pid_get, SIGPROF);
                    break;
            }
            while (var_for_15 == 0){;}
            var_for_15 = 0;
            //printf("i = %i, j = %i\n", i, j);
        }
        /*
        if(i % 16384 == 0) {
            old_sim_num = new_sim_num;
            new_sim_num = i * 100 / len;
            if (new_sim_num - old_sim_num) {
                printf("Processing... , wrote %i%%\n", new_sim_num);
            }
        }
        */
        c = arr[i];
    }


    kill(pid_get, 2);
    close(fd);
    free(arr);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    double time = (end.tv_sec - begin.tv_sec) + (((double)end.tv_nsec - (double)begin.tv_nsec) / (double)1000000000);

    printf("TIME %lg\n", time);
    printf("SIZE %d\n", len);
    printf("SPEED %f\n", (double)len/time);
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
