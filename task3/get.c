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
}
void OnSygSIGUSR1(int sig_num);
void OnSygSIGUSR2(int sig_num);
void OnSyg2(int sig_num);
void OnSyg13(int sig_num){getted_sygnal = 2;}
void OnSyg14(int sig_num){getted_sygnal = 3;}
void OnSyg15(int sig_num){getted_sygnal = 4;}
void OnSyg16(int sig_num){getted_sygnal = 5;}
void OnSyg17(int sig_num){getted_sygnal = 6;}
void OnSyg18(int sig_num){getted_sygnal = 7;}
void OnSyg20(int sig_num){getted_sygnal = 8;}
void OnSyg21(int sig_num){getted_sygnal = 9;}
void OnSyg22(int sig_num){getted_sygnal = 10;}
void OnSyg23(int sig_num){getted_sygnal = 11;}
void OnSyg24(int sig_num){getted_sygnal = 12;}
void OnSyg25(int sig_num){getted_sygnal = 13;}
void OnSyg26(int sig_num){getted_sygnal = 14;}
void OnSyg27(int sig_num){getted_sygnal = 15;}

void sa_sigaction31(int sig_num, siginfo_t* inf, void * p);

int main(int argc, char** argv){
    if(argc != 2){
        printf("Wrong format\n");
        exit(1);
    }
    fd_to = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    printf("My pid = %i\n", getpid());
    struct sigaction act10;
    struct sigaction act12;
    struct sigaction act13;
    struct sigaction act14;
    struct sigaction act15;
    struct sigaction act16;
    struct sigaction act17;
    struct sigaction act18;
    struct sigaction act20;
    struct sigaction act21;
    struct sigaction act22;
    struct sigaction act23;
    struct sigaction act24;
    struct sigaction act25;
    struct sigaction act26;
    struct sigaction act27;
    struct sigaction act3;
    struct sigaction act31; // сигнал 31 посылается в самом начале, чтобы получить pid передатчика
    memset(&act31, 0, sizeof (act31));
    memset(&act10, 0, sizeof (act31));
    memset(&act12, 0, sizeof (act31));
    sigset_t set;
    sigemptyset(&set);
    for (int i = 1; i < 32; ++i) {
        sigaddset(&set, i);
    }

    //struct sa_
    act31.sa_mask = set;
    act31.sa_flags = SA_SIGINFO;
    act31.sa_sigaction = sa_sigaction31;
    sigaction(31, &act31, 0);

    act3.sa_mask = set;
    act3.sa_handler = OnSyg2;
    act10.sa_mask = set;
    act10.sa_handler = OnSygSIGUSR1;
    act10.sa_flags = SA_RESTART;
    act12.sa_mask = set;
    act13.sa_mask = set;
    act14.sa_mask = set;
    act15.sa_mask = set;
    act16.sa_mask = set;
    act17.sa_mask = set;
    act18.sa_mask = set;
    act20.sa_mask = set;
    act21.sa_mask = set;
    act22.sa_mask = set;
    act23.sa_mask = set;
    act24.sa_mask = set;
    act25.sa_mask = set;
    act26.sa_mask = set;
    act27.sa_mask = set;
    act12.sa_handler = OnSygSIGUSR2;
    act13.sa_handler = OnSyg13;
    act14.sa_handler = OnSyg14;
    act15.sa_handler = OnSyg15;
    act16.sa_handler = OnSyg16;
    act17.sa_handler = OnSyg17;
    act18.sa_handler = OnSyg18;
    act20.sa_handler = OnSyg20;
    act21.sa_handler = OnSyg21;
    act22.sa_handler = OnSyg22;
    act23.sa_handler = OnSyg23;
    act24.sa_handler = OnSyg24;
    act25.sa_handler = OnSyg25;
    act26.sa_handler = OnSyg26;
    act27.sa_handler = OnSyg27;
    act12.sa_flags = SA_RESTART;
    act13.sa_flags = SA_RESTART;
    act14.sa_flags = SA_RESTART;
    act15.sa_flags = SA_RESTART;
    act16.sa_flags = SA_RESTART;
    act17.sa_flags = SA_RESTART;
    act18.sa_flags = SA_RESTART;
    act20.sa_flags = SA_RESTART;
    act21.sa_flags = SA_RESTART;
    act22.sa_flags = SA_RESTART;
    act23.sa_flags = SA_RESTART;
    act24.sa_flags = SA_RESTART;
    act25.sa_flags = SA_RESTART;
    act26.sa_flags = SA_RESTART;
    act27.sa_flags = SA_RESTART;
    sigaction(10, &act10, 0);
    sigaction(12, &act12, 0);
    sigaction(13, &act13, 0);
    sigaction(14, &act14, 0);
    sigaction(15, &act15, 0);
    sigaction(16, &act16, 0);
    sigaction(17, &act17, 0);
    sigaction(18, &act18, 0);
    sigaction(20, &act20, 0);
    sigaction(21, &act21, 0);
    sigaction(22, &act22, 0);
    sigaction(23, &act23, 0);
    sigaction(24, &act24, 0);
    sigaction(25, &act25, 0);
    sigaction(26, &act26, 0);
    sigaction(27, &act27, 0);
    sigaction(2, &act3, 0);

    while (send_pid == 0){;}

    sigset_t set_usr1_usr2;
    sigemptyset(&set_usr1_usr2);
    sigaddset(&set_usr1_usr2, SIGUSR1);
    sigaddset(&set_usr1_usr2, SIGUSR2);
    getted_sygnal = -1;
    char buf;
    kill(send_pid, 15);
    while (1){
        while (getted_sygnal == -1){;};
        //sigwait(&set_usr1_usr2, &getted_sygnal);
        //printf("wait = %i\n", counter);
        buf = getted_sygnal;
        buf <<= (4 * counter);
        c |= buf;
        counter++;
        if(counter == 2){
            //printf("writing %c\n", c);
            write(fd_to, &c, 1);
            counter = 0;
            c = 0;
        }
        getted_sygnal = -1;
        //usleep(100);
        kill(send_pid, 15);
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
    //printf("SIGUSR1\n");
    getted_sygnal = 0;
}

void OnSygSIGUSR2(int sig_num){
    //printf("SIGUSR2\n");
    getted_sygnal = 1;
}

void sa_sigaction31(int sig_num, siginfo_t* inf, void * p){
    send_pid = inf -> si_pid;
    printf("OMG I get signal from %i\n", inf->si_pid);
}
