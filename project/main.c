#include <stdio.h>
#include <stdlib.h>
#include "color.h"

int main()
{
    double em[8];
    const char* file = "emotions.txt";
    int emo = emotion (em, file);
    emoColor(emo);
    return 0;
}


