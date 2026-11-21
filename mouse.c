#include "mouse.h"
#include "keyboard.h"
#include "win.h"

unsigned char mouse_response = 0;

int mouse_x = 512;
int mouse_y = 384;

unsigned char mouse_data[3];
int mouse_cycle = 0;

volatile int mouse_left_pressed = 0;
volatile int mouse_left_clicked = 0;

void mouse_handler(){
unsigned char data = port_read(0x60);

if(mouse_cycle == 0 && !(data & 0x08))return;
if(mouse_cycle == 0 && (data & 0xC0))return;

mouse_data[mouse_cycle] = data;
mouse_cycle++;

if(mouse_cycle == 3){
mouse_cycle = 0;

int left_button = mouse_data[0] & 1;

if(left_button && !mouse_left_pressed){
mouse_left_clicked = 1;
}

mouse_left_pressed = left_button;

int dx = (signed char)mouse_data[1];
int dy = (signed char)mouse_data[2];

mouse_x += dx;
mouse_y -= dy;

if(mouse_x < 0)mouse_x = 0;
if(mouse_x > 1023)mouse_x = 1023;

if(mouse_y < 0)mouse_y = 0;
if(mouse_y > 767)mouse_y = 767;

update_mouse_cursor(mouse_x, mouse_y);
}
}


void mouse_init(){
while(port_read(0x64) & 0x02);
port_send(0x64, 0xA8);

while(port_read(0x64) & 0x02);
port_send(0x64, 0x20);

while(!(port_read(0x64) & 0x01));
unsigned char status = port_read(0x60);
status |= 2;

while(port_read(0x64) & 0x02);
port_send(0x64, 0x60);

while(port_read(0x64) & 0x02);
port_send(0x60, status);

while(port_read(0x64) & 0x02);
port_send(0x64, 0xD4);

while(port_read(0x64) & 0x02);
port_send(0x60, 0xF4);

while(!(port_read(0x64) & 0x01));
mouse_response = port_read(0x60);


}


int mouse_take_left_click(){
unsigned int flags;

__asm__ volatile("pushfl; popl %0; cli" : "=r"(flags) :: "memory");

int clicked = mouse_left_clicked;
mouse_left_clicked = 0;

__asm__ volatile("pushl %0; popfl" :: "r"(flags) : "memory", "cc");

return clicked;
}

