#ifndef STACK_T_H
#define STACK_T_H

#include <sys/types.h>

typedef struct {
    int size;
    int count;
    int to_destruct; // != 0 if to destruct
    int shm_stack_t_id;
    int n_process;
    pthread_mutex_t mutex;
    void **data;
} stack_t;

/* Attach (create if needed) shared memory stack to the process.
Returns stack_t* in case of success. Returns NULL on failure. */
stack_t* attach_stack(key_t key, int size);

void PrintStack(stack_t* stack);

/* Detaches existing stack from process.
Operations on detached stack are not permitted since stack pointer becomes invalid. */
int detach_stack(stack_t* stack);

/* Marks stack to be destroed. Destruction are done after all detaches */
int mark_destruct(stack_t* stack);

/* Returns stack maximum size. */
int get_size(stack_t* stack);

/* Returns current stack size. */
int get_count(stack_t* stack);

/* Push val into stack. */
int push(stack_t* stack, void* val);

/* Pop val from stack into memory */
int pop(stack_t* stack, void** val);

//---------------------------------------------
/* Additional tasks */

/* Control timeout on push and pop operations in case stack is full or empty.
val == -1 Operations return immediatly, probably with errors.
val == 0  Operations wait infinitely.
val == 1  Operations wait timeout time.

int set_wait(int val, timespec* timeout);
*/

#endif //STACK_T_H