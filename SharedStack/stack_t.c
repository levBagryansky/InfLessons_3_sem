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
struct sembuf sem_unlock = {0, 1, 0};
struct sembuf sem_lock = {0, -1, 0};

stack_t* attach_stack(key_t key, int size){
	stack_t *result;
	int sem_id = semget(key, 1, 0666| IPC_CREAT | IPC_EXCL);
	if(sem_id == -1){// семафор сущесвтует
		//semctl(sem_id, 0, SETVAL, 1);
		sem_id = semget(key, 1, 0666| IPC_CREAT);
		semop(sem_id, &sem_lock, 1);
		semctl(sem_id, 0, SETVAL, 0);
	}
	else { // семафор не существует
		semctl(sem_id, 0, SETVAL, 1);
		semop(sem_id, &sem_lock, 1); // теперь можно быть увереным что никто параллельно ничего не делает
		semctl(sem_id, 0, SETVAL, 0);
	}

	int shm_id = shmget(key, sizeof(void *) * size + sizeof(stack_t), 0666| IPC_CREAT | IPC_EXCL);
	if (shm_id == -1){ // stack exists => it mustn't be deleted
		printf("Stack exists, just shmat...\n");
		shm_id = shmget(key, sizeof(void *) * size + sizeof(stack_t), 0666| IPC_CREAT);
		result = (stack_t*)shmat(shm_id, NULL, 0);
		result->n_process ++;
		printf("shmated\n");
	} else{
		printf("Stack does not exists, creating...\n");
		result = (stack_t* )shmat(shm_id, NULL, 0);
		memset(result, 0, sizeof(void *) * size + sizeof(stack_t));
		result->n_process++;
		result->sem_id = sem_id;
		result->count = 0;
		result->size = size;
		result->data = (void **) &(result->data) + 1;
		result->shm_stack_t_id = shm_id;
		printf("created\n");
	}

	semop(sem_id, &sem_unlock, 1);
	return result;
}

void Lock(stack_t* stack){
	printf("In Lock, sem_id = %i\n", stack->sem_id);
	semop(stack -> sem_id, &sem_lock, 1);
}

void Unlock(stack_t* stack){
	printf("In Unlock, sem_id = %i\n", stack->sem_id);
	semop(stack -> sem_id, &sem_unlock, 1);
}

int push(stack_t* stack, void* val){
	//semctl(stack->sem_id, 0, GETVAL);
	printf("In Lock Push, sem_id = %i\n", stack->sem_id);
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
	//int res = shmctl(stack->shm_stack_t_id, IPC_RMID, NULL);
	Unlock(stack);
	return 0;
}

int detach_stack(stack_t* stack){
	printf("In Lock Detach, sem_id = %i\n", stack->sem_id);
	stack -> n_process --;
	semop(stack -> sem_id, &sem_lock, 1);
	printf("stack->n_process = %d, stack -> to_destruct = %d\n", stack->n_process, stack -> to_destruct);
	if(stack->n_process == 0 && stack -> to_destruct) {
		//usleep(1e5);
		if(stack->n_process == 0 && stack -> to_destruct) {
			//pthread_mutex_destroy(&stack->mutex);
			int buf_sem_id = stack->sem_id;
			shmctl(stack->shm_stack_t_id, IPC_RMID, NULL);
			shmdt(stack);
			//Unlock(stack);
			semctl(buf_sem_id, 0, IPC_RMID, NULL);
			printf("Unlock and deleting stack\n");
			semop(buf_sem_id, &sem_unlock, 1);
		}else{
			int buf_sem_id = stack->sem_id;
			shmdt(stack);
			printf("Unlock and detaching stack\n");
			semop(buf_sem_id, &sem_unlock, 1);
		}
	} else{
		int buf_sem_id = stack->sem_id;
		shmdt(stack);
		printf("Unlock and detaching stack\n");
		semop(buf_sem_id, &sem_unlock, 1);
	}
	return 1;
}