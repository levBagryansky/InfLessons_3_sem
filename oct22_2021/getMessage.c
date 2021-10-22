#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf{
    long mtype;
    char text[1000];
};

int main() {
    char message[1000] = {0};
    key_t key = ftok("../", 1);
    int msqid = msgget(key, IPC_CREAT | 0666);

    while (1){
        struct msgbuf buf;
        buf.mtype = 1;
        msgrcv(msqid, &buf, sizeof(struct msgbuf) - sizeof(long), 1, 0);
        printf("%s\n", buf.text);

    }
}