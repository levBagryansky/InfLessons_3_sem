#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "stack_t.h"

int main() {
    printf("Hello, World!\n");
    fork();
    fork();
    fork();

    stack_t *p_st = attach_stack(2, 3);
    printf("%p %d\n", p_st, p_st->shm_stack_t_id);
    mark_destruct(p_st);
    detach_stack(p_st);

}
