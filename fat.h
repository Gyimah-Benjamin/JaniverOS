#ifndef FAT12_H
#define FAT12_H

#define SECTOR_SIZE 512
#define ROOT_DIR_START 192
#define ROOT_DIR_SIZE 32
#define FAT_START 64
#define FAT_SIZE 64
#define CLUSTER_SIZE 4
#define DATA_START 224
void cat(char *filename);

struct Dir{
unsigned char name[8];
unsigned char ext[3];
unsigned char attributes;
unsigned char reserved[10];
unsigned short time;
unsigned short data;
unsigned short first_cluster;
unsigned int file_size;
}__attribute__((packed));

#endif
