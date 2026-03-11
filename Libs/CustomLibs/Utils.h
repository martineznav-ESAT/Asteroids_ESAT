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
    extern const int base_font_size;

    extern double current_time,last_time;
    // Genera nueva semilla en base al 
    // ms del reloj del sistema
    void GenerateRandomSeed();

    //Genera un número del 0 al límite indicado
    int GenerateRandomNumber(int limite);

    int GetPointerIndex(int row, int col_t, int col);

    int GetPointerRowFromIndex(int i, int col_t);

    int GetPointerColFromIndex(int i, int col_t);

    void ControlFps();

    void DrawIntText(float x, float y, int value);
}

#endif

