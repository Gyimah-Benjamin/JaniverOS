#include "memory.h"

 char bitmap[PAGE_COUNT/8] __attribute__((section(".bss")));
 char heap[HEAP_SIZE] __attribute__((section(".bss")));

 __attribute__((aligned(4096), section(".bss"))) unsigned int page_directory[1024];
 __attribute__((aligned(4096), section(".bss"))) unsigned int page_table[1024];

char* heap_pointer = heap;

void set_free(int page_num){
int byte = page_num/8;
int bit = page_num%8;
bitmap[byte] = bitmap[byte] & ~(1<<bit);

}

void set_taken(int page_num){
int byte = page_num/8;
int bit = page_num%8;
bitmap[byte] = bitmap[byte] | (1<<bit);
}


int find_free(){
for(int i = 0; i <= PAGE_COUNT; i++){
int byte = i/8;
int bit = i%8;
if(!(bitmap[byte] & (1<<bit))){
return i;
}
}
return -1;
}

void set_page_table(){
for(int i = 0; i < 1024; i++){
page_table[i] = (i * SIZE) | 3;
}

page_directory[0] = ((unsigned int)page_table) | 3;

for(int i = 1; i < 1024; i++){
page_directory[i] = 2;
}
}


void paging(){
__asm__ volatile("mov %0, %%cr3" :: "r"((unsigned int)page_directory));

unsigned int cr0;
__asm__ volatile("mov %%cr0, %0" : "=r"(cr0));

cr0 = cr0 | 0x80000000;

__asm__("mov %0, %%cr0" :: "r"(cr0));
}


void *my_malloc(int size){
struct data *position = (struct data*)heap;

while(position < (struct data*)heap_pointer){
if((position -> free) == 1 && (position -> siz) >= size){
position -> free = 0;
return (void*)(position + 1);
}
position = (struct data*)((char*)(position + 1) + position -> siz);
}


struct data *label = (struct data*)heap_pointer;
label -> siz = size;
label -> free = 0;
heap_pointer = heap_pointer + sizeof(struct data) + size;
return (void*)(label + 1);
}

void my_free(void *ptr){
struct data *label = (struct data*)ptr - 1;
label -> free = 1;
}


unsigned int mem_space = 0x2000000;
unsigned int space_end = 0x2000000;

void *j_malloc(int size){
struct info *position = (struct info*)mem_space;

while(position < (struct info*)space_end){
if((position -> free) == 1 && (position -> size) >= size){
position -> free = 0;
return (void*)(position + 1);
}
position = (struct info*)((char*)(position + 1) + position -> size);
}


struct info *new_space = (struct info*)space_end;
new_space -> size = size;
new_space -> free = 0;
space_end = space_end + sizeof(struct info) + size;
return (void*)(new_space + 1);
}

void j_free(void *ptr){
if(!ptr){
return;
}
struct info *space = (struct info*)ptr - 1;
space -> free = 1;
}
