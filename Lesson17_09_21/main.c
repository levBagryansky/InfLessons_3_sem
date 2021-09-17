#include <stdio.h>
#include <sys/stat.h>
#include <malloc.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/poll.h>

struct ts{
    int a;
    int b;
};

int f (int i, int j){
    int t;
     t = i + j;
    return t;
}

int main(int argc, char** argv) {
    struct pollfd* pollfds = (struct pollfd*) calloc(argc, sizeof (struct pollfd));
    for (int i = 0; i < argc - 1; ++i) {
        int fd = open(argv[i+1], O_RDONLY | O_NONBLOCK);
        pollfds[i].fd = fd;
        pollfds[i].events = POLLIN;
    }

    int poll_value;
    while (true){
        poll_value = poll(pollfds, argc - 1, -1);
        //printf("poll_value: %i\n", poll_value);
        for (int i = 0; i < argc - 1; ++i) {
            if(pollfds[i].revents > 0){
                int is_read;
                char c;
                while ((is_read = read(pollfds[i].fd, &c, 1)) == 1)
                    printf("%c", c);
            }
        }
    }

    return 0;
}
