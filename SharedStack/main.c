#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "stack_t.h"

int main() {
    printf("Hello, World!\n");
    int forked = fork();
    printf("forked = %d\n", forked);
    if(forked){
        stack_t *p_st1 = attach_stack(2, 10);
        //printf("%p %d\n", p_st, p_st->shm_stack_t_id);
        PrintStack(p_st1);
        //mark_destruct(p_st);
        detach_stack(p_st1);
    } else{
        printf("in child\n");
        stack_t *p_st2 = attach_stack(2, 10);
        for (int i = 0; i < 7; ++i) {
            int x = 123;
            push(p_st2, &x);
            printf("pushed &x\n");
        }
        mark_destruct(p_st2);
        detach_stack(p_st2);
    }
}
