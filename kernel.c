#include "memory.h"
#include "fat.h"
#include "vga.h"
#include "keyboard.h"
#include "ata.h"
#include "idt.h"
#include "timer.h"
#include "gdt.h"
#include "font.h"
#include "win.h"
#include "calcul.h"
#define GREEN 0x02
#define NO 0x0F
#define GREY 0x07
#define YELLOW 0x0E
#define VIOLET 0x05
#define BLACK 0x00000000
#define WHITE 0x00FFFFFF
void reeb();
void shut();
void write_file(char *filename, char *data, int size);
void rm(char *filename);
char history_store[10][255];
unsigned char* screen;
unsigned int column = 1024;
unsigned int row = 768;
int track_x = 65;
int track_y = 120 + 5 + 10 + 10;
int track_y1 = 120 + 5 + 10 + 10;
int ad_ten = 0;
int letter_track = 145;
bool is_typing = false;
bool term_win_active = false;
void init_history(){
for(int i = 0; i < 10; i++){
for(int j = 0; j < 255; j++){
history_store[i][j] = 0;
}
}
}

int strcmp(char* a, char* b){
int count = 0;
while(a[count] && b[count]){
if(a[count] != b[count]){
return 0;
}
count++;
}
return a[count] == b[count];
}

int strlen(char *a){
int i;
for(i = 0; a[i]!= 0; i++);
return i;
}

void read_sector(int size, char* buffer){
char* disk = (char*)0x20000;
char* position = disk + (size * SECTOR_SIZE);
for(int i = 0; i < SECTOR_SIZE; i++){
buffer[i] = position[i];
}
}

void ls(){
char* sector = (char*)my_malloc(512);
struct Dir* level;
for(int s = 0; s < 14; s++){
ata_read(ROOT_DIR_START + s, sector);
level = (struct Dir*)sector;

for(int i = 0; i < SECTOR_SIZE/sizeof(struct Dir); i++){
if((level + i) -> name[0] == 0x00){
my_free(sector);
return;
}
if((level + i) -> name[0] == 0xE5){
continue;
}
if((level + i) -> attributes == 0x0F){
continue;
}

if(point_maxim.maxim_flag == 1){
if(active_terminal != 0){
    track_y1 = active_terminal->window.y + 15;
    }

track_x += 14;
ad_ten += 14;
for(int j = 0; j < 8; j++){
if((level + i) -> name[j] == ' '){
break;
}
track_y1 += 10;
term_input(track_x, track_y1, (level + i) -> name[j], TASK_BG, TEXT_B);
}

if((level + i) -> ext[0] != ' '){
track_y1 += 10;
term_input(track_x, track_y1, '.', TASK_BG, TEXT_B);
for(int j = 0; j < 3; j++){
if((level + i) -> ext[j] == ' '){
break;
}
track_y1 += 10;
term_input(track_x, track_y1, (level + i) -> ext[j], TASK_BG, TEXT_B);
}
}
}
else if(point_maxim.maxim_flag == 0){
track_x += 14;
ad_ten += 14;
track_y1 = 5;
for(int j = 0; j < 8; j++){
if((level + i) -> name[j] == ' '){
break;
}
track_y1 += 10;
draw_char(track_x, track_y1, (level + i) -> name[j], TASK_BG, TEXT_B);
}

if((level + i) -> ext[0] != ' '){
track_y1 += 10;
draw_char(track_x, track_y1, '.', TASK_BG, TEXT_B);
for(int j = 0; j < 3; j++){
if((level + i) -> ext[j] == ' '){
break;
}
track_y1 += 10;
draw_char(track_x, track_y1, (level + i) -> ext[j], TASK_BG, TEXT_B);
}
}
term_newline_max();

}
}
}
my_free(sector);
}

