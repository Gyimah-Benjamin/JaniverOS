#include "win.h"
#include "font.h"
#include "memory.h"
//#include "bmp.h"
//#include "wallpaper.h"
void draw_desktop();
void draw_task_bar();
void logo();
bool task_active = false;
Terminal term_win[5];

struct window_size point_maxim;
unsigned char *back_buffer = (unsigned char *)0x00500000;

unsigned int simple_logo[576] = {
    0x000000,0x000000,0x000000,0x000000,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x000000,0x000000,0x000000,0x000000,
    0x000000,0x000000,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x000000,0x000000,
    0x000000,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x000000,
    0x000000,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x000000,
    0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x00F0FF,0x00F0FF,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x121214,0x00F0FF,0x00F0FF,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x121214,0x121214,0x00F0FF,0x00F0FF,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x00F0FF,0x00F0FF,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x00F0FF,0x00F0FF,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x121214,0x121214,0x00F0FF,0x00F0FF,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x121214,0x00F0FF,0x00F0FF,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x00F0FF,0x00F0FF,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0xFF007F,0xFF007F,0xFF007F,0xFF007F,0xFF007F,0xFF007F,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,
    0x000000,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x000000,
    0x000000,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x000000,
    0x000000,0x000000,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x000000,0x000000,
    0x000000,0x000000,0x000000,0x000000,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x121214,0x000000,0x000000,0x000000,0x000000
};

void init_screen() {
    screen = (unsigned char*)(*(unsigned int*)0x7E28);
    for(int i = 0; i < 5; i++){
    term_win[i].check = false;
    }
}

void init_term_N(int x_start, int y_start, int y, int x, int title, char *ptr){
int j = 0;
int m = 0;

if(term_win[0].check == false){
term_win[0].term_x = x;
term_win[0].term_x_start = x_start;
term_win[0].term_y = y;
term_win[0].term_y_start = y_start;
term_win[0].term_title = title;
while(j < 13){
term_win[0].title_buf[j] = ptr[j];
j++;
}
term_win[0].title_buf[j] = '\0';
j = 0;
term_win[0].total_bytes = y * x * 3;
term_win[0].buffer = (unsigned char*)j_malloc(term_win[0].total_bytes);
while(m+2 < term_win[0].total_bytes){
term_win[0].buffer[m++] = 40;
term_win[0].buffer[m++] = 40;
term_win[0].buffer[m++] = 40;
}
m = 0;
term_win[0].check = true;
is_typing = true;
terminal(term_win[0].term_x_start, term_win[0].term_y_start, term_win[0].term_y, term_win[0].term_x, term_win[0].term_title, 
term_win[0].title_buf);

}


else if(term_win[0].check == true && term_win[1].check == false){
term_win[0].track_x = track_x;
term_win[0].track_y = track_y;
term_win[0].track_y1 = track_y1;

track_x = x_start + 33;
track_y = y_start + 25;
track_y1 = y_start + 25;
letter_track = y_start + 25;

term_win[1].term_x = x;
term_win[1].term_x_start = x_start;
term_win[1].term_y = y;
term_win[1].term_y_start = y_start;
term_win[1].term_title = title;
while(j < 13){
term_win[1].title_buf[j] = ptr[j];
j++;
}
term_win[1].title_buf[j] = '\0';
j = 0;

term_win[1].total_bytes = y * x * 3;
term_win[1].buffer = (unsigned char*)j_malloc(term_win[1].total_bytes);
while(m+2 < term_win[1].total_bytes){
term_win[1].buffer[m++] = 40;
term_win[1].buffer[m++] = 40;
term_win[1].buffer[m++] = 40;
}
m = 0;
term_win[1].check = true;
is_typing = true;
terminal(term_win[1].term_x_start, term_win[1].term_y_start, term_win[1].term_y, term_win[1].term_x, term_win[1].term_title, term_win[1].title_buf);
}

}

