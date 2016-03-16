#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

enum emotion {
    ANGER = 0,
    CONTEMPT,
    DISGUST,
    FEAR,
    HAPPINESS,
    NEUTRAL,
    SADNESS,
    SURPRISE,
};

int emotion (double em[8], const char * file);
void emoColor(int emotion);
//int maxEmotion (double em[8]);

#endif // COLOR_H_INCLUDED