void cat(char *filename) {
    if(term_win[0].check == true && term_win[1].check == false){
    track_y1 = active_terminal->window.y + 15;
    track_y = active_terminal->window.y + 15;;
    }

    track_x += 14;
    ad_ten += 14;
    char *sector = (char*)my_malloc(512);
    struct Dir *level;

    // separate filename and extension
    char name[8] = {' ',' ',' ',' ',' ',' ',' ',' '};
    char ext[3]  = {' ',' ',' '};

    int i = 0;
    int j = 0;
    while (filename[i] && filename[i] != '.') {
        name[j++] = filename[i++];
    }
    if (filename[i] == '.') {
        i++;
        j = 0;
        while (filename[i]) {
            ext[j++] = filename[i++];
        }
    }

    for (int s = 0; s < 14; s++) {
        ata_read(ROOT_DIR_START + s, sector);
        level = (struct Dir*)sector;

        for (int i = 0; i < SECTOR_SIZE / sizeof(struct Dir); i++) {
            if ((level+i)->name[0] == 0x00) {
                   my_free(sector);
                   if(point_maxim.maxim_flag == 1){
                   cur_clear_b();
                   string(track_x, track_y1, "File not found", DESK_BG, TEXT_B);
                   }
                   else if(point_maxim.maxim_flag == 0){
                   string(track_x, track_y1, "File not found", DESK_BG, TEXT_B);
                   }
                   return; }
            if ((level+i)->name[0] == 0xE5) continue;
            if ((level+i)->attributes == 0x0F) continue;

            // match name and extension
            int match = 1;
            for (int j = 0; j < 8; j++) {
                if ((level+i)->name[j] != name[j]) { match = 0; break; }
            }
            for (int j = 0; j < 3; j++) {
                if ((level+i)->ext[j] != ext[j]) { match = 0; break; }
            }

            if (match) {
                int cluster = (level+i)->first_cluster;
                int size = (level+i)->file_size;
                int sector_num = DATA_START + (cluster - 2)*CLUSTER_SIZE;

                char *file_buf = (char*)my_malloc(size);
                for(int k = 0; k < size; k++){
                file_buf[k] = 0;
                }
                ata_read(sector_num, file_buf);

                newline();
                for(int k = 0; k < size; k++){
                if(file_buf[k] == '\r'){
                continue;
                }
                if(file_buf[k] == '\n'){
                newline();
                continue;
                }
                if(point_maxim.maxim_flag == 1){
                 if(track_y1 >= (active_terminal->window.width+active_terminal->window.y) && active_terminal != 0){
                  cur_clear();
                  track_y1 = active_terminal->window.y + 15;
                  track_y = active_terminal->window.y + 15;
                  track_x += 14;
                  ad_ten += 14;
                  }

                cur_clear();
                term_input(track_x, track_y1, file_buf[k], DESK_BG, TEXT_B);
                track_y1 += 10;
                track_y += 10;
                }
                else if(point_maxim.maxim_flag == 0){
                term_input(track_x, track_y1, file_buf[k], DESK_BG, TEXT_B);
                track_y1 += 10;
                }
                }

                my_free(file_buf);
                my_free(sector);
                return;
            }
        }
    }
    my_free(sector);
    if(point_maxim.maxim_flag == 1){
    cur_clear_b();
    string(track_x, track_y1, "File not found", DESK_BG, TEXT_B);
    }
    else if(point_maxim.maxim_flag == 0){
    string(track_x, track_y1, "File not found", DESK_BG, TEXT_B);
    }
}