void fill_screen(int x, int y, unsigned int color){
    int offset = (x * 3072) + (y * 3);
    back_buffer[offset]     = color & 0xFF;
    back_buffer[offset + 1] = (color >> 8) & 0xFF;
    back_buffer[offset + 2] = (color >> 16) & 0xFF;
}

void ultim_fill(int x, int y, int y_max, int color){
int offset = (x * (3*y_max)) + (y * 3);

unsigned char *fb = (unsigned char*)screen;
int offset_1 = (x * 3072) + (y * 3);

if(term_win[0].check == true && term_win[1].check == false && is_typing == true){
    term_win[0].buffer[offset]     = color & 0xFF;
    term_win[0].buffer[offset + 1] = (color >> 8) & 0xFF;
    term_win[0].buffer[offset + 2] = (color >> 16) & 0xFF;
}

else if(term_win[0].check == true && term_win[1].check == true && is_typing == true){
    term_win[1].buffer[offset]     = color & 0xFF;
    term_win[1].buffer[offset + 1] = (color >> 8) & 0xFF;
    term_win[1].buffer[offset + 2] = (color >> 16) & 0xFF;
}

else{
    fb[offset_1]     = color & 0xFF;
    fb[offset_1 + 1] = (color >> 8) & 0xFF;
    fb[offset_1 + 2] = (color >> 16) & 0xFF;
}

}

void draw_win(int total, int x, int y, int w){
unsigned char *fb = (unsigned char*)screen;
int j = x;
int m = 0;
int n = 0;
int offset = (j * 1024) + y;

while(m < total){
if(term_win[0].check == true && term_win[1].check == false){
fb[offset] = term_win[0].buffer[m];
}
else if(term_win[0].check == true && term_win[1].check == true){
fb[offset] = term_win[1].buffer[m];
}
m++;
n++;
offset++;

if(n >= w){
j += 3;
n = 0;
offset = (j * 1024) + y;
}

}

}

void mini_draw_win(int x, int y, int y_max){
if(term_win[0].check == true && is_typing == true){
unsigned char *fb = (unsigned char*)screen;
int offset = (x * (3*y_max)) + (y * 3);
int main_offset = (x*3072) + (y*3);

for(int row = 0; row < 8; row++){
for(int col = 0; col < 24; col++){

if(term_win[0].check == true && term_win[1].check == false){
fb[main_offset] = term_win[0].buffer[offset+col+ ((y_max*3)*row)];
}

else if(term_win[0].check == true && term_win[1].check == true){
fb[main_offset] = term_win[1].buffer[offset+col+ ((y_max*3)*row)];
}
main_offset++;

if(col == 23){
x++;
main_offset = (x*3072) + (y*3);
}

}
}
}
}

void mini_draw_rect(int x, int y, int x_term, int y_term, int y_max){
unsigned char *fb = (unsigned char*)screen;
int offset = (x * (3*y_max)) + (y * 3);
int main_offset = (x*3072) + (y*3);

for(int row = 0; row < x_term; row++){
for(int col = 0; col < (y_term*3); col++){

if(term_win[0].check == true && term_win[1].check == false){
fb[main_offset] = term_win[0].buffer[(offset+((y_max*3)*row)) + col];
}

else if(term_win[0].check == true && term_win[1].check == true){
fb[main_offset] = term_win[1].buffer[(offset+((y_max*3)*row)) + col];
}
main_offset++;

}
x++;
main_offset = (x*3072) + (y*3);

}
}

/*void fill_screen1(int x, int y, unsigned int  color){
    unsigned char *fb = (unsigned char*)screen;
    int offset = (x * 3072) + (y * 3);
    fb[offset]     = color & 0xFF;
    fb[offset + 1] = (color >> 8) & 0xFF;
    fb[offset + 2] = (color >> 16) & 0xFF;
}
*/

