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
        scanf("%s", message);
        struct msgbuf buf;
        buf.mtype = 1;
        for (int i = 0; i < 1000; ++i) {
            buf.text[i] = message[i];
        }
        msgsnd(msqid, &buf, sizeof(struct msgbuf) - sizeof(long), MSG_NOERROR);

    }
}