/*
int fat_read(char *filename, char *output_buf){
    char *sector = (char*)my_malloc(512);
    struct Dir *level;

    char name[8] = {' ',' ',' ',' ',' ',' ',' ',' '};
    char ext[3]  = {' ',' ',' '};

    int i = 0;
    int j = 0;
    while(filename[i] && filename[i] != '.'){
        name[j++] = filename[i++];
    }
    if(filename[i] == '.'){
        i++;
        j = 0;
        while(filename[i]){
            ext[j++] = filename[i++];
        }
    }

    for(int s = 0; s < 14; s++){
        ata_read(ROOT_DIR_START + s, sector);
        level = (struct Dir*)sector;

        for(int i = 0; i < SECTOR_SIZE / sizeof(struct Dir); i++){
            if((level+i)->name[0] == 0x00){
                my_free(sector);
                return -1;
            }
            if((level+i)->name[0] == 0xE5) continue;
            if((level+i)->attributes == 0x0F) continue;

            int match = 1;
            for(int j = 0; j < 8; j++){
                if((level+i)->name[j] != name[j]){
                    match = 0;
                    break;
                }
            }
            for(int j = 0; j < 3; j++){
                if((level+i)->ext[j] != ext[j]){
                    match = 0;
                    break;
                }
            }

            if(match){
                int cluster = (level+i)->first_cluster;
                int size = (level+i)->file_size;
                int bytes_read = 0;

                char *fat_buf = (char*)my_malloc(512);

                while(cluster >= 2 && cluster < 0xFFF8 && bytes_read < size){
                    int sector_num = DATA_START + (cluster - 2) * CLUSTER_SIZE;

                    for(int s = 0; s < CLUSTER_SIZE && bytes_read < size; s++){
                        int bytes_left = size - bytes_read;

                        if(bytes_left >= SECTOR_SIZE){
                            ata_read(sector_num + s, output_buf + bytes_read);
                            bytes_read += SECTOR_SIZE;
                        } else {
                            char *temp = (char*)my_malloc(512);
                            ata_read(sector_num + s, temp);
                            for(int b = 0; b < bytes_left; b++){
                                output_buf[bytes_read + b] = temp[b];
                            }
                            bytes_read += bytes_left;
                            my_free(temp);
                        }
                    }

                    // read next cluster from FAT
// read next cluster from FAT16
// read next cluster from FAT16
unsigned short current_cluster = cluster;

int fat_offset = current_cluster * 2;
int fat_sector_num = FAT_START + (fat_offset / SECTOR_SIZE);
int fat_entry_offset = fat_offset % SECTOR_SIZE;

ata_read(fat_sector_num, fat_buf);

cluster = *(unsigned short *)(fat_buf + fat_entry_offset);
                }

                my_free(fat_buf);
                my_free(sector);
                return size;
            }
        }
    }
    my_free(sector);
    return -1;
}
*/

void print_tim();

void date();

