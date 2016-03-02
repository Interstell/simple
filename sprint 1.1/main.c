#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

enum emotion { HAPPY = 0,
SAD,
ANGRY,
DISGUST,
FEAR,
};

void emoColor(int emotion);
void colorDefine(HANDLE hConsole, int emotion);

int main()
{
    char c[15];
    char* str;
    int emotion;
    printf("What do you feel right now?\n");
    printf("Happiness, Sadness, Angry, Disgust or Fear?\n");
    str = gets(c);
    if (strcpy(str, "Happiness") == 0)
    emotion = HAPPY;
    if (strcpy(str, "Sadness") == 0)
    emotion = SAD;
    if (strcpy(str, "Angry") == 0)
    emotion = ANGRY;
    if (strcpy(str, "Disgust") == 0)
    emotion = DISGUST;
    if (strcpy(str, "Fear") == 0)
    emotion = FEAR;
    emoColor(emotion);


    return 0;
}

void emoColor(int emotion){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    int i, j;
    pos.X = 0;
    pos.Y = 0;
    for (i = 0; i < 25; i++){
        for (j = 0; j < 80; j++){
            colorDefine(hConsole, emotion);
            printf(" ");
        }
    }
}

void colorDefine(HANDLE hConsole, int emotion){
    int fmt;
    switch(emotion){
    case HAPPY:
        fmt = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
        break;
    case SAD:
        fmt = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
        break;
    case ANGRY:
        fmt = BACKGROUND_RED | BACKGROUND_INTENSITY;
        break;
    case DISGUST:
        fmt = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
        break;
    case FEAR:
        fmt = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
    }
    SetConsoleTextAttribute(hConsole, fmt);
}
