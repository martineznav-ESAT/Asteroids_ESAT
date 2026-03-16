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
    const int kMaxTextLength = 20;

    //UI_ITEM
    void InitItem(UI_Item* ui_item, ItemType it, Text name){
        *ui_item = {
            it,
            name,
            NULL,
            false
        };
    }

    //Updates the item based on its type, its item index and which is the one actually selected
    void UpdateItem(UI_Item *ui_item, int *selected_i, int item_index){

        switch (ui_item->item_type){
            case UILib::ItemType::BUTTON:
                if(*selected_i == item_index){
                    ui_item->item.btn_item.is_selected = true;
                }else{
                    ui_item->item.btn_item.is_selected = false;
                }
                UILib::UpdateButton(&(ui_item->item.btn_item));
            break;
            case UILib::ItemType::TEXT_INPUT:
                if(Utils::MouseInCollider(ui_item->item.text_item.input_box) && esat::MouseButtonDown(0)){
                    *selected_i = item_index;
                }
                
                if(*selected_i == item_index){
                    ui_item->item.text_item.is_selected  = true;
                }else{
                    ui_item->item.text_item.is_selected  = false;
                }
                UILib::UpdateTextInput(&(ui_item->item.text_item));
            break;
        }
    }
    
    void DrawItem(UI_Item ui_item){
        Utils::Collider left_side = {{(Utils::kWindowWidth*0.5f)- 40 - (Utils::kBaseFontSize*14), 200}, {(Utils::kWindowWidth*0.5f)-40, 250}};
        Utils::Collider aux;

        switch (ui_item.item_type){
            case UILib::ItemType::BUTTON:
                UILib::DrawButton(ui_item.item.btn_item);
            break;
            case UILib::ItemType::TEXT_INPUT:
                aux = ui_item.item.text_item.input_box;

                //Draws Tag Text for the actual Input
                UILib::DrawText(
                    left_side.P2.x - (strlen(ui_item.item_name.text)*0.55*ui_item.item_name.font_size), 
                    aux.P2.y - ((aux.P2.y - aux.P1.y) * 0.5) + (ui_item.item.text_item.input_text.font_size * 0.6f), 
                    ui_item.item_name
                );
                UILib::DrawTextInput(ui_item.item.text_item);
            break;
        }
    }

    //BUTTON

    //Executes the given function
    //Gives a print in case its given null
    //Mainly thought for buttons or similar action-like UI elements and keyboard inputs
    void LaunchAction(void (*action)()){
        if(action != nullptr){
            action();
        }else{
            printf("Action WIP\n");
        }
    }

    //Changes button color in case the mouse is hovering on it
    //and grants acces to click the button if that's the case
    void OnButtonHover(Button *b){
        if(b->is_visible && Utils::MouseInCollider(b->collider)){
            //OnHover
            b->border_color.a = 255;
            b->fill_color.a = 255;
            if(esat::MouseButtonDown(0)){
                //OnClick
                LaunchAction(b->action);
            }
        }else{
            b->border_color.a = 200;
            b->fill_color.a = 200;
        }
    }

    //Given a button as parameter, fills it with the rest of the parameters. Created mainly for readability
    void InitButton(UILib::Button *b, Utils::Collider coll, Utils::Color border_color, Utils::Color fill_color, UILib::Text b_text, bool is_visible, void (*action)()){
        *b = {
            coll,
            border_color,
            fill_color,
            b_text,
            is_visible,
            action,
            false
        };
    }

    //Given a button, it gets checked to manage workability
    void UpdateButton(Button *button){
        if(button->is_visible){
            OnButtonHover(button);
            if(button->is_selected){
                button->border_color.a = 255;
                button->fill_color.a = 255;
                if(esat::IsSpecialKeyDown(esat::kSpecialKey_Enter)){
                    LaunchAction(button->action);
                }
            }
        }
    }

    //Draws on screen the button given as parameter
    void DrawButton(Button b){
        if(b.is_visible){
            Utils::DrawCollider(b.collider, b.border_color, b.fill_color);

            //In case the button has a text, it's drawn centered to the button
            if(b.button_text.text != nullptr){
                UILib::DrawText(
                    b.collider.P1.x + ((b.collider.P2.x-b.collider.P1.x) * 0.5f) - (strlen(b.button_text.text) * b.button_text.font_size * 0.33 ), //Multiplied by 0.33 because it is the scale needed to be centered based con the custom font. The reasonable multiplication should be 0.5 (/2)
                    b.collider.P2.y - ((b.collider.P2.y-b.collider.P1.y) * 0.5f) + (b.button_text.font_size*0.5f), 
                    b.button_text
                );
            }
        }
    }



    //TEXT_INPUT

    //Given a button as parameter, fills it with the rest of the parameters. Created mainly for readability
    void InitTextInput(UILib::TextInput *ti, Utils::Collider coll, Utils::Color border_color, Utils::Color fill_color, UILib::Text ti_text, bool is_visible){
        //The char memory block has 1 extra space for the blinking effect to work when selected 
        ti_text.text = (char*) malloc(sizeof(char) * (kMaxTextLength+1));
        
        *ti = {
            coll,
            border_color,
            fill_color,
            ti_text,
            is_visible,
            false,
            0.0f,
            nullptr,
            false
        };

        ti->pointer = (char*) malloc(sizeof(char) * (kMaxTextLength+2));
        *(ti->pointer) = '|'; 
        *(ti->pointer+1) = '\0'; 
    }


    //Given a string, manages basic input. Mainly used for TextInputs
    bool CharInput(char *string){
        bool is_input = false;
        char input_c;
        int old_length;

        input_c = esat::GetNextPressedKey();

        if(strlen(string) < kMaxTextLength && input_c != 0){
            old_length = strlen(string);
            *(string+old_length) = input_c;
            *(string+old_length+1) = '\0';
            is_input = true;
        }else{
            if(strlen(string) > 0 && esat::IsSpecialKeyDown(esat::kSpecialKey_Backspace)){
                *(string+strlen(string)-1) = '\0';
                is_input = true;
            }
        }

        return is_input;
    }


    void BlinkPointer(TextInput *ti){
        if(Utils::current_time-(ti->blink_timer) >= 500){
            ti->blink_timer = esat::Time();
            ti->is_pointer_v = !(ti->is_pointer_v);
        }
    }

    void UpdateTextInputPointer(TextInput *ti){
        for(int i = 0; i < strlen(ti->input_text.text)+2;i++){
            if(i>=strlen(ti->input_text.text)){
                if(i==strlen(ti->input_text.text)){
                    *(ti->pointer+i) = '|';
                }else{
                    *(ti->pointer+i) = '\0';
                }
            }else{
                *(ti->pointer+i) = ' ';
            }
        }
    }

    //Given a Text Input, it gets checked to manage input workability
    void UpdateTextInput(TextInput *ti){
        if(ti->is_selected){
            ti->border_color.a = 255;

            if(CharInput(ti->input_text.text)){
                UpdateTextInputPointer(ti);
            }

            BlinkPointer(ti);
        }else{
            ti->border_color.a = 200;
        }
    }

    //Draws on screen the TextInput given as parameter
    void DrawTextInput(TextInput ti){
        if(ti.is_visible){

            Utils::DrawCollider(ti.input_box, ti.border_color, ti.fill_color);

            if(ti.input_text.text != nullptr){
                //Draws the text centered vertically inside de input box and aligned to the left horizontaly
                UILib::DrawText(
                    ti.input_box.P1.x + ti.input_text.font_size*0.66f,
                    ti.input_box.P2.y - ((ti.input_box.P2.y - ti.input_box.P1.y) * 0.5) + (ti.input_text.font_size * 0.5f), 
                    ti.input_text
                );
                if(ti.is_selected && ti.is_pointer_v){
                    esat::DrawText(
                        ti.input_box.P1.x + ti.input_text.font_size*0.66f,
                        ti.input_box.P2.y - ((ti.input_box.P2.y - ti.input_box.P1.y) * 0.5) + (ti.input_text.font_size * 0.5f), 
                        ti.pointer
                    );
                }
            }
        }
    }

    //TEXT

    //Draws a text given a position and Text structure 
    void DrawText(float x, float y, Text text){
        esat::DrawSetFillColor(text.color.r, text.color.g, text.color.b, text.color.a);
        esat::DrawSetTextSize(text.font_size);
        esat::DrawText(x, y, text.text);
    }

    //Draws the int value given as text on designed position 
    void DrawIntText(float x, float y, int value){
        char* text = (char*) malloc(sizeof(char)*3);
        text = itoa(value,text,10);
        esat::DrawText(x, y, text);
        free(text);
    }

    void EmptyItemMemory(UI_Item *item){
        free(item->item_name.text);
        switch (item->item_type){
            case UILib::ItemType::BUTTON:
                free(item->item.btn_item.button_text.text);
            break;
            case UILib::ItemType::TEXT_INPUT:
                free(item->item.text_item.input_text.text);
                free(item->item.text_item.pointer);
            break;
            case UILib::ItemType::CHECKBOX:
                
            break;
        }
    }
}