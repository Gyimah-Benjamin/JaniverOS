#include "win.h"

void calc(char *content, int size){
    int x = 0;
    char num_1[5];
    char num_2[5];
    int num1_count = 0;
    int num2_count = 0;
    int total = 0;
    int total_1 = 0;
    int final_val = 0;
    bool cyc = false;
    char opera = 0;
    int dig[4];
    int dig_count = 0;
    for (x = 0; content[x] != '\0'; x++) {
     if(content[x] != '0' && content[x] != '1' && content[x] != '2' && content[x] != '3' && content[x] != '4' && content[x] != '5' && content[x] != '6' &&
        content[x] != '7' && content[x] != '8' && content[x] != '9' && content[x] != '+' && content[x] != '-' && content[x] != '/' && content[x] != '*'){
         continue;

}

        if(content[x] == '+' || content[x] == '/' ||
                 content[x] == '*' || content[x] == '-') {
            opera = content[x];
            cyc = true;
        }

        else if(!cyc) {
            num_1[num1_count++] = content[x];
        }

      else {
            num_2[num2_count++] = content[x];

        }

    }
    num_1[num1_count] = '\0';
    num_2[num2_count] = '\0';
    if(num1_count == 0 || num2_count == 0){
     string(track_x, track_y1, "Invalid Operation", RED, TEXT_B);
    return;
}

    if (opera == '+') {
       for(int i = 0; num_1[i] != '\0'; i++){
        total = (total*10) + (num_1[i] - '0');
        }

       for(int n = 0; num_2[n] != '\0'; n++){
        total_1 = (total_1*10) + (num_2[n] - '0');
        }

    final_val = total + total_1;

    if(final_val == 0){
    dig[dig_count++] = 0;
}

while(final_val > 0){
dig[dig_count++] = final_val%10;
final_val = final_val/10;
}
    }

    else if (opera == '/') {
        for(int i = 0; num_1[i] != '\0'; i++){
        total = (total*10) + (num_1[i] - '0');
        }

       for(int n = 0; num_2[n] != '\0'; n++){
        total_1 = (total_1*10) + (num_2[n] - '0');
        }

    final_val = total / total_1;

    if(final_val == 0){
    dig[dig_count++] = 0;
}

while(final_val > 0){
dig[dig_count++] = final_val%10;
final_val = final_val/10;
}
    }


    else if (opera == '*') {
        for(int i = 0; num_1[i] != '\0'; i++){
        total = (total*10) + (num_1[i] - '0');
        }

       for(int n = 0; num_2[n] != '\0'; n++){
        total_1 = (total_1*10) + (num_2[n] - '0');
        }

    final_val = total * total_1;

    if(final_val == 0){
    dig[dig_count++] = 0;
}

while(final_val > 0){
dig[dig_count++] = final_val%10;
final_val = final_val/10;
}
    }

    else if (opera == '-') {
        for(int i = 0; num_1[i] != '\0'; i++){
        total = (total*10) + (num_1[i] - '0');
        }

       for(int n = 0; num_2[n] != '\0'; n++){
        total_1 = (total_1*10) + (num_2[n] - '0');
        }

    final_val = total - total_1;

    if(final_val == 0){
    dig[dig_count++] = 0;
}

while(final_val > 0){
dig[dig_count++] = final_val%10;
final_val = final_val/10;
}
    }


    for(int y = dig_count - 1; y >= 0; y--){
    term_input(track_x, track_y1, dig[y] + '0', TEXT_W, TEXT_B);
    track_y1 += 10;
}
}
