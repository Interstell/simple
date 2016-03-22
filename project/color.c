#include <stdio.h>
#include "color.h"
#include <windows.h>

int maxEmotion (double em[8]){
    int i, maxIndex = 0;
    for (i = 0; i < 8; i++){
        if (em[i] > em[maxIndex]){
            maxIndex = i;
        }
    }
    return maxIndex;
}

int emotion (double em[8], const char * file){
    FILE* fp = NULL;
    int i = 0;
    fp = fopen (file, "r");
    if (fp == NULL){
        puts("Error, file wasn`t opened");
        return 1;
    }
    if (fgetc(fp) == '#'){
        printf("Error, there`s no faces on photo");
        return 1;
    }
    rewind(fp);
    for (i = 0; i < 8; i++){ // possible error
    fscanf(fp, "%lf", &em[i]);
    }
    int emotion = maxEmotion(em);
    fclose(fp);
    return emotion;
}


int colorDefine(int emotion){
    int fmt = 0;
    switch(emotion){
    case ANGER:
        fmt = BACKGROUND_RED;
        break;
    case CONTEMPT:
        fmt = BACKGROUND_RED | BACKGROUND_INTENSITY;
        break;
    case DISGUST:
        fmt = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
        break;
    case FEAR:
        fmt = BACKGROUND_RED | BACKGROUND_BLUE |  BACKGROUND_INTENSITY;
        break;
    case HAPPINESS:
        fmt = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
        break;
    case NEUTRAL:
        fmt = BACKGROUND_RED | BACKGROUND_GREEN;
        break;
    case SADNESS:
        fmt = BACKGROUND_BLUE;
        break;
    case SURPRISE:
        fmt = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
        break;
    default:
        printf("error");
        break;
    }
    return fmt;
}

void emoColor(int emotion){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    int i, j;
    pos.X = 0;
    pos.Y = 0;
    SetConsoleTextAttribute(hConsole, colorDefine(emotion));
    for (i = 0; i < 25; i++){
        for (j = 0; j < 80; j++){
            pos.X = j;
            pos.Y = i;
            SetConsoleCursorPosition(hConsole, pos);
            printf(" ");
        }
    }
}
