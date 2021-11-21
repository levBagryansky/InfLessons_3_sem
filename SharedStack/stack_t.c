#include "stack_t.h"
#include <sys/ipc.h>
#include <sys/shm.h>
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
        memset(result, 0, sizeof(void *) * size + sizeof(stack_t));
        pthread_mutex_init(&(result->mutex), NULL);
        pthread_mutex_lock(&result->mutex);
        result->size = size;
        result->count = 0;
        result->shm_stack_t_id = shm_stack_t_id;
        printf("in attach %i\n", shm_stack_t_id);
        result->data = (void **) &(result->data) + 1;
    } else{
        shm_stack_t_id = shmget(key, sizeof(void *) * size + sizeof(stack_t), IPC_CREAT | 0666);
        result = (stack_t *) shmat(shm_stack_t_id, NULL, 0);
        pthread_mutex_lock(&result->mutex);
    }

    result -> n_process ++;
    pthread_mutex_unlock(&result->mutex);
    return result;
}

int push(stack_t* stack, void* val){
    pthread_mutex_lock(&stack->mutex);
    if(stack->count == stack->size){
        return -1;
    }
    stack->data[stack->count] = val;
    stack->count ++;
    pthread_mutex_unlock(&stack->mutex);
    return 0;
}

int pop(stack_t* stack, void** val){
    pthread_mutex_lock(&stack->mutex);
    if(stack->count == 0){
        pthread_mutex_unlock(&stack->mutex);
        return -1;
    }

    stack->count --;
    *val = stack->data[stack->count];
    stack->data[stack->count] = NULL;
    pthread_mutex_unlock(&stack->mutex);
    return 0;
}

void PrintStack(stack_t* stack){
    pthread_mutex_lock(&stack->mutex);
    printf("Printing stack %d:\ncount = %d, size = %d\nelements:\n",stack->shm_stack_t_id, stack->count, stack->size);
    for (int i = 0; i < stack->size; ++i) {
        printf("%i) %p\n", i, (stack->data[i]));
    }
    pthread_mutex_unlock(&stack->mutex);
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