void print_uptime(){
unsigned int seconds = ticks/18;
unsigned int minutes = seconds/60;
unsigned int hours = minutes/60;

seconds = seconds % 60;
minutes = minutes % 60;
if(point_maxim.maxim_flag == 1){
track_x += 14;
ad_ten += 14;
track_y1 = active_terminal->window.y + 15;
string(track_x, track_y1, "Up-time:", TASK_BG, TEXT_B);

term_input(track_x, track_y1, '0' + hours/10, TASK_BG, TEXT_B);
track_y1 += 10;
term_input(track_x, track_y1, '0' + hours%10, TASK_BG, TEXT_B);
track_y1 += 10;

string(track_x, track_y1, ":", TASK_BG, TEXT_B);

term_input(track_x, track_y1, '0' + minutes/10, TASK_BG, TEXT_B);
track_y1 += 10;
term_input(track_x, track_y1, '0' + minutes%10, TASK_BG, TEXT_B);
track_y1 += 10;

string(track_x, track_y1, ":", TASK_BG, TEXT_B);

term_input(track_x, track_y1, '0' + seconds/10, TASK_BG, TEXT_B);
track_y1 += 10;
term_input(track_x, track_y1, '0' + seconds%10, TASK_BG, TEXT_B);
}
else if(point_maxim.maxim_flag == 0){
track_x += 14;
ad_ten += 14;
track_y1 = active_terminal->window.y + 15;
string(track_x, track_y1, "Up-time:", TASK_BG, TEXT_B);

term_input(track_x, track_y1, '0' + hours/10, TASK_BG, TEXT_B);
track_y1 += 10;
term_input(track_x, track_y1, '0' + hours%10, TASK_BG, TEXT_B);
track_y1 += 10;

string(track_x, track_y1, ":", TASK_BG, TEXT_B);

term_input(track_x, track_y1, '0' + minutes/10, TASK_BG, TEXT_B);
track_y1 += 10;
term_input(track_x, track_y1, '0' + minutes%10, TASK_BG, TEXT_B);
track_y1 += 10;

string(track_x, track_y1, ":", TASK_BG, TEXT_B);

term_input(track_x, track_y1, '0' + seconds/10, TASK_BG, TEXT_B);
track_y1 += 10;
term_input(track_x, track_y1, '0' + seconds%10, TASK_BG, TEXT_B);
}
}
void shell(char* cmd){
if(cmd[0] == 0){
return;
}


if(strcmp(cmd, "help")){
if(point_maxim.maxim_flag == 1){
track_x += 14;
ad_ten += 14;

track_y1 = active_terminal->window.y + 15;

string(track_x, track_y1, "--------------------------Available commands---------------------", TASK_BG, TEXT_B);
track_x += 14;
ad_ten += 14;


track_y1 = active_terminal->window.y + 15;


string(track_x, track_y1,"------>>>>>help, about, clear, version, echo, rm, time, calc<<<<<------", TASK_BG, TEXT_B);
track_x += 14;
ad_ten += 14;


track_y1 = active_terminal->window.y + 15;



string(track_x, track_y1,"---->>>>>date, reboot, history, ls, cat, write, uptime, exit<<<<<-----",TASK_BG, TEXT_B);

}
else if(point_maxim.maxim_flag == 0){
track_x += 14;
ad_ten += 14;
track_y1 = 5;
string(track_x, track_y1, "--------------------------Available commands---------------------", TASK_BG, TEXT_B);
track_x += 14;
ad_ten += 14;
track_y1 = 5;
string(track_x, track_y1,"------>>>>>help, about, clear, version, echo, rm, time<<<<<------", TASK_BG, TEXT_B);
track_x += 14;
ad_ten += 14;
track_y1 = 5;
string(track_x, track_y1,"---->>>>>date, reboot, history, ls, cat, write, uptime, exit<<<<<-----",TASK_BG, TEXT_B);
}
}

else if(strcmp(cmd, "about")){
if(point_maxim.maxim_flag == 1){
track_x += 14;
ad_ten += 14;


track_y1 = active_terminal->window.y + 15;


string(track_x, track_y1, "Jan Operating System .", TASK_BG, TEXT_B);
}

else if(point_maxim.maxim_flag == 0){
track_x += 14;
ad_ten += 14;
track_y1 = 5;
string(track_x, track_y1, "Jan Operating System .", TASK_BG, TEXT_B);
}
}

else if(strcmp(cmd, "clear")){
clear();
}
else if(strcmp(cmd, "version")){
if(point_maxim.maxim_flag == 1){
track_x += 14;
ad_ten += 14;



track_y1 = active_terminal->window.y + 15;


string(track_x, track_y1, "Version 1.0x", TASK_BG, TEXT_B);
}

else if(point_maxim.maxim_flag == 0){
track_x += 14;
ad_ten += 14;
track_y1 = 5;
string(track_x, track_y1, "Version 1.0x", TASK_BG, TEXT_B);
}
}

else if(cmd[0] == 'e' && cmd[1] == 'c' && cmd[2] == 'h' && cmd[3] == 'o' && cmd[4] == ' '){
track_x += 14;
ad_ten += 14;

track_y1 = active_terminal->window.y + 15;

string(track_x, track_y1, cmd + 5, TASK_BG, TEXT_B);
}

else if(strcmp(cmd, "time")){
print_tim();
}

else if(strcmp(cmd, "date")){
date();
}

else if(strcmp(cmd, "reboot")){
is_typing = false;
reeb();
term_newline();
}

else if(strcmp(cmd, "shutdown")){
shut();
}

else if(strcmp(cmd, "uptime")){
print_uptime();
}

else if(strcmp(cmd, "history")){
int counter = 0;
while(counter < 10){
if(history_store[counter % 10][0] != 0){
for(int i = 0; history_store[counter % 10][i] != 0; i++){
print_char(history_store[counter % 10][i], VIOLET);
}
newline();
}
counter++;
}
}

else if(strcmp(cmd, "ls")){
ls();
}

else if(strcmp(cmd, "exit")){
task_active = false;
draw_task_bar();
//terminal_close(30, 120, TERM_Y, TERM_X, TERM_TITLE, "Jan Terminal");
terminal_close();
icon(90, 120);
term_win_active = false;
point_maxim.maxim_flag = 0;
}

else if(cmd[0] == 'c' && cmd[1] == 'a' && cmd[2] == 't' && cmd[3] == ' '){
cat(cmd + 4);
cur_clear();
}

else if(cmd[0] == 'w' && cmd[1] == 'r' && cmd[2] == 'i' && cmd[3] == 't' && cmd[4] == 'e'){
if(cmd[5] != ' '){
printR("Guide: write filename file_content");
newline();
}
else{
int k = 6;
while(cmd[k] != 0 && cmd[k] != ' ')k++;
if(cmd[k] == 0){
printR("Guide: write filename file_content");
newline();
}
else{
cmd[k] = 0;
write_file(cmd + 6, cmd + k + 1, strlen(cmd + k + 1));
}
}
}

else if(cmd[0] == 'r' && cmd[1] == 'm' && cmd[2] == ' '){
rm(cmd + 3);
newline();
}

else if(cmd[0] == 'c' && cmd[1] == 'a' && cmd[2] == 'l' && cmd[3] == 'c' && cmd[4] == ' '){
track_x += 14;
ad_ten += 14;

track_y1 = active_terminal->window.y + 15;


int k = 5;
if(cmd[k] == '\0'){
string(track_x, track_y1, "Guide:", RED, TEXT_B);
}
else{
calc(cmd + 5,strlen(cmd + 5));
}
}


else{
track_x += 14;
ad_ten += 14;

track_y1 = active_terminal->window.y + 15;

string(track_x, track_y1, "Command does not exist yet.", RED, TEXT_B);
}
}

