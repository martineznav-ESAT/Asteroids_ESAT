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
#include "./UILib.h"
#include "../../Code/GameStatus.h"

namespace UILib{
    //BUTTONS 

    void LaunchAction(void (*action)()){
        if(action != nullptr){
            action();
        }else{
            printf("Action WIP\n");
        }
    }

    bool MouseInButton(Button b){
        return (
            b.collider.P1.x < esat::MousePositionX() &&
            b.collider.P1.y < esat::MousePositionY() &&
            b.collider.P2.x > esat::MousePositionX() &&
            b.collider.P2.y > esat::MousePositionY()
        );
    }

    void OnButtonHover(Button *b){
        if(b->is_visible && MouseInButton(*b)){
            //OnHover
            b->color.a = 255;
            if(esat::MouseButtonDown(0)){
                //OnClick
                LaunchAction(b->action);
            }
        }else{
            b->color.a = 230;
        }
    }


    void ButtonUpdate(Button *buttons, int total_btns){
        for(int i = 0; i < total_btns; i++){
            OnButtonHover((buttons+i));
        }
    }

    void DrawText(float x, float y, Text text){
        esat::DrawSetFillColor(text.color.r, text.color.g, text.color.b, text.color.a);
        esat::DrawSetTextSize(text.font_size);
        esat::DrawText(x, y, text.text);
    }

    void DrawIntText(float x, float y, int value){
        char* text = (char*) malloc(sizeof(char)*3);
        text = itoa(value,text,10);
        esat::DrawText(x, y, text);
        free(text);
    }

    //Draws the button given as parameter
    void DrawButton(Button b){
        if(b.is_visible){
            //Generates draw coords and draws de button collider with the button values
            JMATH::Vec2 *draw_coords = (JMATH::Vec2*) malloc(sizeof(JMATH::Vec2) * 5);

            *(draw_coords+0) = b.collider.P1;
            *(draw_coords+1) = {b.collider.P1.x, b.collider.P2.y};
            *(draw_coords+2) = b.collider.P2;
            *(draw_coords+3) = {b.collider.P2.x, b.collider.P1.y};
            *(draw_coords+4) = b.collider.P1;

            esat::DrawSetStrokeColor(b.color.r, b.color.g, b.color.b, b.color.a);
            esat::DrawSetFillColor(b.color.r, b.color.g, b.color.b, b.color.a);
            esat::DrawSolidPath(&(draw_coords->x), 5);
            free(draw_coords);

            //In case the button has a text, it's drawn centered to the button
            if(b.button_text.text != nullptr){
                UILib::DrawText(
                    b.collider.P1.x + ((b.collider.P2.x-b.collider.P1.x) * 0.5f) - (strlen(b.button_text.text) * b.button_text.font_size*0.5f),
                    b.collider.P2.y - ((b.collider.P2.y-b.collider.P1.y) * 0.5f) + (b.button_text.font_size*0.5f), 
                    b.button_text
                );
            }
        }
    }
}