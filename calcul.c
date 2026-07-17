#include "win.h"
#include "calcul.h"

void add(char *ptr){
int real_val;
int real_val1;
int i;
for(i = 0; ptr[i] != '\0'; i++){
if(ptr[i] == '+'){
i++;
int val1 = ptr[i];
i++;
int val2 = ptr[i];
i++;
int real_val = (val1*10)+val2;
}
int val3 = ptr[i];
i++;
int val4 = ptr[i];
i++;
int real_val1 = (val3*10)+val4;
}

int total = real_val + real_val1;
char tot[2];
tot[0] = total;
tot[1] = '\0';
string(track_x, track_y1, tot, DESK_BG, TEXT_B);
}

void calc(char *content, int size){
int opera = 0;
int result = 0;
int result_1 = 0;
int i = 0;
char temp_num[6];

while(i < size){
if(content[i] == '*' || content[i] == '/' || content[i] == '+' || content[i] == '-'){
temp_num[i] = content[i];

i++;
for(i = i; content[i] != '\0'; i++){
temp_num[i] = content[i] - '0';
}
temp_num[i] = '\0';
for(i = 0; temp_num[i] != '\0'; i++){
if(temp_num[i] == '+'){
add(temp_num);
}
}
return;
}
for(i = 0; content[i] != '*' || content[i] != '/' || content[i] != '+' || content[i] != '-'; i++){
temp_num[i] = content[i] - '0';
}

}
}
