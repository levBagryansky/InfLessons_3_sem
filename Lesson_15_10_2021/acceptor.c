#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Wrong format\n");
        return 1;
    }

    int fdTo = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    key_t key = ftok("../", 1);
    key_t keySize = ftok("../", 2);
    int shmSizeId = shmget(keySize, sizeof (int), IPC_CREAT|0666);

    int* size = (int*) shmat(shmSizeId, NULL, O_RDONLY);
    int shmFileId = shmget(key, *size, IPC_CREAT | 0666);
    char* arr = (char*) shmat(shmFileId, NULL, O_RDONLY);
    write(fdTo, arr, *size);

    shmctl(shmFileId, IPC_RMID, NULL);
    shmctl(shmSizeId, IPC_RMID, NULL);
    shmdt(arr);
    shmdt(size);
}