/*void draw_log1(int x, int y, unsigned int color){
    int offset = (x * 3072) + (y * 3);
    back_buffer[offset]     = color & 0xFF;
    back_buffer[offset + 1] = (color >> 8) & 0xFF;
    back_buffer[offset + 2] = (color >> 16) & 0xFF;
}
*/
void draw_log(int x, int y, unsigned int  color){
    unsigned char *fb = (unsigned char*)screen;
    int offset = (x * 3072) + (y * 3);
    fb[offset]     = color & 0xFF;
    fb[offset + 1] = (color >> 8) & 0xFF;
    fb[offset + 2] = (color >> 16) & 0xFF;
}

void icon(int xth, int yth){
int index = 0;

for(int y = 0; y < 24; y++){
for(int x = 0; x < 24; x++){

int color = simple_logo[index];
index += 1;
if(color == 0x000000){
continue;
}
draw_log((xth+y), (yth+x), color);
//draw_log1((xth+y), (yth+x), color);
}
}
}

void grad_color(char *back_buffer, int row, int col){
for(int i = 0; i < row; i++){
int blue = 30 - (i*10)/row;
int green = (i*33)/row;
int red = (i*49)/row;


for(int j = 0; j < col; j++){
int pix = (i*col+j)*3;
back_buffer[pix + 0] = blue;
back_buffer[pix + 1] = green;
back_buffer[pix + 2] = red;
}
}
}

void grad_task(char *back_buffer, int row_st, int col_st, int row_end, int col_end){
unsigned char *fb = (unsigned char*)screen;
for(int i = row_st; i < row_st+row_end; i++){
int blue, green, red;

if(i < row_st + 2){
blue = 90;
green = 90;
red = 90;
}

else{
blue = 20 + (i/19);
green = 20 + (i/19);
red = 20 + (i/19);
}

if(i > ((row_st+row_end)-1)){
blue = 90;
green = 90;
red = 90;
}

for(int j = col_st; j < col_end; j++){
int pix = (i*col_end+j)*3;
back_buffer[pix + 0] = blue;
back_buffer[pix + 1] = green;
back_buffer[pix + 2] = red;
}
}
}

unsigned char rtc(unsigned char reg){
__asm__("out %%al, %%dx" :: "a"(reg), "d"(0x70));

unsigned char val;
__asm__("in %%dx, %%al" : "=a"(val) : "d"(0x71));

return val;
}


void flush(){
unsigned char *fb = (unsigned char*)screen;
unsigned long long *src = (unsigned long long *)back_buffer;
unsigned long long *dest = (unsigned long long *)fb;

long long total = (1024*768*3)/8;

for(int i = 0; i < total; i++){
dest[i] = src[i];
}
}


void draw_rect(int x, int y, int w, int h, int color){
for(int i = 0; i < h; i++){
for(int j = 0; j < w; j++){
if(term_win[0].check == true && term_win[1].check == false){
ultim_fill(i, j, w, color);
}

else if(term_win[0].check == true && term_win[1].check == true){
ultim_fill(i, j, w, color);
}

else{
ultim_fill(x+i, y+j, 0, color);
}

}
}
//if(term_win[0].check == true){
//draw_win((w*h*3), x*3, y*3, w*3);
//}

}


void draw_rect_1(int x, int y, int w, int h, int color){
for(int i = x; i < x+h; i++){
for(int j = y; j < y+w; j++){
if(term_win[0].check == true && term_win[1].check == false && is_typing == true){
ultim_fill(i, j, term_win[0].term_y, color);
}

else if(term_win[0].check == true && term_win[1].check == true && is_typing == true){
ultim_fill(i, j, term_win[1].term_y, color);
}

else{
ultim_fill(i, j, 0, color);
}

}
}
//if(term_win[0].check == true){
//draw_win((w*h*3), x*3, y*3, w*3);
//}

}

void clear(){
if(point_maxim.maxim_flag == 1){
draw_rect(60, 120, TERM_Y, TERM_X - 30, TEXT_B);
ad_ten = -14;
track_x = 50;
track_y = 120 + 5 + 10 + 10;
track_y1 = 120 + 5 + 10 + 10;
letter_track = 145;
}
}

