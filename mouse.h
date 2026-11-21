#ifndef MOUSE_H
#define MOUSE_H

extern int mouse_x;
extern int mouse_y;

void mouse_init();
void mouse_handler();

extern volatile int mouse_left_pressed;
extern volatile int mouse_left_clicked;
int mouse_take_left_click();

#endif
