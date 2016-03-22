#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include <windows.h>



int main()
{
    double em[8];
    system("C:\\Files\\Institute\\2semester\\PPPI\\test\\testUrl.bat");
    const char* file = "C:\\Files\\Institute\\2semester\\PPPI\\test\\emotions.txt";
    int emo = emotion (em, file);
    emoColor(emo);
    return 0;
}


