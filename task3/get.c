#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void PrintSig(int sig_num){
    printf("Sig = %i\n", sig_num);
}

int main(int argc, char** argv){
    if(argc != 2){
        printf("Wrong format\n");
    }

    printf("My pid = %i\n", getpid());
    signal(SIGUSR1, PrintSig);
    while (1){
        ;
    }
}