void cur() {
   if(point_maxim.maxim_flag == 1){
    draw_rect_1(track_x - 2, track_y, 1, 14, RED);
  if(term_win[0].check == true && term_win[1].check == false){
    mini_draw_rect(track_x - 2, track_y, 14, 1, term_win[0].term_y);
  }

  else if(term_win[0].check == true && term_win[1].check == true){
mini_draw_rect(track_x - 2, track_y, 14, 1, term_win[1].term_y);
}

}
}

void cur_clear() {
   if(point_maxim.maxim_flag == 1){
    draw_rect_1(track_x - 2, track_y, 1, 14, TEXT_B);
if(term_win[0].check == true && term_win[1].check == false){
    mini_draw_rect(track_x - 2, track_y, 14, 1, term_win[0].term_y);
}
else if(term_win[0].check == true && term_win[1].check == true){
mini_draw_rect(track_x - 2, track_y, 14, 1, term_win[1].term_y);
}

}
}

void cur_clear_b() {
   if(point_maxim.maxim_flag == 1){
    draw_rect_1(track_x - 2, track_y + 10, 1, 14, TEXT_B);
if(term_win[0].check == true && term_win[1].check == false){
    mini_draw_rect(track_x - 2, track_y + 10, 14, 1, term_win[0].term_y);
}
else if(term_win[0].check == true && term_win[1].check == true){
mini_draw_rect(track_x - 2, track_y + 10, 14, 1, term_win[1].term_y);
}

}
}



void term_newline(){
if(term_win[0].check == true && term_win[1].check == false){
ad_ten += 14;
track_x += 14;
string(67 + ad_ten, 120 + 5, "~", TASK_BG, TEXT_B);
string(65 + ad_ten, 120 + 5 + 10, "$", TASK_BG, TEXT_B);
track_y = 120 + 5 + 10 + 10;
track_y1 = 120 + 5 + 10 + 10;
letter_track = 145;
}
else if(term_win[0].check == true && term_win[1].check == true){
ad_ten += 14;
track_x += 14;
string(term_win[1].term_x_start+37 + ad_ten, term_win[1].term_y_start + 5, "~", TASK_BG, TEXT_B);
string(term_win[1].term_x_start+35 + ad_ten, term_win[1].term_y_start + 15, "$", TASK_BG, TEXT_B);
track_y = term_win[1].term_y_start + 25;
track_y1 = term_win[1].term_y_start + 25;
letter_track = term_win[1].term_y_start + 25;
}

}

void term_newline_xpromt(){
ad_ten += 14;
track_x += 14;
if(term_win[0].check == true && term_win[1].check == false){
track_y = 145;
}
else if(term_win[0].check == true && term_win[1].check == true){
track_y = term_win[1].term_y_start+25;
}
track_y1 += 10;
}

void term_newline_max(){
ad_ten += 14;
track_x += 14;
string(37 + ad_ten, 5, "~", TASK_BG, TEXT_B);
string(35 + ad_ten, 5 + 10, "$", TASK_BG, TEXT_B);
track_y = 5 + 10 + 10;
track_y1 = 5 + 10 + 10;
letter_track = 25;
}

void draw_char(int x, int y, char al, int color, int color1){
unsigned char* conect = (unsigned char*)font[al];
for(int row = 0; row < 8; row++){
unsigned char conect1 = conect[row];
for(int bit = 0; bit < 8; bit++){
if((0x80 >> bit) & conect1){
if(term_win[0].check == true && term_win[1].check == false && is_typing == true){
ultim_fill(x + row, y + bit, term_win[0].term_y, color);
}

else if(term_win[0].check == true && term_win[1].check == true && is_typing == true){
ultim_fill(x + row, y + bit, term_win[1].term_y, color);
}

else{
ultim_fill(x + row, y + bit, 0, color);
}

}

else{
if(term_win[0].check == true && term_win[1].check == false && is_typing == true){
ultim_fill(x + row, y + bit, term_win[0].term_y, color1);
}

else if(term_win[0].check == true && term_win[1].check == true && is_typing == true){
ultim_fill(x + row, y + bit, term_win[1].term_y, color1);
}

else{
ultim_fill(x + row, y + bit, 0, color1);
}

}
}
}
}