int find_cluster(){
    char *sector = (char*)my_malloc(512);
    for(int s = 0; s < FAT_SIZE; s++){
        ata_read(FAT_START + s, sector);
        unsigned short *fat = (unsigned short*)sector;
        for(int i = 0; i < 256; i++){
            int clust = s * 256 + i;
            if(clust < 2) continue;
            if(fat[i] == 0x0000){
                my_free(sector);
                return clust;
            }
        }
    }
    my_free(sector);
    return -1;
}

void clus_taken(char *fat, int clust){
    unsigned short *fat16 = (unsigned short*)fat;
    fat16[clust % 256] = 0xFFFF;
}

void write_file(char *filename, char *data, int size){
char *sector = (char*)my_malloc(512);

char file_name[8] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
char ext[3] = {' ', ' ', ' '};

int i = 0, j = 0;
while(filename[i] && filename[i] != '.' && j < 8){
file_name[j++] = filename[i++];
}

if(filename[i] == '.'){
i++;
j = 0;

while(filename[i] && j < 3){
ext[j++] = filename[i++];
}
}

else{
ext[0] = 'T';
ext[1] = 'X';
ext[2] = 'T';
}

int cluster = find_cluster();
if(cluster == -1){
cur_clear();
if(active_terminal != 0){
    track_y1 = active_terminal->window.y + 15;
    }

track_x += 14;
ad_ten += 14;
string(track_x, track_y1, "Disk full", DESK_BG, TEXT_B);
my_free(sector);
return;
}

char *fat = (char*)my_malloc(512);
char *dst = (char*)my_malloc(512);

ata_read(FAT_START, fat);
clus_taken(fat, cluster);

ata_write(FAT_START, fat);

my_free(fat);

for(int s = 0; s < 14; s++){
ata_read(ROOT_DIR_START + s, sector);
struct Dir *level = (struct Dir*)sector;

for(int i = 0; i < SECTOR_SIZE/sizeof(struct Dir); i++){

if((level + i) -> name[0] == 0x00){
break;
}
if((level + i) -> name[0] == 0xE5){
continue;
}

int exist = 1;

for(int j = 0; j < 8; j++){
if((level + i) -> name[j] != file_name[j]){
exist = 0;
break;
}
}

if(exist){
if(point_maxim.maxim_flag == 1){
cur_clear();
if(active_terminal != 0){
    track_y1 = active_terminal->window.y + 15;
    }

track_x += 14;
ad_ten += 14;
string(track_x, track_y1,"File already exist", DESK_BG, TEXT_B);
return;
}
else if(point_maxim.maxim_flag == 0){
cur_clear();
track_y1 = 120 + 10 + 5;
track_x += 14;
ad_ten += 14;
string(track_x, track_y1,"File already exist", DESK_BG, TEXT_B);
return;
}
}
}
for(int i = 0; i < SECTOR_SIZE/sizeof(struct Dir); i++){

if((level + i) -> name[0] == 0x00 || (level + i) -> name[0] == 0xE5){
for(int j = 0; j < 8; j++){
(level + i) -> name[j] = file_name[j];
}

for(int j = 0; j < 3; j++){
(level + i) -> ext[j] = ext[j];
}

(level + i) -> attributes = 0x20;
(level + i) -> first_cluster = cluster;
(level + i) -> file_size = size;

ata_write(ROOT_DIR_START+s, sector);

for(int i = 0; i < 512; i++){
dst[i] = 0;
}
for(int j = 0; j < size; j++){
dst[j] = data[j];
}
ata_write(DATA_START + (cluster -2)*CLUSTER_SIZE, dst);
my_free(dst);
my_free(sector);
cur_clear();
if(active_terminal != 0){
    track_y1 = active_terminal->window.y + 15;
    }

track_x += 14;
ad_ten += 14;
string(track_x, track_y1, "File written successfully", DESK_BG, TEXT_B);
return;
}
}
}
my_free(sector);
cur_clear();
if(active_terminal != 0){
    track_y1 = active_terminal->window.y + 15;
    }

track_x += 14;
ad_ten += 14;
string(track_x, track_y1, "Disk is full", DESK_BG, TEXT_B);
}

