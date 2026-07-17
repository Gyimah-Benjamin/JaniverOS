#ifndef BMP_H
#define BMP_H

struct header{
unsigned short signa;
unsigned int file_size;
unsigned int reserved;
unsigned int data_offset;
}__attribute__((packed));


struct info{
unsigned int head_size;
int width;
int height;
unsigned short planes;
unsigned short bits_per_pixel;
unsigned int compression;
unsigned int image_size;
int x_pix_per_meter;
int y_pix_per_meter;
unsigned int colors_used;
unsigned int colors_import;
}__attribute__((packed));

#endif
