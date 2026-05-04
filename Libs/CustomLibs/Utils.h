#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "./JMATH.h"

#ifndef Utils_H
#define Utils_H

namespace Utils{
    struct Color{
        float r, g, b, a ;
    };

    struct Collider{
        JMATH::Vec2 P1 = {0,0};
        JMATH::Vec2 P2 = {10,10};
    };

    extern const unsigned char kFPS;
    extern const int kWindowWidth, kWindowHeight;
    extern const int kBaseFontSize;

    extern double current_time,last_time;

    //Generates a new random number seed based
    //on clock system ms 
    void GenerateRandomSeed();

    //Genera un número del 0 al límite indicado sin incluirlo
    int GenerateRandomNumber(int limite);

    //Genera un número del limite indicado al limite en negativo pasando por el 0 sin incluir el limite 
    int GenerateRandomNumberNegative(int limite);

    //Genera un número decimal (2 decimales) del 0 al límite indicado sin incluirlo
    float GenerateRandomFloat(int limite);

    //Genera un número decimal (2 decimales) del limite indicado al limite en negativo pasando por el 0 sin incluir el limite 
    float GenerateRandomFloatNegative(int limite);

    int GetPointerIndex(int row, int col_t, int col);

    int GetPointerRowFromIndex(int i, int col_t);

    int GetPointerColFromIndex(int i, int col_t);

    void ControlFps();

    //Detects if the mouse is inside the collider area
    bool MouseInCollider(Collider c);

    void DrawCollider(Collider c, Color border_color, Color fill_color);

    int FindCharIndexInString(char* string, char find_char);

    //Fills the string with the character used as parameter up to the introduced limit
    //Limit -1 implies the whole text will be filled
    void StringFillWithChar(char* string, int str_length, char character, int limit);

    //Transforms a timestamp given of time_t type into a tm structure corrected with the local timezone.
    tm TimestampToStructTM(time_t timestamp);
}

#endif

