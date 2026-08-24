#ifndef WIN_H
#define WIN_H
#define DESK_BG 0x00A8D8EA
#define TASK_BG 0x0281C0E9
#define TASK_H 40
#define TEXT_W 0x00FFFFFF
#define TEXT_B 0x00000000
#define RED 0x00FF0000
#define GREYY 0x00333333
#define SCREEN_W 1024
#define SCREEN_H 768
#define MAXIMUM 728
#define TASK_Y (SCREEN_H - TASK_H)
#define VIB_BLUE 0xFF1C222E

#define TERM_X 550
#define TERM_Y 750
#define TERM_TITLE 30
extern struct window_size point_maxim;
extern unsigned char* screen;
extern int ad_ten;
extern int track_x;
extern int track_y;
extern int track_y1;
extern int letter_track;
extern bool task_active;
extern bool is_typing;
extern int slot_counter;
int window_slots();
int window_slots_used();
void fill_screen(int x, int y, unsigned int color);
void fill_screen1(int x, int y, unsigned int color);
void icon(int xth, int yth);
void draw_rect(int x, int y, int w, int h, int color);
void draw_char(int x, int y, char al, int color, int color1);
void string(int x, int y, char *str, int color, int color1);
void init_screen();
void boot_anime();
void draw_wallpaper();
void term_newline();
void term_input(int x, int y, char al, int color, int color1);
void flush();
void term_newline_xpromt();
void term_newline_max();
void mini_draw_win(int x, int y, int y_max);
void draw_win(int total, int x, int y, int w);
void terminal_max(int x, int y, int term_y, int term_x, int term_title, char *str );
//void terminal_close(int x, int y, int term_y, int term_x, int term_title, char *str);
void terminal_close();
int fat_read(char *filename, char *output_buf);
unsigned char rtc(unsigned char reg);
void draw_desktop();
void draw_task_bar();
void cur_clear();
void clear();
void cur_clear_b();
void delay(unsigned int time);
void port_send(unsigned short port, unsigned short value);
struct window_size{
int x_beg;
int x_end;
int y_beg;
int y_end;
int term_tit;
char maxim_flag;
}__attribute__((packed));

typedef struct{
int x;
int y;

int width;
int height;
int id;

int title;
int title_gap;
char title_buffer[14];
unsigned char *buffer;
int total_bytes;

bool visible;
bool focused;
}__attribute__ ((packed)) Window;

typedef struct{
Window window;

bool check;

int track_x;
int track_y;
int track_y1;
int letter_track;
int ad_ten;
int position;
int browse;
int arrow_count;
char input_buffer[1024];
}__attribute__ ((packed)) Terminal;

extern Terminal term_win[5];

void terminal(Terminal *term);
void init_term_N(Terminal *term);
void init_term_N_focused(Terminal *term);
extern Terminal *active_terminal;

void save_term_state(Terminal *term);
void load_term_state(Terminal *term);

#endif
