#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    //int n = execl("/bin/ls", "ls", "-la", NULL);
    if (argc < 3) {
        printf("Wrong format\n");
        return 0;
    }

    int fdOut = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    dup2(fdOut, STDOUT_FILENO);
    int exected = execvp(argv[2], argv + 2);
    printf("We are here, its a mistake\n");
    return 0;
}
