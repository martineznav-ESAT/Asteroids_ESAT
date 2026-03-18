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
            case UILib::ItemType::CHECKBOX:
                if(*selected_i == item_index){
                    ui_item->item.chk_item.is_selected = true;
                }else{
                    ui_item->item.chk_item.is_selected = false;
                }
                UILib::UpdateCheckbox(&(ui_item->item.chk_item));
            break;
        }
    }
    
    void DrawItem(UI_Item ui_item){
        switch (ui_item.item_type){
            case UILib::ItemType::BUTTON:
                UILib::DrawButton(ui_item.item.btn_item);
            break;
            case UILib::ItemType::TEXT_INPUT:
                UILib::DrawTextInput(ui_item.item.text_item, ui_item.item_name);
            break;
            case UILib::ItemType::CHECKBOX:
                UILib::DrawCheckbox(ui_item.item.chk_item, ui_item.item_name);
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
    void InitTextInput(UILib::TextInput *ti, Utils::Collider tag_box, Utils::Collider input_box, Utils::Color border_color, Utils::Color fill_color, UILib::Text ti_text, bool is_visible, bool is_tag_v, bool is_number_only, int max_length){
        //The char memory block has 1 extra space for the blinking effect to work when selected 
        ti_text.text = (char*) malloc(sizeof(char) * (max_length+1));
        *(ti_text.text) = '\0'; 
        
        *ti = {
            tag_box,
            input_box,
            border_color,
            fill_color,
            ti_text,
            is_visible,
            false,
            0.0f,
            nullptr,
            false,
            is_tag_v,
            is_number_only,
            max_length
        };

        ti->pointer = (char*) malloc(sizeof(char) * (max_length+2));
        *(ti->pointer) = '|'; 
        *(ti->pointer+1) = '\0'; 
    }


    //Given a string, manages basic input. Mainly used for TextInputs
    bool CharInput(char *string, int max_length = kMaxTextLength){
        bool is_input = false;
        char input_c;
        int old_length;

        input_c = esat::GetNextPressedKey();

        if(strlen(string) < max_length && input_c != 0){
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

    //Given a string, manages basic input limited to numbers only. Mainly used for TextInputs with only number parameter
    bool NumberInput(char *string, int max_length = kMaxTextLength){
        bool is_input = false;
        char input_c;
        int old_length;

        input_c = esat::GetNextPressedKey();
    

        if(strlen(string) < max_length && input_c != 0 && input_c >= '0' && input_c <= '9'){
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

            if(ti->is_number_only){
                if(NumberInput(ti->input_text.text, ti->max_length)){
                    UpdateTextInputPointer(ti);
                }
            }else{
                if(CharInput(ti->input_text.text, ti->max_length)){
                    UpdateTextInputPointer(ti);
                }
            }

            BlinkPointer(ti);
        }else{
            ti->border_color.a = 200;
        }
    }

    //Draws on screen the TextInput given as parameter
    void DrawTextInput(TextInput ti, Text tag){

        if(ti.is_visible){

            //Draws Tag Text for the actual Input
            if(ti.is_tag_v){
                UILib::DrawText(
                    ti.tag_box.P1.x, 
                    ti.tag_box.P2.y, 
                    tag
                );
            }

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

    //CHECKBOX

    //Given a button as parameter, fills it with the rest of the parameters. Created mainly for readability
    void InitCheckbox(Checkbox *chk, Utils::Collider tag_box, Utils::Collider collider, Utils::Color border_color, Utils::Color fill_color, Text chk_text, bool is_checked, bool is_visible, bool is_tag_v){
        *chk = {
            tag_box,
            collider,
            border_color,
            fill_color,
            chk_text,
            is_checked,
            is_visible,
            false,
            is_tag_v
        };
    }

    //Changes Checkbox color in case the mouse is hovering on it
    //and grants acces to click the Checkbox if that's the case
    void OnCheckboxHover(Checkbox *chk){
        if(chk->is_visible && Utils::MouseInCollider(chk->collider)){
            //OnHover
            chk->border_color.a = 255;
            chk->fill_color.a = 255;
            if(esat::MouseButtonDown(0)){
                //OnClick
                chk->is_checked = !(chk->is_checked);
                // if(chk->is_checked){
                //     printf("CHECKED\n");
                // }else{
                //     printf("UN-CHECKED\n");
                // }
            }
        }else{
            chk->border_color.a = 200;
            chk->fill_color.a = 200;
        }
    }

    //Given a chexkbox, it gets checked to manage workability
    void UpdateCheckbox(Checkbox *chk){
        if(chk->is_visible){
            OnCheckboxHover(chk);
            if(chk->is_selected){
                chk->border_color.a = 255;
                chk->fill_color.a = 255;
                if(esat::IsSpecialKeyDown(esat::kSpecialKey_Enter)){
                    chk->is_checked = !(chk->is_checked);
                    // if(chk->is_checked){
                    //     printf("CHECKED\n");
                    // }else{
                    //     printf("UN-CHECKED\n");
                    // }
                }
            }
        }
    }

    //Draws on screen the Checkbox given as parameter
    void DrawCheckbox(Checkbox chk, Text tag){

        if(chk.is_visible){

            //Draws Tag Text for the actual Input
            if(chk.is_tag_v){
                UILib::DrawText(
                    chk.tag_box.P1.x, 
                    chk.tag_box.P2.y, 
                    tag
                );
            }

            Utils::DrawCollider(chk.collider, chk.border_color, chk.fill_color);

            //In case the button has a text, it's drawn centered to the checkbox
            if(chk.chk_text.text != nullptr && chk.is_checked){
                UILib::DrawText(
                    chk.collider.P1.x + ((chk.collider.P2.x-chk.collider.P1.x) * 0.5f) - (strlen(chk.chk_text.text) * chk.chk_text.font_size * 0.25f ), //Multiplied by 0.33 because it is the scale needed to be centered based con the custom font. The reasonable multiplication should be 0.5 (/2)
                    chk.collider.P2.y - ((chk.collider.P2.y-chk.collider.P1.y) * 0.5f) + (chk.chk_text.font_size*0.5f), 
                    chk.chk_text
                );
            }
        }
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
                free(item->item.chk_item.chk_text.text);
            break;
        }
    }
}