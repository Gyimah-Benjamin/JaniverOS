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
Terminal *active_terminal = 0;
int slot_counter = 0;

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
int x = 550, x_start = 0, y = 750, y_start = 0, title = 30;
char *ptr = "Jan Terminal";
int j = 0;
int id = 0;

    for(int i = 0; i < 5; i++){
    term_win[i].check = false;
    term_win[i].window.height = x;
    term_win[i].window.x = x_start;
    term_win[i].window.width = y;
    term_win[i].window.y = y_start;
    term_win[i].window.title = title;
    term_win[i].window.title_gap = x_start;
    term_win[i].window.total_bytes = y * x * 3;
    term_win[i].position = 0;
    term_win[i].input_buffer[0] = '\0';
    term_win[i].browse = 0;
    term_win[i].arrow_count = 0;
    term_win[i].window.id = id;
while(j < 13){
term_win[i].window.title_buffer[j] = ptr[j];
j++;
}
term_win[i].window.title_buffer[j] = '\0';
j = 0;
id++;
//x_start = x_start+70, y_start = y_start+50, title = 30;
    }
}

void init_term_N(Terminal *term){
int j = 0;
int m = 0;

save_term_state(active_terminal);

//track_x = term->window.x + 33;
//track_y = term->window.y + 25;
//track_y1 = term->window.y + 25;
//letter_track = term->window.y + 25;

term->window.buffer = (unsigned char*)j_malloc(term->window.total_bytes);
while(m+2 < term->window.total_bytes){
term->window.buffer[m++] = 40;
term->window.buffer[m++] = 40;
term->window.buffer[m++] = 40;
}
m = 0;
term->check = true;
active_terminal = term;
is_typing = true;
//load_term_state(active_terminal);
terminal(term);

}

void init_term_N_focused(Terminal *term){
save_term_state(active_terminal);


active_terminal = term;

load_term_state(active_terminal);

draw_win(active_terminal->window.total_bytes, (((active_terminal->window.x+30)+(active_terminal->window.id*70))*3),
(((active_terminal->window.y+50)+(active_terminal->window.id*40))*3), active_terminal->window.width*3);
load_term_state(active_terminal);

slot_counter++;
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

if(active_terminal != 0){
    active_terminal->window.buffer[offset]     = color & 0xFF;
    active_terminal->window.buffer[offset + 1] = (color >> 8) & 0xFF;
    active_terminal->window.buffer[offset + 2] = (color >> 16) & 0xFF;
}


else{
    fb[offset_1]     = color & 0xFF;
    fb[offset_1 + 1] = (color >> 8) & 0xFF;
    fb[offset_1 + 2] = (color >> 16) & 0xFF;
}

}

void update(int *x, int *y){
*x = 30 + (active_terminal->window.id*70);
*y = 50 + (active_terminal->window.id*50);

/*track_x = *x+35;
track_y = *y+25;
track_y1 = *y+25;
letter_track = *y+25;
ad_ten = 0;
*/
}

void draw_win(int total, int x, int y, int w){
unsigned char *fb = (unsigned char*)screen;

//update(&x, &y);

//x = x*3;
//y = y*3;

int j = x;
int m = 0;
int n = 0;
int offset = (j * 1024) + y;

while(m < total){
fb[offset] = active_terminal->window.buffer[m];
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
unsigned char *fb = (unsigned char*)screen;
int offset = (x * (3*y_max)) + (y * 3);

x = (x+30)+(active_terminal->window.id*70);
y = (y+50)+(active_terminal->window.id*40);
int main_offset = (x*3072) + (y*3);

for(int row = 0; row < 8; row++){
for(int col = 0; col < 24; col++){

fb[main_offset] = active_terminal->window.buffer[offset+col+ ((y_max*3)*row)];
main_offset++;

if(col == 23){
x++;
main_offset = (x*3072) + (y*3);
}


}
}
}

int window_slots(){
int slot_taken;

for(slot_taken = 0; slot_taken < 5; slot_taken++){
if(term_win[slot_taken].check == false){
term_win[slot_taken].check = true;
return slot_taken;
}
else if(term_win[slot_taken].check == true && slot_taken == 2){
return -1;
}

}

}

int window_slots_used(){

for(slot_counter = slot_counter; slot_counter < 5; slot_counter++){
 if(term_win[slot_counter].check == true){
return slot_counter;
}

}

}

void mini_draw_rect(int x, int y, int x_term, int y_term, int y_max){
unsigned char *fb = (unsigned char*)screen;
int offset = (x * (3*y_max)) + (y * 3);

x = (x+30)+(active_terminal->window.id*70);
y = (y+50)+(active_terminal->window.id*40);

int main_offset = (x*3072) + (y*3);

for(int row = 0; row < x_term; row++){
for(int col = 0; col < (y_term*3); col++){

fb[main_offset] = active_terminal->window.buffer[(offset+((y_max*3)*row)) + col];
main_offset++;

}
x++;
main_offset = (x*3072) + (y*3);

}
}

void fill_screen1(int x, int y, unsigned int  color){
    unsigned char *fb = (unsigned char*)screen;
    int offset = (x * 3072) + (y * 3);
    fb[offset]     = color & 0xFF;
    fb[offset + 1] = (color >> 8) & 0xFF;
    fb[offset + 2] = (color >> 16) & 0xFF;
}


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
if(active_terminal != 0){
ultim_fill(i, j, w, color);
}


else{
ultim_fill(x+i, y+j, 0, color);
}

}
}

}


