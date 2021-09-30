#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc != 2){
        printf("Wrong format\n");
        return 0;
    }

    FILE *fp = fopen(argv[1], "w");

    int pid = 0;
    for (int i = 0; i < 10; ++i) {
        int f = fork();
    }

    pid = getpid();
    fprintf(fp, "pid = %i ", pid);
    fprintf(fp, "parent = %i\n", getpid());
    return 0;
}