void draw_char_l(int x, int y, char al, int color, int color1, int scale){
unsigned char* conect = (unsigned char*)font[al];
for(int row = 0; row < 8; row++){
unsigned char conect1 = conect[row];
for(int bit = 0; bit < 8; bit++){
if((0x80 >> bit) & conect1){
draw_rect(x + row * scale, y + bit * scale, scale, scale, color);
}

else{
draw_rect(x + row * scale, y + bit * scale, scale, scale, color1);
}
}
}
}

void string_l(int x, int y, char *str, int color, int color1, int scale){
int i = 0;
while(str[i] != 0){
draw_char_l(x, y, str[i], color, color1, scale);
y += 10 * scale;
i++;
}
}

void string(int x, int y, char *str, int color, int color1){
int i = 0;
while(str[i] != 0){
term_input(x, y, str[i], color, color1);
y += 10;
track_y1 += 10;
i++;
}
}

void term_input(int x, int y, char al, int color, int color1){
if(term_win[0].check == true && term_win[1].check == false && is_typing == true){
draw_char(x, y, al, color, color1);
mini_draw_win(x, y, term_win[0].term_y);
}

else if(term_win[0].check == true && term_win[1].check == true && is_typing == true){
draw_char(x, y, al, color, color1);
mini_draw_win(x, y, term_win[1].term_y);
}

else{
draw_char(x, y, al, color, color1);
mini_draw_win(x, y, 1024);
}
}


void terminal(int x, int y, int term_y, int term_x, int term_title, char *str ){
int incre = 0;
int screen_new = x + term_title;
int black_term = term_x - term_title;

draw_rect(x, y, term_y, term_x, TEXT_B);
draw_rect(x, y, term_y, term_title, GREYY);

for(int i = 0; str[i] != 0; i++){
draw_char((term_title/2)-5, ((term_y/2)-30) + incre, str[i], TEXT_W, GREYY);
incre += 10;
}

if(term_win[0].check == true && term_win[1].check == false){
draw_win((term_win[0].total_bytes), (term_win[0].term_x_start*3), (term_win[0].term_y_start*3), (term_win[0].term_y*3));
}

else if(term_win[0].check == true && term_win[1].check == true){
draw_win((term_win[1].total_bytes), (term_win[1].term_x_start*3), (term_win[1].term_y_start*3), (term_win[1].term_y*3));
}

if(term_win[0].check == true && term_win[1].check == false){
string(term_title + 35, y + 5, "~", TASK_BG, TEXT_B);
string(term_title + 33, y + 5 + 10, "$", TASK_BG, TEXT_B);
}

else if(term_win[0].check == true && term_win[1].check == true){
string(x + 35, y + 5, "~", TASK_BG, TEXT_B);
string(x + 33, y + 5 + 10, "$", TASK_BG, TEXT_B);
}
icon(737, 110);
}


void terminal_max(int x, int y, int term_y, int term_x, int term_title, char *str ){
int incre = 0;
draw_rect(x, y, term_y, term_x, TEXT_B);

draw_rect(x, y, term_y, term_title, GREYY);
for(int i = 0; str[i] != 0; i++){
draw_char((term_title - 30) + 13, (term_y / 2 - 45) + incre, str[i], TEXT_W, GREYY);
incre += 10;
}
for(int i = 0; i < 728; i++){
for(int x = 0; x < 1024; x++){
for(int y = 0; y < 3; y++){
}
}
}
}

void terminal_close(){
/*if(term_win[0].check == true && term_win[1].check == false){
grad_color(back_buffer, 728, 1024);
flush();
}
else if(term_win[0].check == true && term_win[1].check == true){
grad_color(back_buffer, 728, 1024);
flush();
term_win[1].check = false;
draw_win(term_win[0].total_bytes, term_win[0].term_x_start, term_win[0].term_y_start, term_win[0].term_y);
}*/
}


