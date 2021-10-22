#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    msgget();
    struct msgbuf mmbuf;
    return 0;
}
