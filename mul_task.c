#include "mul_task.h"

Task task[MAX_TASK];
int current_task = 0;
int task_count = 1;
int started = 0;
//int terminal_id = 0;

void create_task(void (*function)()){
if(task_count >= MAX_TASK){
return;
}

int *stack_top;

stack_top = task[task_count].stack + (STACK_SIZE/4);

*(--stack_top) = 0x202; //EFLAGS
*(--stack_top) = 0x08; //CS
*(--stack_top) = (int)function; //EIP

*(--stack_top) = 0; //EAX
*(--stack_top) = 0; //ECX
*(--stack_top) = 0; //EDX
*(--stack_top) = 0; //EBX
*(--stack_top) = 0; //ESP
*(--stack_top) = 0; //EBP
*(--stack_top) = 0; //ESI
*(--stack_top) = 0; //EDI

task[task_count].esp = (unsigned int)stack_top;
task[task_count].active = 1;
/*if(terminal_id_bool==true){
task[task_count].terminal_id = terminal_id;
terminal_id++;
task_count++;
}*/
task_count++;
}

unsigned int scheduler(unsigned int current_esp){
if(started == 0){
started = 1;
current_task = 1;

task[0].esp = current_esp;

return task[current_task].esp;
}

task[current_task].esp = current_esp;
int next_task = current_task + 1;

if(next_task >= task_count){
next_task = 0;
}

current_task = next_task;
return task[current_task].esp;

}


/*void terminal_task(){
while(1){
int terminal_id = task[current_task].terminal_id;

track_x = term_win[terminal_id].track_x;
track_y = term_win[terminal_id].track_y;
track_y1 = term_win[terminal_id].track_y1;
letter_track = term_win[terminal_id].letter_track;
ad_ten = term_win[terminal_id].ad_ten;

}
}
*/
