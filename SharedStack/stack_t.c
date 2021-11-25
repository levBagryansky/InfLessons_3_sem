#include "stack_t.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

struct timespec ts = {0, 1e6};

stack_t* attach_stack(key_t key, int size){
	/*
	struct sembuf sem_lock = {0, -1, 0};
	int sem_id = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
	if(sem_id == -1){ // семафор уже существует
		sem_id = semget(key, 1, IPC_CREAT | 0666);
		//semop(sem_id, &sem_lock, 1);
	}else {+
		semctl(sem_id, 	0, SETVAL, 1);
	}
	//semtimedop(sem_id, &sem_lock, 1, &ts);
	int semoped = semop(sem_id, &sem_lock, 1);
	printf("AAAAAAAsemop = %i\n", semoped);
	printf("Lock and attach stack\n");*/
	stack_t *result;
	int shm_stack_t_id = shmget(key, sizeof(void *) * size + sizeof(stack_t), IPC_CREAT | IPC_EXCL | 0666);
	if(shm_stack_t_id != -1) {// stack does not exist
		result = (stack_t *) shmat(shm_stack_t_id, NULL, 0);
		memset(result, 0, sizeof(void *) * size + sizeof(stack_t));
		result->n_process++;
		if(result == (stack_t *)(-1)){
			printf("shmat returned NULL\n");
		}
		printf("in attach %i\n", shm_stack_t_id);

		result->sem_id = semget(key, 1, IPC_CREAT | 0666);
		semctl(result->sem_id, 0, SETVAL, 1);
		//Lock(result);
		result->size = size;
		result->count = 0;
		result->shm_stack_t_id = shm_stack_t_id;
		result->data = (void **) &(result->data) + 1;
	} else{ //stack exists
		shm_stack_t_id = shmget(key, sizeof(void *) * size + sizeof(stack_t), IPC_CREAT | 0666);
		result = (stack_t *) shmat(shm_stack_t_id, NULL, 0);
		result->n_process++;
		if(result == (stack_t *)(-1)){
			printf("shmat returned NULL\n");
		}
		result->sem_id = semget(key, 1, IPC_CREAT | 0666);
		printf("sem_id = %i\n", result->sem_id);
		//Lock(result);
	}

	usleep(100000);
	//result -> n_process ++;
	//Unlock(result);
	//pthread_mutex_unlock(&result->mutex);
	return result;
}

void Lock(stack_t* stack){
	printf("In Lock, sem_id = %i\n", stack->sem_id);
	struct sembuf sem_lock = {0, -1, 0};
	semop(stack -> sem_id, &sem_lock, 1);
}

void Unlock(stack_t* stack){
	printf("In Unlock, sem_id = %i\n", stack->sem_id);
	struct sembuf sem_unlock = {0, 1, 0};
	semop(stack -> sem_id, &sem_unlock, 1);
}

int push(stack_t* stack, void* val){
	//semctl(stack->sem_id, 0, GETVAL);
	printf("In Lock Push, sem_id = %i\n", stack->sem_id);
	struct sembuf sem_lock = {0, -1, 0};
	semop(stack -> sem_id, &sem_lock, 1);
	if(stack->count == stack->size){
		return -1;
	}
	stack->data[stack->count] = val;
	stack->count ++;
	printf("pushed\n");
	Unlock(stack);
	return 0;
}

int pop(stack_t* stack, void** val){
	//pthread_mutex_lock(&stack->mutex);
	printf("In Lock Pop, sem_id = %i\n", stack->sem_id);
	struct sembuf sem_lock = {0, -1, 0};
	semop(stack -> sem_id, &sem_lock, 1);	if(stack->count == 0){
		pthread_mutex_unlock(&stack->mutex);
		return -1;
	}

	stack->count --;
	*val = stack->data[stack->count];
	stack->data[stack->count] = NULL;
	printf("poped\n");
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
		sleep(0.1);
	}
	printf("Finish printing\n");
	//pthread_mutex_unlock(&stack->mutex);
	Unlock(stack);
}

int mark_destruct(stack_t* stack){
	Lock(stack);
	printf("Marking destruct\n");
	stack-> to_destruct ++;
	int res = shmctl(stack->shm_stack_t_id, IPC_RMID, NULL);
	Unlock(stack);
	return res;
}

int detach_stack(stack_t* stack){
	printf("In Lock Detach, sem_id = %i\n", stack->sem_id);
	struct sembuf sem_lock = {0, -1, 0};
	semop(stack -> sem_id, &sem_lock, 1);	stack->n_process --;
	printf("stack->n_process = %d, stack -> to_destruct = %d\n", stack->n_process, stack -> to_destruct);
	if(stack->n_process == 0 && stack -> to_destruct) {
		//pthread_mutex_destroy(&stack->mutex);
		int buf_sem_id = stack->sem_id;
		shmdt(stack);
		//Unlock(stack);
		struct sembuf sem_unlock = {0, 1, 0};
		printf("Unlock and deleting stack\n");
		semctl(buf_sem_id, 0, IPC_RMID, NULL);
		semop(buf_sem_id, &sem_unlock, 1);
	} else{
		int buf_sem_id = stack->sem_id;
		shmdt(stack);
		//Unlock(stack);
		struct sembuf sem_unlock = {0, 1, 0};
		printf("Unlock and detaching stack\n");
		semop(buf_sem_id, &sem_unlock, 1);
	}
	return 1;
}