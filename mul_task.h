#ifndef MUL_TASK_H
#define MUL_TASK_H

#define MAX_TASK 3
#define STACK_SIZE 4096

typedef struct{
unsigned int esp;
int active;

int stack[STACK_SIZE/4];
//int terminal_id;

}Task;

extern Task task[MAX_TASK];
extern int current_task;

void create_task(void (*function)());

unsigned int scheduler(unsigned int current_esp);

#endif
