#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

typedef void (*handler)();

int getted_sygnal = 0;
int fd_to;

void PrintSig(){
    printf("Sig = %i\n", getted_sygnal);
//    exit(1);
}

void OnSyg0();
void OnSyg1();
void OnSyg2();
void OnSyg3();
void OnSyg4();
void OnSyg5();
void OnSyg6();
void OnSyg7();
void OnSyg8();
void OnSyg9();
void OnSyg10();
void OnSyg11();
void OnSyg12();
void OnSyg13();
void OnSyg14();
void OnSyg15();
void OnSyg16();
void OnSyg17();
void OnSyg18();
void OnSyg19();
void OnSyg20();
void OnSyg255();//{getted_sygnal = 255; PrintSig();}

void OnSyg25(){getted_sygnal = 0; PrintSig();}
void OnSyg26(){getted_sygnal = 1; PrintSig();}
void OnSyg27(){getted_sygnal = 2; PrintSig();}
void OnSyg28(){getted_sygnal = 3; PrintSig();}
void OnSyg29(){getted_sygnal = 4; PrintSig();}
void OnSyg30(){getted_sygnal = 5; PrintSig();}
void OnSyg31(){getted_sygnal = 6; PrintSig();}
void OnSyg32(){getted_sygnal = 7; PrintSig();}
void OnSyg33(){getted_sygnal = 8; PrintSig();}
void OnSyg34(){getted_sygnal = 9; PrintSig();}
void OnSyg35(){getted_sygnal = 10; PrintSig();}
void OnSyg36(){getted_sygnal = 11; PrintSig();}
void OnSyg37(){getted_sygnal = 12; PrintSig();}
void OnSyg38(){getted_sygnal = 13; PrintSig();}
void OnSyg39(){getted_sygnal = 14; PrintSig();}
void OnSyg40(){getted_sygnal = 15; PrintSig();}

handler handlers[41] = {OnSyg0, OnSyg1, OnSyg2, OnSyg3, OnSyg4, OnSyg5, OnSyg6, OnSyg7, OnSyg8, OnSyg9, OnSyg10,
                        OnSyg11, OnSyg12, OnSyg13, OnSyg14, OnSyg15, OnSyg16, OnSyg17, OnSyg18, OnSyg19, OnSyg20,
                        NULL, NULL, NULL, NULL, OnSyg25, OnSyg26, OnSyg27, OnSyg28, OnSyg29, OnSyg30,
                        OnSyg31, OnSyg32, OnSyg33, OnSyg34, OnSyg35, OnSyg36, OnSyg37, OnSyg38, OnSyg39, OnSyg40};


int main(int argc, char** argv){
    if(argc != 2){
        printf("Wrong format\n");
        exit(1);
    }
    fd_to = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    printf("My pid = %i\n", getpid());
    struct sigaction acts[50];
    sigset_t set;
    sigemptyset(&set);
    for (int i = 1; i < 50; ++i) {
        sigaddset(&set, i);
    }
    for (int i = 0; i < 50; ++i) {
        acts[i].sa_mask = set;
    }
    for (int i = 0; i < 41; ++i) {
        acts[i].sa_handler = handlers[i];
        sigaction(i, &(acts[i]), 0);
    }
    //signal(255, OnSyg255);
    while (1){;}
}

void OnSyg0(){
    getted_sygnal = 0;
}
void OnSyg1(){getted_sygnal = 1; PrintSig();}
void OnSyg2(){getted_sygnal = 2;
    PrintSig();
    close(fd_to);
    exit(1);
}
void OnSyg3(){getted_sygnal = 3; PrintSig();}
void OnSyg4(){getted_sygnal = 4; PrintSig();}
void OnSyg5(){getted_sygnal = 5; PrintSig();}
void OnSyg6(){getted_sygnal = 6; PrintSig();}
void OnSyg7(){getted_sygnal = 7; PrintSig();}
void OnSyg8(){getted_sygnal = 8; PrintSig();}
void OnSyg9(){getted_sygnal = 9; PrintSig();}
void OnSyg10(){getted_sygnal = 10; PrintSig();}
void OnSyg11(){getted_sygnal = 11; PrintSig();}
void OnSyg12(){getted_sygnal = 12; PrintSig();}
void OnSyg13(){getted_sygnal = 13; PrintSig();}
void OnSyg14(){getted_sygnal = 14; PrintSig();}
void OnSyg15(){getted_sygnal = 15; PrintSig();}
void OnSyg16(){getted_sygnal = 16; PrintSig();}
void OnSyg17(){getted_sygnal = 17; PrintSig();}
void OnSyg18(){getted_sygnal = 18; PrintSig();}
void OnSyg19(){getted_sygnal = 19; PrintSig();}
void OnSyg20(){getted_sygnal = 20; PrintSig();}
void OnSyg255(){getted_sygnal = 255; PrintSig();}