void rm(char *filename){
char name_1[8] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
char ext_1[3] = {' ', ' ', ' '};
int i = 0, j = 0;

while(filename[i] != '\0'){
if(filename[i] == '.'){
i++;
int v = 0;
while(filename[i] && v < 3){
ext_1[v] = filename[i];
i++;
v++;
}
break;
}
if(j < 8){
name_1[j] = filename[i];
j++;
}
i++;
}
char *sector = (char*)my_malloc(512);
struct Dir *level = (struct Dir*)sector;

for(int s = 0; s < 14; s++){
ata_read(ROOT_DIR_START + s, sector);

for(int i = 0; i < SECTOR_SIZE/sizeof(struct Dir); i++){
if((level + i) -> name[0] == 0x00){
break;
}

if((level + i) -> name[0] == 0xE5){
continue;
}

int x, v = 0;
int match = 1;
for(x = 0; x < 8; x++){
if((level + i) -> name[x] != name_1[x]){
match = 0;
break;
}
}
while(v < 3){
if((level + i) -> ext[v] != ext_1[v]){
match = 0;
break;
}
v++;
}

if(match){

(level + i) -> name[0] = 0xE5;
cur_clear();
if(active_terminal != 0){
    track_y1 = active_terminal->window.y + 15;
    }

track_x += 14;
ad_ten += 14;
string(track_x, track_y1, "File deleted", DESK_BG, TEXT_B);
char *buff = (char*)my_malloc(512);

ata_read(FAT_START, buff);

int cluster = (level + i) -> first_cluster;
unsigned short *fat16 = (unsigned short*)buff;
fat16[cluster] = 0x0000;  // mark as free

ata_write(FAT_START, buff);
my_free(buff);

ata_write(ROOT_DIR_START + s, sector);
my_free(sector);
return;
}

}
}
cur_clear_b();
if(active_terminal != 0){
    track_y1 = active_terminal->window.y + 15;
    }

track_x += 14;
ad_ten += 14;
string(track_x, track_y1, "File does not exist", DESK_BG, TEXT_B);
my_free(sector);
return;
}







void print_tim(){
unsigned char hours = rtc(0x04);
unsigned char minutes = rtc(0x02);
unsigned char seconds = rtc(0x00);

hours = ((hours >> 4)*10 + (hours & 0x0F));
minutes = ((minutes >> 4)*10 + (minutes & 0x0F));
seconds = ((seconds >> 4)*10 + (seconds & 0x0F));
if(point_maxim.maxim_flag == 1){
track_x += 14;
ad_ten += 14;
track_y1 = active_terminal->window.y + 15;

draw_char(track_x, track_y1, '0' + hours/10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + hours%10, TASK_BG, TEXT_B);

track_y1 += 10;
draw_char(track_x, track_y1, ':', TASK_BG, TEXT_B);
track_y1 += 10;

draw_char(track_x, track_y1, '0' + minutes/10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + minutes%10, TASK_BG, TEXT_B);
track_y1 += 10;

draw_char(track_x, track_y1, ':', TASK_BG, TEXT_B);
track_y1 += 10;

draw_char(track_x, track_y1, '0' + seconds/10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + seconds%10, TASK_BG, TEXT_B);
}
else if(point_maxim.maxim_flag == 0){
track_x += 14;
ad_ten += 14;
track_y1 = 5;

draw_char(track_x, track_y1, '0' + hours/10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + hours%10, TASK_BG, TEXT_B);

track_y1 += 10;
draw_char(track_x, track_y1, ':', TASK_BG, TEXT_B);
track_y1 += 10;

draw_char(track_x, track_y1, '0' + minutes/10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + minutes%10, TASK_BG, TEXT_B);
track_y1 += 10;

draw_char(track_x, track_y1, ':', TASK_BG, TEXT_B);
track_y1 += 10;

draw_char(track_x, track_y1, '0' + seconds/10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + seconds%10, TASK_BG, TEXT_B);
}
}

