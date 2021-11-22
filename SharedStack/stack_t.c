#include "stack_t.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

stack_t* attach_stack(key_t key, int size){
    int shm_stack_t_id = shmget(key, sizeof(void *) * size + sizeof(stack_t), IPC_CREAT | IPC_EXCL | 0666);
    stack_t *result;
    if(shm_stack_t_id != -1) {// stack does not exist
        result = (stack_t *) shmat(shm_stack_t_id, NULL, 0);
        if(result == (stack_t *)(-1)){
            printf("shmat returned NULL\n");
        }
        memset(result, 0, sizeof(void *) * size + sizeof(stack_t));
        //pthread_mutex_init(&(result->mutex), NULL);
        //pthread_mutex_lock(&result->mutex);
        result->sem_id = semget(key, 1, IPC_CREAT | 0666);
        semctl(result->sem_id, 0, SETVAL, 1);
        result->size = size;
        result->count = 0;
        result->shm_stack_t_id = shm_stack_t_id;
        printf("in attach %i\n", shm_stack_t_id);
        result->data = (void **) &(result->data) + 1;
    } else{
        shm_stack_t_id = shmget(key, sizeof(void *) * size + sizeof(stack_t), IPC_CREAT | 0666);
        result = (stack_t *) shmat(shm_stack_t_id, NULL, 0);
        if(result == (stack_t *)(-1)){
            printf("shmat returned NULL\n");
        }
        //pthread_mutex_lock(&result->mutex);
    }

    result -> n_process ++;
    //pthread_mutex_unlock(&result->mutex);
    return result;
}

void Lock(stack_t* stack){
    struct sembuf sem_lock = {0, -1, 0};
    semop(stack -> sem_id, &sem_lock, 1);
}

void Unlock(stack_t* stack){
    struct sembuf sem_unlock = {0, 1, 0};
    semop(stack -> sem_id, &sem_unlock, 1);
}

int push(stack_t* stack, void* val){
    //pthread_mutex_lock(&stack->mutex);
    //semctl(stack->sem_id, 0, GETVAL);
    Lock(stack);
    if(stack->count == stack->size){
        return -1;
    }
    stack->data[stack->count] = val;
    stack->count ++;
    //pthread_mutex_unlock(&stack->mutex);
    Unlock(stack);
    return 0;
}

int pop(stack_t* stack, void** val){
    //pthread_mutex_lock(&stack->mutex);
    Lock(stack);
    if(stack->count == 0){
        pthread_mutex_unlock(&stack->mutex);
        return -1;
    }

    stack->count --;
    *val = stack->data[stack->count];
    stack->data[stack->count] = NULL;
    //pthread_mutex_unlock(&stack->mutex);
    Unlock(stack);
    return 0;
}

void PrintStack(stack_t* stack){
    //pthread_mutex_lock(&stack->mutex);
    Lock(stack);
    printf("Printing stack %d:\ncount = %d, size = %d\nelements:\n",stack->shm_stack_t_id, stack->count, stack->size);
    for (int i = 0; i < stack->size; ++i) {
        printf("%i) %p\n", i, (stack->data[i]));
        sleep(1);
    }
    printf("Finish printing\n");
    //pthread_mutex_unlock(&stack->mutex);
    Unlock(stack);
}

int mark_destruct(stack_t* stack){
    //stack-> to_destruct ++;
    return shmctl(stack->shm_stack_t_id, IPC_RMID, NULL);
}

int detach_stack(stack_t* stack){
    stack->n_process --;
    if(stack->n_process == 0 && stack -> to_destruct)
        pthread_mutex_destroy(&stack->mutex);
    shmdt(stack);
    return 0;
}