#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

typedef void (*handler)();

int getted_sygnal = 0;
int fd_to;
char c;
uint counter = 0;
pid_t send_pid = 0;

void PrintSig(){
    printf("Sig = %i\n", getted_sygnal);
//    exit(1);
}
void OnSygSIGUSR1(int sig_num);
void OnSygSIGUSR2(int sig_num);
void OnSyg2(int sig_num);
void sa_sigaction31(int sig_num, siginfo_t* inf, void * p);

int main(int argc, char** argv){
    if(argc != 2){
        printf("Wrong format\n");
        exit(1);
    }
    fd_to = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    printf("My pid = %i\n", getpid());
    struct sigaction act1;
    struct sigaction act2;
    struct sigaction act3;
    struct sigaction act31; // сигнал 31 посылается в самом начале, чтобы получить pid передатчика
    memset(&act31, 0, sizeof (act31));
    sigset_t set;
    sigemptyset(&set);
    for (int i = 1; i < 32; ++i) {
        sigaddset(&set, i);
    }

    //struct sa_
    act31.sa_mask = set;
    act31.sa_sigaction = sa_sigaction31;
    act31.sa_flags = SA_SIGINFO;
    sigaction(31, &act31, 0);


    act3.sa_mask = set;
    act3.sa_handler = OnSyg2;
    act1.sa_mask = set;
    act1.sa_handler = OnSygSIGUSR1;
    act2.sa_mask = set;
    act2.sa_handler = OnSygSIGUSR2;
    sigaction(SIGUSR1, &act1, 0);
    sigaction(SIGUSR2, &act2, 0);
    sigaction(2, &act3, 0);
    while (send_pid == 0){;}
    sigset_t set_usr1_usr2;
    sigemptyset(&set_usr1_usr2);
    sigaddset(&set_usr1_usr2, SIGUSR1);
    sigaddset(&set_usr1_usr2, SIGUSR2);
    char buf;
    kill(send_pid, 10);
    while (1){
        printf("counter = %i\n", counter);
        sigwait(&set_usr1_usr2, &getted_sygnal);
        if(getted_sygnal == 10){
            buf = 0;
        } else if(getted_sygnal == 12){
            buf = 1;
        }
        buf <<= counter;
        c |= buf;
        counter++;
        if(counter == 8){
            printf("writing %c\n", c);
            write(fd_to, &c, 1);
            counter = 0;
        }
        kill(send_pid, 10);
    }
}

void OnSyg2(int sig_num){
    //printf("x = %i\n", sig_num);
    getted_sygnal = 2;
    PrintSig();
    close(fd_to);
    exit(1);
}

void OnSygSIGUSR1(int sig_num){
    /*
    if (counter % 8 == 0){
        c = 0;
    }
    c = 2 * c + 0;
    counter++;
    if(counter % 8 == 0){
        write(fd_to, &c, 1);
    }
     */
    printf("SIGUSR1\n");
    //getted_sygnal = SIGUSR1; PrintSig();
}

void OnSygSIGUSR2(int sig_num){
    printf("SIGUSR2\n");
    /*
    if (counter % 8 == 0){
        c = 0;
    }

    c <<= 1;
    c |= 1;
    counter++;
    if(counter % 8 == 0){
        write(fd_to, &c, 1);
    }
    printf("counter in 2 = %i\n", counter);
    */
}

void OnSyg31(int sig_num){
    ;
}

void sa_sigaction31(int sig_num, siginfo_t* inf, void * p){
    send_pid = inf -> si_pid;
    printf("OMG I get signal from %i\n", inf->si_pid);
}