void delay(unsigned int time){
for(int i = 0; i < time; i++);
}

void draw_cursor(int x, int y, int visible){
if(visible){
draw_rect(x-7, y, 2, 27, RED);
}

else{
draw_rect(x-7, y, 2, 27, TEXT_B);
}

}

void boot_anime(){
for(int i = 0; i < 5; i++){
    term_win[i].check = false;
    }
for(int j = 0; j < 1024*768*3; j++){
back_buffer[j] = 0x00000000;
}
//draw_rect(0, 0, SCREEN_W, SCREEN_H, 0x00000000);
int lx = SCREEN_W / 2 - 120;
int ly = SCREEN_H / 2 - 40;

char *name = "JaniverOS";

for(int i = 0; name[i] != 0; i++){
for(int j = 0; j < 1; j++){
draw_cursor(ly, lx, 1);

delay(92200500);
draw_cursor(ly, lx, 0);

delay(92200500);
}

draw_char_l(ly, lx + 8, name[i], 0x00444444, 0x00000000, 0);

delay(29500000);
draw_char_l(ly, lx + 4, name[i], 0x00888888, 0x00000000, 1);

delay(11000000);
draw_char_l(ly, lx, name[i], TEXT_W, 0x00000000, 2);
delay(110500000);
lx += 10 * 2;

}

for(int i = 0; i < 6; i++){
draw_cursor(ly, lx, i%2);

delay(110500000);
}

draw_cursor(ly, lx, 0);
flush();
grad_color(back_buffer, 728, 1024);
draw_task_bar();

}

/*void logo(){
int lx = SCREEN_W / 2 - 100;
int ly = SCREEN_H / 2 - 40;

string_l(ly, lx, "JanOS",TEXT_W, DESK_BG, 2);
}
*/

void draw_task_bar(){
if(task_active == true){
unsigned char hours = rtc(0x04);
unsigned char minutes = rtc(0x02);
unsigned char seconds = rtc(0x00);

hours = ((hours >> 4)*10 + (hours & 0x0F));
minutes = ((minutes >> 4)*10 + (minutes & 0x0F));
seconds = ((seconds >> 4)*10 + (seconds & 0x0F));

term_input(745, 945, '0' + hours/10, TEXT_W, GREYY);
term_input(745, 955, '0' + hours%10, TEXT_W, GREYY);


term_input(745, 965, ':', TEXT_W, GREYY);

term_input(745, 975, '0' + minutes/10, TEXT_W, GREYY);
term_input(745, 985, '0' + minutes%10, TEXT_W, GREYY);

term_input(745, 995, ':', TEXT_W, GREYY);

term_input(745, 1005, '0' + seconds/10, TEXT_W, GREYY);
term_input(745, 1015, '0' + seconds%10, TEXT_W, GREYY);

}
else{
grad_task(back_buffer, TASK_Y, 0, TASK_H, SCREEN_W);
flush();
icon(90, 40);
unsigned char hours = rtc(0x04);
unsigned char minutes = rtc(0x02);
unsigned char seconds = rtc(0x00);

hours = ((hours >> 4)*10 + (hours & 0x0F));
minutes = ((minutes >> 4)*10 + (minutes & 0x0F));
seconds = ((seconds >> 4)*10 + (seconds & 0x0F));

term_input(745, 945, '0' + hours/10, TEXT_W, GREYY);
term_input(745, 955, '0' + hours%10, TEXT_W, GREYY);


term_input(745, 965, ':', TEXT_W, GREYY);

term_input(745, 975, '0' + minutes/10, TEXT_W, GREYY);
term_input(745, 985, '0' + minutes%10, TEXT_W, GREYY);

term_input(745, 995, ':', TEXT_W, GREYY);

term_input(745, 1005, '0' + seconds/10, TEXT_W, GREYY);
term_input(745, 1015, '0' + seconds%10, TEXT_W, GREYY);

task_active = true;
}
}
