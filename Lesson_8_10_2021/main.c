#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void print1(){
    printf("London ");
    fflush(stdout);
}

void print2(){
    printf("is the capital of Great Britain\n");
}

int main() {
    printf("Hi!, my pid is %i\n", getpid());

    struct sigaction act_usr1;
    act_usr1.sa_handler = &print1;

    struct sigaction act_usr2;
    act_usr2.sa_handler = &print2;
    act_usr2.sa_flags = SA_RESTART;
    act_usr1.sa_flags = SA_RESTART;

    int sig1_return = sigaction(SIGUSR1, &act_usr1, NULL);
    int sig2_return = sigaction(SIGUSR2, &act_usr2, NULL);
    if(sig1_return == -1 || sig2_return == -1){
        printf("Error Error\n");
    }

    while (1){
        //sleep(1);
    }
    return 0;
}

