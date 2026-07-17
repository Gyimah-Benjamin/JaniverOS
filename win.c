#include "win.h"
#include "font.h"
//#include "bmp.h"
//#include "wallpaper.h"
void draw_desktop();
void draw_task_bar();
void logo();
bool task_active = false;
struct window_size point_maxim;
unsigned char *back_buffer = (unsigned char *)0x00500000;

void init_screen() {
    screen = (unsigned char*)(*(unsigned int*)0x7E28);
}

void fill_screen(int x, int y, unsigned int color){
    int offset = (x * 3072) + (y * 3);
    back_buffer[offset]     = color & 0xFF;
    back_buffer[offset + 1] = (color >> 8) & 0xFF;
    back_buffer[offset + 2] = (color >> 16) & 0xFF;
}

void fill_screen1(int x, int y, unsigned int color){
    unsigned char *fb = (unsigned char*)screen;
    int offset = (x * 3072) + (y * 3);
    fb[offset]     = color & 0xFF;
    fb[offset + 1] = (color >> 8) & 0xFF;
    fb[offset + 2] = (color >> 16) & 0xFF;
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
for(int i = row_st; i < row_st+row_end; i++){
int blue, green, red;

if(i < row_st + 5){
blue = 90;
green = 90;
red = 90;
}

else{
blue = 20 + (i/19);
green = 20 + (i/19);
red = 20 + (i/19);
}

if(i > ((row_st+row_end)-2)){
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

for(int i = 0; i < total; i += 8){
dest[i + 0] = src[i + 0];
dest[i + 1] = src[i + 1];
dest[i + 2] = src[i + 2];
dest[i + 3] = src[i + 3];
dest[i + 4] = src[i + 4];
dest[i + 5] = src[i + 5];
dest[i + 6] = src[i + 6];
dest[i + 7] = src[i + 7];
}
}



void draw_rect(int x, int y, int w, int h, int color){
for(int i = x; i < x + h; i++){
for(int j = y; j < y + w; j++){
fill_screen(i, j, color);
fill_screen1(i, j, color);
}
}
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
    draw_rect(track_x - 2, track_y, 1, 14, RED);
}
}

void cur_clear() {
   if(point_maxim.maxim_flag == 1){
    draw_rect(track_x - 2, track_y, 1, 14, TEXT_B);
}
}

void cur_clear_b() {
   if(point_maxim.maxim_flag == 1){
    draw_rect(track_x - 2, track_y + 10, 1, 14, TEXT_B);
}
}



void term_newline(){
ad_ten += 14;
track_x += 14;
string(67 + ad_ten, 120 + 5, "~", TASK_BG, TEXT_B);
string(65 + ad_ten, 120 + 5 + 10, "$", TASK_BG, TEXT_B);
track_y = 120 + 5 + 10 + 10;
track_y1 = 120 + 5 + 10 + 10;
letter_track = 145;
}

void term_newline_xpromt(){
ad_ten += 14;
track_x += 14;
track_y = 145;
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
fill_screen1(x + row, y + bit, color);
}
else{
fill_screen1(x + row, y + bit, color1);
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
draw_char(x, y, str[i], color, color1);
y += 10;
track_y1 += 10;
i++;
}
}

void terminal(int x, int y, int term_y, int term_x, int term_title, char *str ){
int incre = 0;
draw_rect(x, y, term_y, term_x, TEXT_B);

draw_rect(x, y, term_y, term_title, GREYY);
for(int i = 0; str[i] != 0; i++){
draw_char((term_title + x) /2 + 15, (term_y + y) /2 + incre, str[i], TEXT_W, GREYY);
incre += 10;
}
string(term_title + 37, y + 5, "~", TASK_BG, TEXT_B);
string(term_title + 35, y + 5 + 10, "$", TASK_BG, TEXT_B);
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

void terminal_close(int x, int y, int term_y, int term_x, int term_title, char *str){
grad_color(back_buffer, 728, 1024);
flush();
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
draw_rect(0, 0, SCREEN_W, SCREEN_H, 0x00000000);
flush();
int lx = SCREEN_W / 2 - 100;
int ly = SCREEN_H / 2 - 40;

char *name = "JanOS";

for(int i = 0; name[i] != 0; i++){
for(int j = 0; j < 1; j++){
draw_cursor(ly, lx, 1);
flush();
delay(52200500);
draw_cursor(ly, lx, 0);
flush();
delay(52200500);
}

draw_char_l(ly, lx + 8, name[i], 0x00444444, 0x00000000, 0);
flush();
delay(9500000);
draw_char_l(ly, lx + 4, name[i], 0x00888888, 0x00000000, 1);
flush();
delay(9500000);
draw_char_l(ly, lx, name[i], TEXT_W, 0x00000000, 2);
flush();
lx += 10 * 2;

}

for(int i = 0; i < 6; i++){
draw_cursor(ly, lx, i%2);
flush();
delay(56500000);
}

draw_cursor(ly, lx, 0);
flush();
delay(20500000);
grad_color(back_buffer, 728, 1024);
flush();
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

draw_char(745, 945, '0' + hours/10, TEXT_W, GREYY);
draw_char(745, 955, '0' + hours%10, TEXT_W, GREYY);


draw_char(745, 965, ':', TEXT_W, GREYY);

draw_char(745, 975, '0' + minutes/10, TEXT_W, GREYY);
draw_char(745, 985, '0' + minutes%10, TEXT_W, GREYY);

draw_char(745, 995, ':', TEXT_W, GREYY);

draw_char(745, 1005, '0' + seconds/10, TEXT_W, GREYY);
draw_char(745, 1015, '0' + seconds%10, TEXT_W, GREYY);
}
else{
grad_task(back_buffer, TASK_Y, 0, TASK_H, SCREEN_W);
flush();
unsigned char hours = rtc(0x04);
unsigned char minutes = rtc(0x02);
unsigned char seconds = rtc(0x00);

hours = ((hours >> 4)*10 + (hours & 0x0F));
minutes = ((minutes >> 4)*10 + (minutes & 0x0F));
seconds = ((seconds >> 4)*10 + (seconds & 0x0F));

draw_char(745, 945, '0' + hours/10, TEXT_W, GREYY);
draw_char(745, 955, '0' + hours%10, TEXT_W, GREYY);


draw_char(745, 965, ':', TEXT_W, GREYY);

draw_char(745, 975, '0' + minutes/10, TEXT_W, GREYY);
draw_char(745, 985, '0' + minutes%10, TEXT_W, GREYY);

draw_char(745, 995, ':', TEXT_W, GREYY);

draw_char(745, 1005, '0' + seconds/10, TEXT_W, GREYY);
draw_char(745, 1015, '0' + seconds%10, TEXT_W, GREYY);

task_active = true;
}
}
