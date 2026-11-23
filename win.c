#include "win.h"
#include "font.h"
#include "memory.h"
#include "mouse.h"
#include "janiver_logo.h"

//#include "bmp.h"
//#include "wallpaper.h"

unsigned char mouse_saved[12][6][3];
int old_mouse_x = 0;
int old_mouse_y = 0;
int mouse_cursor_drawn = 0;

void draw_desktop();
void draw_task_bar();
void logo();
void redraw_windows();
void present_frame();

bool task_active = false;
Terminal term_win[5];
Terminal *active_terminal = 0;
int slot_counter = 0;
int total_windows = 0;
void hide_mouse_cursor(void);
unsigned char *render_buffer = 0;
void redraw_windows_z_order(Terminal *active_terminal);
void draw_janiver_text(int top, int left, unsigned int color);
void draw_janiver_wave(int top, int left, int progress, int phase, unsigned int color);
void draw_loading_text(int top, int left, unsigned int color);
void draw_spinner(int cx, int cy, int step);
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
    render_buffer = (unsigned char*)j_malloc(1024*768*3);
int x = 550, x_start = 0, y = 750, y_start = 0, title = 30;
char *ptr = "Jan Terminal";
int j = 0;
int id = 0;

    for(int i = 0; i < 5; i++){
    term_win[i].check = false;
    term_win[i].window.height = x;
    term_win[i].window.x = x_start;
    term_win[i].window.width = y;
    term_win[i].window.buffer_width = y;
    term_win[i].window.buffer_height = x;
    term_win[i].window.y = y_start;
    term_win[i].window.title = title;
    term_win[i].window.title_gap = x_start;
    term_win[i].window.total_bytes = y * x * 3;
    term_win[i].position = 0;
    term_win[i].input_buffer[0] = '\0';
    term_win[i].browse = 0;
    term_win[i].arrow_count = 0;
    term_win[i].window.id = id;

    term_win[i].window.restore_x = term_win[i].window.x;
    term_win[i].window.restore_y = term_win[i].window.y;
    term_win[i].window.restore_width = term_win[i].window.width;
    term_win[i].window.restore_height = term_win[i].window.height;

    term_win[i].window.visible = false;
    term_win[i].window.focused = false;
    term_win[i].window.minimized = false;
    term_win[i].window.maximized = false;
while(j < 13){
term_win[i].window.title_buffer[j] = ptr[j];
j++;
}
term_win[i].window.title_buffer[j] = '\0';
j = 0;
id++;
    }
}

void copy_desktop(){
for(int i = 0; i < 1024 * 768 * 3; i++){
render_buffer[i] = back_buffer[i];
}
}

void init_term_N(Terminal *term){
int j = 0;
int m = 0;

save_term_state(active_terminal);


term->window.buffer = (unsigned char*)j_malloc(term->window.total_bytes);
while(m+2 < term->window.total_bytes){
term->window.buffer[m++] = 40;
term->window.buffer[m++] = 40;
term->window.buffer[m++] = 40;
}
m = 0;
term->check = true;
term->window.visible = true;
term->window.focused = true;
term->window.minimized = false;
term->window.maximized = false;

active_terminal = term;
is_typing = true;
terminal(term);
total_windows++;

}