void date(){
unsigned char day = rtc(0x07);
unsigned char month = rtc(0x08);
unsigned char year = rtc(0x09);

day = (day >> 4)*10 + (day & 0x0F);
month = (month >> 4)*10 + (month & 0x0F);
year = (year >> 4)*10 + (year & 0x0F);

if(point_maxim.maxim_flag == 1){
track_x += 14;
ad_ten += 14;
track_y1 = active_terminal->window.y + 15;

draw_char(track_x, track_y1, '0' + day/10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + day%10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '/', TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + month/10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + month%10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '/', TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '2', TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0', TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + year/10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + year%10, TASK_BG, TEXT_B);
}

else if(point_maxim.maxim_flag == 0){
track_x += 14;
ad_ten += 14;
track_y1 = 5;
draw_char(track_x, track_y1, '0' + day/10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + day%10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '/', TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + month/10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + month%10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '/', TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '2', TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0', TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + year/10, TASK_BG, TEXT_B);
track_y1 += 10;
draw_char(track_x, track_y1, '0' + year%10, TASK_BG, TEXT_B);
}
}


void reeb(){
port_send(0x64, 0xFE);
}

void shut(){
port_send(0x604, 0x2000);
}
__attribute__((section(".text.kernel_main")))
void kernel_main() {
    init_gdt();
    initialize_pic();
    init_history();
    init_screen();
    boot_anime();
//    draw_wallpaper();
    //set_page_table();
    //paging();

    print("                   ");
    print("Jan Operating System Version 1.0");
    newline();
char buffer[1024];
int history_count = 0;
int browse = 0;
int arrow_count = 0;
int position = 0;
int pro = 0;
int blink = 0;
pro = cursor;
    while (1) {
if(point_maxim.maxim_flag == 1){
if((ticks / 15) % 2 == 0){
cur();
}
else{
cur_clear();
}
}
draw_task_bar();
if(last_key){
if(point_maxim.maxim_flag == 1 && last_key == '\b' && term_win_active == true){

if(active_terminal != 0){

if(letter_track > active_terminal->window.y + 25){
if(track_y <= active_terminal->window.y + 25){
cur_clear();
track_x -= 14;
ad_ten -= 14;
track_y = (active_terminal->window.width+active_terminal->window.y)-4;
cur_clear();
}
active_terminal->position--;
track_y -= 10;
track_y1 -= 10;
cur_clear_b();
term_input(track_x, track_y, ' ', TEXT_W, TEXT_B);
active_terminal->input_buffer[active_terminal->position] = 0;
cur();
last_key = 0;
letter_track -= 10;
}
}
}


else if(point_maxim.maxim_flag == 0 && last_key == '\b' && letter_track > 25 && term_win_active == true){
position--;
track_y -= 10;
term_input(track_x, track_y, ' ', TEXT_W, TEXT_B);
cur();
last_key = 0;
letter_track -= 10;
}

else if(last_key == DOWN && term_win_active == true){
if(active_terminal->arrow_count > 0){
active_terminal->arrow_count--;
}

while(track_y > active_terminal->window.y+25){
track_y -= 10;
letter_track -= 10;
term_input(track_x, track_y, ' ', TEXT_W, TEXT_B);
cur_clear_b();
}
if(active_terminal->arrow_count == 0){
active_terminal->input_buffer[0] = 0;
active_terminal->position = 0;
cur();
}
else{
int i;
active_terminal->browse = (history_count - active_terminal->arrow_count) % 10;
for(i = 0; history_store[active_terminal->browse][i] != 0; i++){
active_terminal->input_buffer[i] = history_store[active_terminal->browse][i];
term_input(track_x, track_y, active_terminal->input_buffer[i], TEXT_W, TEXT_B);
cur_clear();
track_y += 10;
letter_track += 10;
}
active_terminal->position = i;
}
last_key = 0;
}

else if(last_key == UP &&  term_win_active == true){
    if(active_terminal->arrow_count < 10 && active_terminal->arrow_count < history_count){
        active_terminal->arrow_count++;
    }

    // clear current line
    while(track_y > active_terminal->window.y+25){
        track_y -= 10;
        letter_track -= 10;
        term_input(track_x, track_y, ' ', TEXT_W, TEXT_B);
        cur_clear_b();
    }

    // get the right slot
    active_terminal->browse = (history_count - active_terminal->arrow_count) % 10;

    // load into buffer and print
    int i = 0;
    while(history_store[active_terminal->browse][i] != 0){
        active_terminal->input_buffer[i] = history_store[active_terminal->browse][i];
        term_input(track_x, track_y, active_terminal->input_buffer[i], TEXT_W, TEXT_B);
        cur_clear();
        i++;
        track_y += 10;
        letter_track += 10;
    }
    active_terminal->position = i;
    last_key = 0;
}

else if(last_key == '\n'){
active_terminal->input_buffer[active_terminal->position] = 0;
if(active_terminal->position > 0){
int i;
for(i = 0; active_terminal->input_buffer[i] != 0; i++){
history_store[history_count % 10][i] = active_terminal->input_buffer[i];
}

history_store[history_count % 10][i] = 0;

history_count++;
active_terminal->arrow_count = 0;
}
cur_clear();
shell(active_terminal->input_buffer);
if(point_maxim.maxim_flag == 1 && term_win_active == true){
term_newline();
cur();
}
else if(point_maxim.maxim_flag == 0 && term_win_active == true){
term_newline_max();
}
active_terminal->position = 0;

last_key = 0;
}

else if(last_key == 'W'){
int slot_free = window_slots();
if(slot_free != -1){
active_terminal->window.focused = false;
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
cur();
}
}

else if(last_key == 'C'){
if(active_terminal != 0 && active_terminal->window.focused == true){
if(slot_counter == 2){
slot_counter = 0;
}

int slot_used = window_slots_used();

active_terminal->window.focused = false;
cur_clear();
init_term_N_focused(&term_win[slot_used]);
active_terminal->window.focused = true;

}
}

else if(last_key == 'P'){
terminal_max(0, 0, SCREEN_W, MAXIMUM, TERM_TITLE, "Jan Terminal");
point_maxim.maxim_flag = 0;
ad_ten = 0;
track_x = 35;
track_y = 25;
letter_track = 25;
}


else if(last_key == 'X'){
if(point_maxim.maxim_flag == 0){
//terminal_close(0, 0, SCREEN_W, MAXIMUM, TERM_TITLE, "Jan Terminal");
terminal_close();
flush();
point_maxim.maxim_flag = 1;
}

else if(point_maxim.maxim_flag == 1){
if(term_win[0].check == true && term_win[1].check == false){
task_active = false;
//draw_task_bar();
//terminal_close(30, 120, TERM_Y, TERM_X, TERM_TITLE, "Jan Terminal");
terminal_close();
j_free(term_win[0].window.buffer);
term_win[0].check = false;
//icon(90, 120);
point_maxim.maxim_flag = 0;
term_win_active = false;
}

else if(term_win[0].check == true && term_win[1].check == true){
task_active = false;
//draw_task_bar();
//terminal_close(30, 120, TERM_Y, TERM_X, TERM_TITLE, "Jan Terminal");
terminal_close();
j_free(term_win[1].window.buffer);
//icon(90, 120);

}

}
}

else if(last_key != 0){
if(point_maxim.maxim_flag == 1 && term_win_active == true){
 if(track_y >= (active_terminal->window.y+active_terminal->window.width)-10){
cur_clear();
term_newline_xpromt();
}
active_terminal->input_buffer[active_terminal->position] = last_key;
cur_clear();
term_input(track_x,track_y, last_key,TEXT_W,TEXT_B);
track_y += 10;
track_y1 += 10;
cur();
active_terminal->position++;
letter_track += 10;
}

else if(point_maxim.maxim_flag == 0 && term_win_active == true){
if(track_y >= 1024){
term_newline_max();
position = 0;
}
buffer[position] = last_key;
draw_char(track_x,track_y, last_key,TEXT_W,TEXT_B);
track_y += 10;
position++;
letter_track += 10;
}
}
last_key = 0;
}
}
}
