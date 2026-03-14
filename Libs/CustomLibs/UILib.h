#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "./Utils.h"

#ifndef UILib_H
#define UILib_H

namespace UILib{
    extern const int kMaxTextLength;

    enum ItemType{
        BUTTON,
        TEXT_INPUT,
        CHECKBOX
    };

    //UI Text structure (only aesthetic related values, not position)
    struct Text{
        Utils::Color color;
        char* text = nullptr;
        float font_size;
    };

    //UI Button structure
    struct Button{
        Utils::Collider collider;
        Utils::Color color;
        Text button_text;
        bool is_visible;
        void (*action)();
    };

    //UI TextInput structure
    struct TextInput{
        Utils::Collider input_box;
        Text input_text;
        bool is_visible;
        bool is_selected;
    };

    //UI Checkbox structure
    struct Checkbox{
        Utils::Collider input_box;
        bool is_checked;
        bool is_visible;
        bool is_selected;
    };

    struct UI_Item{
        ItemType item_type;
        union {
            Button btn_item;
            TextInput text_item;
            Checkbox chk_item;
        };
    };

    //Executes the function used as parameter.
    void LaunchAction(void (*action)());

    void ButtonUpdate(Button *buttons, int total_btns);
    
    //Given an int value, it will draw it 
    //on screen as a text 
    void DrawIntText(float x, float y, int value);

    void DrawText(float x, float y, Text text);

    void DrawButton(Button b);
}

#endif

