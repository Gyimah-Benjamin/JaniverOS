#ifndef MEMORY_H
#define MEMORY_H

#define SIZE 4096
#define MEMORY 1048576
#define PAGE_COUNT  (MEMORY/SIZE)
#define HEAP_SIZE 1024

extern char bitmap[PAGE_COUNT/8];
extern char heap[HEAP_SIZE];

extern unsigned int page_directory[1024];
extern unsigned int page_table[1024];


extern char* heap_pointer;

struct data{
int siz;
int free;
};

struct info{
int size;
int free;
};

void set_free(int page_num);
void set_taken(int page_num);
void set_page_table();
void paging();
void *my_malloc(int size);
void my_free(void *ptr);
void *j_malloc(int size);
void j_free(void *ptr);

#endif