void draw_rect_1(int x, int y, int width, int height, int color){
for(int i = x; i < x+height; i++){
for(int j = y; j < y+width; j++){
if(active_terminal != 0){
ultim_fill(i, j, active_terminal->window.width, color);
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
mini_draw_rect(track_x - 2, track_y, 14, 1, active_terminal->window.width);

}
}

void cur_clear() {
   if(point_maxim.maxim_flag == 1){
    draw_rect_1(track_x - 2, track_y, 1, 14, TEXT_B);
mini_draw_rect(track_x - 2, track_y, 14, 1, active_terminal->window.width);

}
}

void cur_clear_b() {
   if(point_maxim.maxim_flag == 1){
    draw_rect_1(track_x - 2, track_y + 10, 1, 14, TEXT_B);
    mini_draw_rect(track_x - 2, track_y + 10, 14, 1, active_terminal->window.width);

}
}



void term_newline(){
ad_ten += 14;
track_x += 14;
string(active_terminal->window.x+35 + ad_ten, active_terminal->window.y + 5, "~", TASK_BG, TEXT_B);
string(active_terminal->window.x+33 + ad_ten, active_terminal->window.y + 15, "$", TASK_BG, TEXT_B);
track_y = active_terminal->window.y + 25;
track_y1 = active_terminal->window.y + 25;
letter_track = active_terminal->window.y + 25;

}

void term_newline_xpromt(){
ad_ten += 14;
track_x += 14;
track_y = active_terminal->window.y+25;
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
if(active_terminal != 0){
ultim_fill(x + row, y + bit, active_terminal->window.width, color);
}


else{
ultim_fill(x + row, y + bit, 0, color);
}

}

else{
if(active_terminal != 0){
ultim_fill(x + row, y + bit, active_terminal->window.width, color1);
}

else{
ultim_fill(x + row, y + bit, 0, color1);
}

}
}
}
}


void draw_char_1(int x, int y, char al, int color, int color1){
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
term_input(x, y, str[i], color, color1);
y += 10;
track_y1 += 10;
i++;
}
}

void term_input(int x, int y, char al, int color, int color1){
if(active_terminal != 0){

draw_char(x, y, al, color, color1);

mini_draw_win(x, y, active_terminal->window.width);
}


else{
draw_char(x, y, al, color, color1);
mini_draw_win(x, y, 1024);
}
}


void gen_window(Window *win){
int incre = 0;


draw_rect(win->x, win->y, win->width, win->height, TEXT_B);
draw_rect(win->x, win->y, win->width, win->title, GREYY);

for(int i = 0; win->title_buffer[i] != 0; i++){
draw_char((win->title/2)-5, ((win->width/2)-30) + incre, win->title_buffer[i], TEXT_W, GREYY);
incre += 10;
}

draw_win((win->total_bytes), (((win->x+30)+(win->id*70))*3), (((win->y+50)+(win->id*40))*3), (win->width*3));


}

void terminal(Terminal *term){
Window *win = &term->window;

gen_window(win);

string(win->title_gap + 35, win->y + 5, "~", TASK_BG, TEXT_B);
string(win->title_gap + 33, win->y + 5 + 10, "$", TASK_BG, TEXT_B);

icon(737, 110);
}

void save_term_state(Terminal *term){
term->track_x = track_x;
term->track_y = track_y;
term->track_y1 = track_y1;
term->letter_track = letter_track;
term->ad_ten = ad_ten;
}

void load_term_state(Terminal *term){
track_x = term->track_x;
track_y = term->track_y;
track_y1 = term->track_y1;
letter_track = term->letter_track;
ad_ten = term->ad_ten;
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

draw_char_1(745, 945, '0' + hours/10, TEXT_W, GREYY);
draw_char_1(745, 955, '0' + hours%10, TEXT_W, GREYY);


draw_char_1(745, 965, ':', TEXT_W, GREYY);

draw_char_1(745, 975, '0' + minutes/10, TEXT_W, GREYY);
draw_char_1(745, 985, '0' + minutes%10, TEXT_W, GREYY);

draw_char_1(745, 995, ':', TEXT_W, GREYY);

draw_char_1(745, 1005, '0' + seconds/10, TEXT_W, GREYY);
draw_char_1(745, 1015, '0' + seconds%10, TEXT_W, GREYY);

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

draw_char_1(745, 945, '0' + hours/10, TEXT_W, GREYY);
draw_char_1(745, 955, '0' + hours%10, TEXT_W, GREYY);


draw_char_1(745, 965, ':', TEXT_W, GREYY);

draw_char_1(745, 975, '0' + minutes/10, TEXT_W, GREYY);
draw_char_1(745, 985, '0' + minutes%10, TEXT_W, GREYY);

draw_char_1(745, 995, ':', TEXT_W, GREYY);

draw_char_1(745, 1005, '0' + seconds/10, TEXT_W, GREYY);
draw_char_1(745, 1015, '0' + seconds%10, TEXT_W, GREYY);

task_active = true;
}
}
