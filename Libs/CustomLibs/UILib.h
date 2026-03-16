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
        Utils::Color border_color;
        Utils::Color fill_color;
        Text button_text;
        bool is_visible;
        void (*action)();
        bool is_selected;
    };

    //UI TextInput structure
    struct TextInput{
        Utils::Collider input_box;
        Utils::Color border_color;
        Utils::Color fill_color;
        Text input_text;
        bool is_visible;
        bool is_selected;
        float blink_timer = 0.0f;
        char* pointer = nullptr;
        bool is_pointer_v = false;
    };

    //UI Checkbox structure
    struct Checkbox{
        Utils::Collider input_box;
        Utils::Color border_color;
        Utils::Color fill_color;
        bool is_checked;
        bool is_visible;
        bool is_selected;
    };

    union ItemSelection{
        Button btn_item;
        TextInput text_item;
        Checkbox chk_item;
    };

    //UI Item structure. Useful to have a single memory block with different UI elements
    struct UI_Item{
        ItemType item_type;
        Text item_name;
        ItemSelection item;
    };

    void InitItem(UI_Item *ui_item, ItemType it, Text name);

    void UpdateItem(UI_Item *ui_item, int *selected_i, int item_index);
    
    void DrawItem(UI_Item ui_item);

    //Executes the function used as parameter.
    void LaunchAction(void (*action)());
    
    //Given a button as parameter, fills it with the rest of the parameters. Created mainly for readability
    void InitButton(Button *b, Utils::Collider coll, Utils::Color border_color, Utils::Color fill_color, Text b_text, bool is_visible, void (*action)());
    
    //Given a button, it gets checked to manage workability
    void UpdateButton(Button *button);

    //Draws on screen the button given as parameter
    void DrawButton(Button b);

    //Given a TextInput as parameter, fills it with the rest of the parameters. Created mainly for readability
    void InitTextInput(TextInput *ti, Utils::Collider coll, Utils::Color border_color, Utils::Color fill_color, Text ti_text, bool is_visible);

    //Given a TextInput, it gets checked to manage workability
    void UpdateTextInput(TextInput *ti);

    //Draws on screen the TextInput given as parameter
    void DrawTextInput(TextInput ti);

    //Draws a text given a position and Text structure 
    void DrawText(float x, float y, Text text);

    //Given an int value, it will draw it 
    //on screen as a text 
    void DrawIntText(float x, float y, int value);


    void EmptyItemMemory(UI_Item *item);

}

#endif

