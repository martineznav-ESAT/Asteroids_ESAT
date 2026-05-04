#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "./Utils.h"

namespace Utils{
    const unsigned char kFPS = 60;
    // const int kWindowWidth = 800, kWindowHeight = 608;
    const int kWindowWidth = 1280, kWindowHeight = 960;
    const int kBaseFontSize = 19;

    double current_time,last_time;

    // Genera nueva semilla en base al 
    // ms del reloj del sistema
    void GenerateRandomSeed(){
        srand(time(NULL)); 
    }
    
    //Genera un número del 0 al límite indicado sin incluirlo
    int GenerateRandomNumber(int limite){
        return (rand()%limite);
    }

    //Genera un número del limite indicado al limite en negativo pasando por el 0 sin incluir el limite 
    int GenerateRandomNumberNegative(int limite){
        return (GenerateRandomNumber(limite)) * (GenerateRandomNumber(2) == 0 ? -1 : 1);
    }

    //Genera un número decimal (2 decimales) del 0 al límite indicado sin incluirlo
    float GenerateRandomFloat(int limite){
        return (GenerateRandomNumber(limite*100)/100.0f);
    }

    //Genera un número decimal (2 decimales) del limite indicado al limite en negativo pasando por el 0 sin incluir el limite 
    float GenerateRandomFloatNegative(int limite){
        return (GenerateRandomFloat(limite) * (GenerateRandomNumber(2) == 0 ? -1 : 1));
    }

    int GetPointerIndex(int row, int col_t, int col){
        return ((row*col_t)+col);
    }

    int GetPointerRowFromIndex(int i, int col_t){
        return (i/col_t);
    }

    int GetPointerColFromIndex(int i, int col_t){
        return (i%col_t);
    }

    void ControlFps(){
        do{
            current_time = esat::Time();
        }while((current_time-last_time)<=1000.0/kFPS);
    }

    //Detects if the mouse is inside the collider area
    bool MouseInCollider(Collider c){
        return (
            c.P1.x < esat::MousePositionX() &&
            c.P1.y < esat::MousePositionY() &&
            c.P2.x > esat::MousePositionX() &&
            c.P2.y > esat::MousePositionY()
        );
    }

    void DrawCollider(Collider c, Color border_color, Color fill_color){
        //Generates draw coords and draws de button collider with the button values
        JMATH::Vec2 *draw_coords = (JMATH::Vec2*) malloc(sizeof(JMATH::Vec2) * 5);

        *(draw_coords+0) = c.P1;
        *(draw_coords+1) = {c.P1.x, c.P2.y};
        *(draw_coords+2) = c.P2;
        *(draw_coords+3) = {c.P2.x, c.P1.y};
        *(draw_coords+4) = c.P1;

        esat::DrawSetStrokeColor(border_color.r, border_color.g, border_color.b, border_color.a);
        esat::DrawSetFillColor(fill_color.r, fill_color.g, fill_color.b, fill_color.a);
        esat::DrawSolidPath(&(draw_coords->x), 5);

        free(draw_coords);
    }

    int FindCharIndexInString(char* string, char find_char){
        int count;
        bool is_found = false;
        for (count = 0; count < strlen(string) && !(is_found = (*(string+count) == find_char)) ; count++);
        if(!is_found){
            count = -1;
        }
        return count;
    }

    //Fills the string with the character used as parameter up to the introduced limit
    //Limit -1 implies the whole text will be filled
    void StringFillWithChar(char* string, int str_length, char character, int limit = -1){
        for(int i = 0; i < str_length && i < limit; i++){
            *(string+i) = character;
        }
    }

    //Transforms a timestamp given of time_t type into a tm structure corrected with the local timezone.
    tm TimestampToStructTM(time_t timestamp){
        char *time_buffer = (char*)malloc(26);
        tm time_struct;
        _localtime64_s(&time_struct, &timestamp); 
        asctime_s(time_buffer, 26, &time_struct);

        free(time_buffer);
        return time_struct;
    }
}