void init_term_N_focused(Terminal *term){
save_term_state(active_terminal);


active_terminal = term;

load_term_state(active_terminal);

redraw_windows_z_order(active_terminal);
present_frame();

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
int offset = (x * (3*active_terminal->window.buffer_width)) + (y * 3);

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


void draw_win(int total, int x, int y, int w){
unsigned char *fb = (unsigned char*)screen;

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

void draw_win_region_to_buffer(Terminal *term, unsigned char *destination,
                               int x, int y, int width, int height){

    int start_row = term->window.x + 30 + (term->window.id * 70);
    int start_col = term->window.y + 50 + (term->window.id * 40);

    int end_row = start_row + term->window.height;
    int end_col = start_col + term->window.width;

    int region_end_row = x + height;
    int region_end_col = y + width;

    int top = x > start_row ? x : start_row;
    int left = y > start_col ? y : start_col;

    int bottom = region_end_row < end_row ? region_end_row : end_row;
    int right = region_end_col < end_col ? region_end_col : end_col;

    if(top >= bottom || left >= right)
        return;

    for(int row = top; row < bottom; row++){

        int src = ((row - start_row) * term->window.width
                   + (left - start_col)) * 3;

        int dest = (row * 3072) + (left * 3);

        for(int col = left; col < right; col++){

            destination[dest++] = term->window.buffer[src++];
            destination[dest++] = term->window.buffer[src++];
            destination[dest++] = term->window.buffer[src++];
        }
    }
}

void present_render_region(int x, int y, int width, int height){
unsigned char *fb = (unsigned char*)screen;

for(int row = 0; row < height; row++){
int offset = ((x + row) * 3072) + (y * 3);

for(int col = 0; col < width * 3; col++){
fb[offset + col] = render_buffer[offset + col];
}
}

}

void draw_win_to_buffer(Terminal *term, unsigned char *destination){
int start_row;
int start_col;

if(term->window.maximized){
start_row = 0;
start_col = 0;
}
else{
start_row = term->window.x + 30 + (term->window.id * 70);
start_col = term->window.y + 50 + (term->window.id * 40);
}

int width = term->window.buffer_width;
int height = term->window.buffer_height;

int display_width = term->window.width;
int display_height = term->window.height;

int row_start = 0;
int col_start = 0;

int row_end = display_height < height ? display_height : height;
int col_end = display_width < width ? display_width : width;

if(start_row < 0){
row_start = -start_row;
}
if(start_row + display_height > 768){
row_end = 768 - start_row;
}
if(start_col < 0){
col_start = -start_col;
}
if(start_col + display_width > 1024){
col_end = 1024 - start_col;
}
if(row_start >= row_end || col_start >= col_end){
return;
}
for(int row = row_start; row < row_end; row++){
int src = (row * width + col_start)*3;
int dest = ((start_row + row) * 3072) + (start_col + col_start) * 3;

for(int col = col_start; col < col_end; col++){
destination[dest++] = term->window.buffer[src++];
destination[dest++] = term->window.buffer[src++];
destination[dest++] = term->window.buffer[src++];
}
}

}

void redraw_windows(){
copy_desktop();

    for(int i = 0; i < 5; i++){
     if(term_win[i].check == false)continue;
     if(term_win[i].window.visible == false)continue;
     if(term_win[i].window.minimized == true)continue;

     draw_win_to_buffer(&term_win[i], render_buffer);
    }
}

void redraw_windows_z_order(Terminal *active_terminal){
copy_desktop();

    for(int i = 0; i < 5; i++){
     if(term_win[i].window.id == active_terminal->window.id || term_win[i].check == false || term_win[i].window.visible == false || term_win[i].window.minimized == true )continue;


     draw_win_to_buffer(&term_win[i], render_buffer);
    }
 draw_win_to_buffer(active_terminal, render_buffer);
}

void present_frame(){
unsigned char *fb = (unsigned char*)screen;

for(int i = 0; i < 1024 * 768 * 3; i++){
fb[i] = render_buffer[i];
}

}

void present_region(int x, int y, int width, int height){
unsigned char *fb = (unsigned char*)screen;

for(int i = 0; i < height; i++){
for(int j = 0; j < width * 3; j++){
int offset = ((x + i) * 3072) + ((y *3) + j);
fb[offset] = back_buffer[offset];
}
}
}

void redraw_region(int x, int y, int width, int height){

    // Restore the affected area from the desktop
    for(int row = 0; row < height; row++){

        int offset = ((x + row) * 3072) + (y * 3);

        for(int col = 0; col < width * 3; col++){
            render_buffer[offset + col] = back_buffer[offset + col];
        }
    }

    // Recompose only the windows overlapping this region
    for(int i = 0; i < 5; i++){

        if(term_win[i].window.visible == false) continue;
        if(term_win[i].window.minimized == true) continue;

        draw_win_region_to_buffer(
            &term_win[i],
            render_buffer,
            x, y, width, height
        );
    }

    // Present only the affected area
    present_render_region(x, y, width, height);
}

void mini_draw_win(int x, int y, int y_max){
if(active_terminal->window.maximized == false){
x = (x+30)+(active_terminal->window.id*70);
y = (y+50)+(active_terminal->window.id*40);
}

redraw_region(x, y, 8, 8);
}

int window_slots(){
int slot_taken;

for(slot_taken = 0; slot_taken < 5; slot_taken++){
if(term_win[slot_taken].check == false){
term_win[slot_taken].check = true;
return slot_taken;
}
else if(term_win[slot_taken].check == true && slot_taken == 3){
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

if(active_terminal->window.maximized == false){
x = (x+30)+(active_terminal->window.id*70);
y = (y+50)+(active_terminal->window.id*40);
}

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


void draw_log(int x, int y, unsigned int  color){
fill_screen(x, y, color);
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

}
}
}

void grad_color(char *back_buffer, int row, int col){
for(int i = 0; i < row; i++){
int blue = 204;
int green = 97;
int red = 0;


for(int j = 0; j < col; j++){
int pix = (i*col+j)*3;
back_buffer[pix + 0] = blue;
back_buffer[pix + 1] = green;
back_buffer[pix + 2] = red;
}
}
}

int blend_color(int start, int end, int amount){
return start + ((end - start) * amount)/255;
}

void draw_wallpaper_wave(char *buffer, int row, int col, int start_x, int base, int amplitude, int blue, int green, int red, int alpha){
int width = col - start_x;
for(int x = start_x; x < col; x++){
int pos = x - start_x;
int curve = (pos * (width - pos))/width;
int rise = (curve * amplitude) / (width/4);
int wave_y = base - rise;

if(wave_y < 0)wave_y = 0;
if(wave_y >= row)continue;

for(int y = wave_y; y < row; y++){
int pix = (y*col+x) * 3;

buffer[pix] = (blue*alpha + 204 * (255 - alpha))/255;
buffer[pix + 1] = (green*alpha + 97 * (255 - alpha))/255;
buffer[pix + 2] = (red*alpha)/255;
}
}

}

void grad_task(char *back_buffer, int row_st, int col_st, int row_end, int col_end){
for(int i = row_st; i < row_st+row_end; i++){
int blue, green, red;

if(i < row_st + 2){
blue = 204;
green = 97;
red = 0;
}

else{
blue = 25 + ((i-row_st)/4);
green = 25 + ((i-row_st)/4);
red = 25 + ((i-row_st)/4);
}


for(int j = col_st; j < col_end; j++){
int pix = (i*col_end+j)*3;
back_buffer[pix] = blue;
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


void draw_rect(int x, int y, int w, int h, int color){
for(int i = 0; i < h; i++){
for(int j = 0; j < w; j++){
if(active_terminal != 0){
ultim_fill(x+i, y+j, w, color);
}


else{
ultim_fill(x+i, y+j, 0, color);
}

}
}

}

void draw_task_rect(char *buffer, int x, int y, int width, int height, int color){
for(int i = y; i < y + height; i++){
for(int j = x; j < x + width; j++){

int pix = (i * SCREEN_W + j)*3;

buffer[pix] = color & 0xFF;
buffer[pix + 1] = (color >> 8) & 0xFF;
buffer[pix + 2] = (color >> 16) & 0xFF;

}
}

}


void window_fill_rect(Window *win, int x, int y, int width, int height, int color){
for(int i = x; i < x+height; i++){
for(int j = y; j < y+width; j++){

int offset = (i*win->buffer_width+j)*3;

if(i < 0 || i >= win->buffer_height || j < 0 || j >= win->buffer_width){
continue;
}

win->buffer[offset] = color & 0xFF;
win->buffer[offset+1] = (color >> 8) & 0xFF;
win->buffer[offset+2] = (color >> 16) & 0xFF;
}
}

}

void window_draw_char(Window *win, int x, int y, char al, int color, int color1){
unsigned char* conect = (unsigned char*)font[al];
for(int row = 0; row < 8; row++){
unsigned char conect1 = conect[row];
for(int bit = 0; bit < 8; bit++){

int offset = ((x+row)*win->buffer_width+(y+bit))*3;
int color_used;

if((0x80 >> bit) & conect1){
color_used = color;
}

else{
color_used = color1;
}

if(x+row >= 0 && x+row < win->buffer_height && y+bit >= 0 && y+bit < win->buffer_width){
win->buffer[offset] = color_used & 0xFF;
win->buffer[offset+1] = (color_used >> 8) & 0xFF;
win->buffer[offset+2] = (color_used >> 16) & 0xFF;
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

}

void draw_rect_2(int x, int y, int width, int height, int color){
for(int i = x; i < x+height; i++){
for(int j = y; j < y+width; j++){
if(active_terminal != 0){
fill_screen1(i, j, color);
}



}
}

}

void clear(){
if(active_terminal->window.focused == true){
draw_rect_1(active_terminal->window.x+30, active_terminal->window.y, active_terminal->window.width, active_terminal->window.height, TEXT_B);
draw_win((active_terminal->window.total_bytes), (((active_terminal->window.x+30)+(active_terminal->window.id*70))*3),
(((active_terminal->window.y+50)+(active_terminal->window.id*40))*3), (active_terminal->window.width*3));
cur_clear();
ad_ten = -14;
track_x = 32-14;
track_y = 25;
track_y1 = 25;
letter_track = 25;
}
}

void jan_editor(){
if(active_terminal->window.focused == true){
draw_rect_2(((active_terminal->window.x+60)+(active_terminal->window.id*70)),((active_terminal->window.y+50)+(active_terminal->window.id*40)), (active_terminal->window.width), (active_terminal->window.height), TEXT_B);

ad_ten = -14;
track_x = active_terminal->window.x+32-14;
track_y = active_terminal->window.y+25;
track_y1 = active_terminal->window.y+25;
letter_track = active_terminal->window.y+25;
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
string(35 + ad_ten, 5, "~", TASK_BG, TEXT_B);
string(33 + ad_ten, 15, "$", TASK_BG, TEXT_B);
track_y = 25;
track_y1 = 25;
letter_track = 25;

}

void term_newline_xpromt(){
ad_ten += 14;
track_x += 14;
track_y = 25;
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
ultim_fill(x + row, y + bit, active_terminal->window.buffer_width, color);
}


else{
ultim_fill(x + row, y + bit, 0, color);
}

}

else{
if(active_terminal != 0){
ultim_fill(x + row, y + bit, active_terminal->window.buffer_width, color1);
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
fill_screen(x + row, y + bit, color);

}

else{
fill_screen(x + row, y + bit, color1);
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

void draw_mouse_cursor(int x, int y, unsigned int color){
for(int row = 0; row < 12; row++){
for(int col = 0; col <= row/2; col++){

int px = x + col;
int py = y + row;
if(py < 0 || py >= 768 || px < 0 || px >= 1024)continue;

fill_screen1(py, px, color);
}
}

}

void update_mouse_cursor(int x, int y){
unsigned char *fb = (unsigned char*)screen;

if(mouse_cursor_drawn){
for(int row = 0; row < 12; row++){
for(int col = 0; col < 6; col++){
int py = old_mouse_y + row;
int px = old_mouse_x + col;

if(py < 0 || py >= 768 || px < 0 || px >= 1024)continue;

int offset = (py*3072) + (px*3);

fb[offset] = mouse_saved[row][col][0];
fb[offset+1] = mouse_saved[row][col][1];
fb[offset+2] = mouse_saved[row][col][2];

}
}
}

for(int row = 0; row < 12; row++){
for(int col = 0; col < 6; col++){
int py = y+row;
int px = x+col;

if(py < 0 || py >= 768 || px < 0 || px >= 1024)continue;
int offset = (py*3072) + (px*3);

mouse_saved[row][col][0] = fb[offset];
mouse_saved[row][col][1] = fb[offset+1];
mouse_saved[row][col][2] = fb[offset+2];
}
}

draw_mouse_cursor(x, y, 0xFFFFFF);

old_mouse_x = x;
old_mouse_y = y;
mouse_cursor_drawn = 1;
}

void hide_mouse_cursor(){

if(!mouse_cursor_drawn)return;
unsigned char *fb = (unsigned char*)screen;

for(int row = 0; row < 12; row++){
for(int col = 0; col < 6; col++){
int py = old_mouse_y + row;
int px = old_mouse_x + col;

if(py < 0 || py >= 768 || px < 0 || px >= 1024)continue;

int offset = (py*3072) + (px*3);

fb[offset] = mouse_saved[row][col][0];
fb[offset+1] = mouse_saved[row][col][1];
fb[offset+2] = mouse_saved[row][col][2];

}
}
mouse_cursor_drawn = 0;
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

void draw_window_decorations(Window *win){
int incre = 0;


window_fill_rect(win, 0, 0, win->width, win->title, TITLE_C);


window_fill_rect(win, 7, win->width - 58, 10, 10, 0x00E6B422);
window_fill_rect(win, 7, win->width - 38, 10, 10, 0x0000AA00);
window_fill_rect(win, 7, win->width - 18, 10, 10, RED);

for(int i = 0; win->title_buffer[i] != 0; i++){
window_draw_char(win, (win->title/2)-5, ((win->width/2)-30) + incre, win->title_buffer[i], TEXT_W, TITLE_C);
incre += 10;
}
}

void gen_window(Window *win){

window_fill_rect(win, 0, 0, win->width, win->height, TEXT_B);

draw_window_decorations(win);

redraw_windows();
present_frame();
}

void terminal(Terminal *term){
unsigned int flags;

__asm__ volatile("pushfl; popl %0; cli" : "=r"(flags) :: "memory");

hide_mouse_cursor();

Window *win = &term->window;

gen_window(win);

string(win->title_gap + 35, 5, "~", TASK_BG, TEXT_B);
string(win->title_gap + 33, 15, "$", TASK_BG, TEXT_B);

icon(737, 110);
redraw_windows();
present_frame();

__asm__ volatile("pushl %0; popfl" :: "r"(flags) : "memory", "cc");

update_mouse_cursor(mouse_x, mouse_y);
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

    for(int y = 0; y < 768; y++){
        for(int x = 0; x < 1024; x++){
            int offset = (y*3072) + (x*3);
            screen[offset] = 204;
            screen[offset + 1] = 97;
            screen[offset + 2] = 0;
        }
    }

    for(int i = 0; i < 5; i++){
        term_win[i].check = false;
    }
    for(int j = 0; j < 1024*768*3; j++){
        back_buffer[j] = 0x00000000;
    }

    /* fade in the text */
    for(int bright = 0; bright <= 255; bright += 2){
        int red = bright;
        int green = 97 + (158 * bright/255);
        int blue = 204 + (51 * bright/255);

        unsigned int color = (red << 16) | (green << 8) | blue;
        draw_janiver_text(320, 382, color);
        delay(6000000);
    }
    draw_janiver_text(320, 382, 0xFFFFFF);

    /* draw the wave */
    for(int p = 0; p <= 238; p += 4){
        draw_janiver_wave(291, 388, p, 0, 0xFFFFFF);
        for(volatile int d = 0; d < 1500000; d++);
    }

    /* rotating dots */
    for(int step = 0; step < 30; step++){
        draw_spinner(512, 440, step);
        delay(19000000);
    }

//    while(1);      /* delete this line when you want the desktop to load */

    grad_color(back_buffer, 728, 1024);
    draw_wallpaper_wave(back_buffer, 728, 1024, 5, 728, 120, 225, 120, 20, 100);
    draw_wallpaper_wave(back_buffer, 728, 1024, 1, 728, 80, 214, 112, 3, 100);
//    draw_wallpaper_wave(back_buffer, 728, 1024, 720, 65, 150, 55, 3);
    draw_task_bar();

    copy_desktop();
    present_frame();
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

draw_char_1(745, 945, '0' + hours/10, TEXT_W, CHARCOAL);
draw_char_1(745, 955, '0' + hours%10, TEXT_W, CHARCOAL);


draw_char_1(745, 965, ':', TEXT_W, CHARCOAL);

draw_char_1(745, 975, '0' + minutes/10, TEXT_W, CHARCOAL);
draw_char_1(745, 985, '0' + minutes%10, TEXT_W, CHARCOAL);

draw_char_1(745, 995, ':', TEXT_W, CHARCOAL);

draw_char_1(745, 1005, '0' + seconds/10, TEXT_W, CHARCOAL);
draw_char_1(745, 1015, '0' + seconds%10, TEXT_W, CHARCOAL);

present_region(745, 945, 79, 16);
}
else{
grad_task(back_buffer, TASK_Y, 0, TASK_H, SCREEN_W);
draw_task_rect(back_buffer, 12, TASK_Y + 5, 46, 30, 0x00444444);
for(int x = 0; x < 46; x++){
int curve = (x*(46-x))/46;
int y = TASK_Y + 34 - (curve/12);


int pix = (y*SCREEN_W + 12 + x) *3;

back_buffer[pix] = 204;
back_buffer[pix + 1] = 97;
back_buffer[pix + 2] = 0;

}

icon(90, 40);
unsigned char hours = rtc(0x04);
unsigned char minutes = rtc(0x02);
unsigned char seconds = rtc(0x00);

hours = ((hours >> 4)*10 + (hours & 0x0F));
minutes = ((minutes >> 4)*10 + (minutes & 0x0F));
seconds = ((seconds >> 4)*10 + (seconds & 0x0F));

draw_char_1(745, 945, '0' + hours/10, TEXT_W, CHARCOAL);
draw_char_1(745, 955, '0' + hours%10, TEXT_W, CHARCOAL);


draw_char_1(745, 965, ':', TEXT_W, CHARCOAL);

draw_char_1(745, 975, '0' + minutes/10, TEXT_W, CHARCOAL);
draw_char_1(745, 985, '0' + minutes%10, TEXT_W, CHARCOAL);

draw_char_1(745, 995, ':', TEXT_W, CHARCOAL);

draw_char_1(745, 1005, '0' + seconds/10, TEXT_W, CHARCOAL);
draw_char_1(745, 1015, '0' + seconds%10, TEXT_W, CHARCOAL);
task_active = true;
}
}


void open_terminal(){
int slot_free = window_slots();
if(slot_free != -1){
active_terminal->window.focused = false;
cur_clear();
cur_clear();
init_term_N(&term_win[slot_free]);

active_terminal->window.focused = true;

point_maxim.maxim_flag = 1;
term_win_active = true;
ad_ten = 0;
track_x = term_win[slot_free].window.x+32;
track_y = term_win[slot_free].window.y+25;
track_y1 = term_win[slot_free].window.y+25;
letter_track = term_win[slot_free].window.y+25;

}
}

void close_window(Terminal *term){
int was_active = (term == active_terminal);

term->window.visible = false;
term->window.focused = false;
term->window.minimized = false;
term->check = false;
total_windows--;

if(was_active){
active_terminal = 0;

for(int i = 0; i < 5; i++){
if(term_win[i].check == true && term_win[i].window.visible == true && term_win[i].window.minimized == false){
active_terminal = &term_win[i];
active_terminal->window.focused = true;
load_term_state(active_terminal);
break;
}
}
}

unsigned int flags;

__asm__ volatile("pushfl; popl %0; cli" : "=r"(flags) :: "memory");

hide_mouse_cursor();
redraw_windows();
present_frame();

__asm__ volatile("pushl %0; popfl" :: "r"(flags) : "memory", "cc");

update_mouse_cursor(mouse_x, mouse_y);
}

void resize_window_buffer(Window *win, int new_width, int new_height){
int old_width = win->buffer_width;
int old_height = win->buffer_height;

unsigned char *new_buffer = (unsigned char*)j_malloc(new_width*new_height*3);

for(int i = 0; i < new_width * new_height * 3; i++){
new_buffer[i] = 0;
}

int copy_width = old_width < new_width ? old_width : new_width;
int copy_height = old_height < new_height ? old_height : new_height;

for(int row = 0; row < copy_height; row++){
int old_offset = row*old_width*3;
int new_offset = row*new_width*3;

for(int col = 0; col < copy_width*3; col++){
new_buffer[new_offset + col] = win->buffer[old_offset+col];
}
}
j_free(win->buffer);

win->buffer = new_buffer;
win->buffer_width = new_width;
win->buffer_height = new_height;
win->total_bytes = new_width*new_height*3;
}


void maximize_window(Window *win){
if(win->maximized == false){
resize_window_buffer(win, 1024, 728);

win->restore_x = win->x;
win->restore_y = win->y;
win->restore_width = win->width;
win->restore_height = win->height;


win->x = -30 - (win->id*70);
win->y = -50 - (win->id*40);

win->width = 1024;
win->height = 728;

win->maximized = true;
draw_window_decorations(win);
}

else{
resize_window_buffer(win, win->restore_width, win->restore_height);
win->x = win->restore_x;
win->y = win->restore_y;
win->width = win->restore_width;
win->height = win->restore_height;

win->maximized = false;
draw_window_decorations(win);
}

}


#define BG_COLOR 0x0061CC

void draw_janiver_text(int top, int left, unsigned int color){

    unsigned char *fb = (unsigned char*)screen;

    int bg_b = BG_COLOR & 0xFF;
    int bg_g = (BG_COLOR >> 8) & 0xFF;
    int bg_r = (BG_COLOR >> 16) & 0xFF;

    int c_b = color & 0xFF;
    int c_g = (color >> 8) & 0xFF;
    int c_r = (color >> 16) & 0xFF;

    for(int y = 0; y < TEXTAA_HEIGHT; y++){
        for(int x = 0; x < TEXTAA_WIDTH; x++){
            int a = textaa_alpha[y * TEXTAA_WIDTH + x];
            if(a == 0) continue;

            int offset = ((top + y) * 3072) + ((left + x) * 3);
            fb[offset]     = (bg_b * (255 - a) + c_b * a) / 255;
            fb[offset + 1] = (bg_g * (255 - a) + c_g * a) / 255;
            fb[offset + 2] = (bg_r * (255 - a) + c_r * a) / 255;
        }
    }
}


void draw_janiver_wave(int top, int left, int progress, int phase, unsigned int color){

    unsigned char *fb = (unsigned char*)screen;

    int bg_b = BG_COLOR & 0xFF;
    int bg_g = (BG_COLOR >> 8) & 0xFF;
    int bg_r = (BG_COLOR >> 16) & 0xFF;

    int c_b = color & 0xFF;
    int c_g = (color >> 8) & 0xFF;
    int c_r = (color >> 16) & 0xFF;

    if(progress > WAVEAA_WIDTH) progress = WAVEAA_WIDTH;

    for(int x = 0; x < progress; x++){
        for(int y = 0; y < WAVEAA_HEIGHT; y++){
            int a = waveaa_alpha[y * WAVEAA_WIDTH + x];
            if(a == 0) continue;

            int offset = ((top + y) * 3072) + ((left + x) * 3);
            fb[offset]     = (bg_b * (255 - a) + c_b * a) / 255;
            fb[offset + 1] = (bg_g * (255 - a) + c_g * a) / 255;
            fb[offset + 2] = (bg_r * (255 - a) + c_r * a) / 255;
        }
    }
}


void draw_spinner(int cx, int cy, int step){

    int dx[8] = {0, 10, 14, 10, 0, -10, -14, -10};
    int dy[8] = {-14, -10, 0, 10, 14, 10, 0, -10};

    for(int i = 0; i < 8; i++){

        int age = ((step % 8) - i + 8) % 8;     /* 0 = newest dot, brightest */
        int level = 255 - age * 30;

        int red   = level;
        int green = 97 + (158 * level / 255);
        int blue  = 204 + (51 * level / 255);

        for(int yy = -3; yy <= 3; yy++){
            for(int xx = -3; xx <= 3; xx++){
                if(xx * xx + yy * yy > 10) continue;

                int offset = ((cy + dy[i] + yy) * 3072) + ((cx + dx[i] + xx) * 3);
                screen[offset]     = blue;
                screen[offset + 1] = green;
                screen[offset + 2] = red;
            }
        }
    }
}
