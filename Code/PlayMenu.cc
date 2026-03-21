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

#include "../Libs/CustomLibs/Utils.h"
#include "../Libs/CustomLibs/UILib.h"

#include "./GameManager.h"
#include "./PlayMenu.h"
#include "./LoginMenu.h"

namespace PlayMenu{
    //Memory block that holds all the menu items no matter if they are visible or not.
    UILib::UI_Item *menu_items = nullptr;
    int selected_item = -1;


    //ACTIONS
    void SinglePlayerAction(){
        printf("SINGLE_P ACTION WIP\n");
    }

    void MultiPlayerAction(){
        LoginMenu::Load(GameManager::Level::PLAY_MENU);
    }

    //PLAY MENU INIT

    //Initializes all menu buttons
    void InitButtons(){
        JMATH::Vec2 centered_coord = {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f};
        JMATH::Vec2 margin_y = {0.0f, 50.0f};

        Utils::Collider base_collider = {{centered_coord.x, centered_coord.y-(float)Utils::kBaseFontSize},{centered_coord.x, (centered_coord.y+(float)Utils::kBaseFontSize)}};

        Utils::Color button_color = {0,0,0,255};
        Utils::Color text_color = {255,255,255,255};
        float font_width_aux = ((float)Utils::kBaseFontSize*2)/3.5f;

        UILib::InitButton(
            &((menu_items+((int)PlayMenuItems::SINGLEPLAYER_BTN))->item.btn_item),
            {
                JMATH::Vec2Sub(
                    {base_collider.P1.x-(strlen((menu_items+((int)PlayMenuItems::SINGLEPLAYER_BTN))->item_name.text)*((float)Utils::kBaseFontSize*2)/3.5f), base_collider.P1.y},
                    margin_y
                ),
                JMATH::Vec2Sub(
                    {base_collider.P2.x+(strlen((menu_items+((int)PlayMenuItems::SINGLEPLAYER_BTN))->item_name.text)*((float)Utils::kBaseFontSize*2)/3.5f), base_collider.P2.y},
                    margin_y
                )
            },
            button_color,
            button_color,
            {
                text_color,
                (menu_items+((int)PlayMenuItems::SINGLEPLAYER_BTN))->item_name.text,
                (float)Utils::kBaseFontSize*2
            },
            true,
            SinglePlayerAction
        );

        UILib::InitButton(
            &((menu_items+((int)PlayMenuItems::MULTIPLAYER_BTN))->item.btn_item),
            {
                JMATH::Vec2Sum(
                    {base_collider.P1.x-(strlen((menu_items+((int)PlayMenuItems::MULTIPLAYER_BTN))->item_name.text)*((float)Utils::kBaseFontSize*2)/3.5f), base_collider.P1.y},
                    margin_y
                ),
                JMATH::Vec2Sum(
                    {base_collider.P2.x+(strlen((menu_items+((int)PlayMenuItems::MULTIPLAYER_BTN))->item_name.text)*((float)Utils::kBaseFontSize*2)/3.5f), base_collider.P2.y},
                    margin_y
                )
            },
            button_color,
            button_color,
            {
                text_color,
                (menu_items+((int)PlayMenuItems::MULTIPLAYER_BTN))->item_name.text,
                (float)Utils::kBaseFontSize*2
            },
            true,
            MultiPlayerAction
        );

    }


    //Initializes all UI items
    void InitMenuItems(){

        // text_inputs = (UILib::TextInput*) malloc(sizeof(UILib::TextInput)*(int)PlayMenuItems::TOTAL_ITEMS);
        menu_items = (UILib::UI_Item*) malloc(sizeof(UILib::UI_Item)*(int)PlayMenuItems::TOTAL_ITEMS);

        UILib::InitItem(
            (menu_items + ((int)PlayMenuItems::SINGLEPLAYER_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"SINGLEPLAYER", Utils::kBaseFontSize*2.0f}
        );
        UILib::InitItem(
            (menu_items + ((int)PlayMenuItems::MULTIPLAYER_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"MULTIPLAYER", Utils::kBaseFontSize*2.0f}
        );
    }

    //Whole Play Menu initializer
    void Init(){
        InitMenuItems();
        InitButtons();
    }

    //PLAY MENU LOAD

    //Loads the play menu
    void Load(){
        GameManager::game_status.level = GameManager::Level::PLAY_MENU;
    }

    //PLAY MENU UPDATE

    //Whole Play Menu update method
    void Update(){

        //Unselect Menu item on click
        //Since its the first click event registered, there's no need to check for collision
        //If later on a collision with the same input its located, the selected_item will be 
        //overriden to the proper value
        if(esat::MouseButtonDown(0)){
            selected_item = -1;
        }

        //Menu Key controls
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_Up)){
            if(selected_item <= 0){
                selected_item = ((int)PlayMenuItems::TOTAL_ITEMS) - 1;
            }else{
                selected_item--;
            }
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_Down) || esat::IsSpecialKeyDown(esat::kSpecialKey_Tab)){
            ++selected_item %= (int)PlayMenuItems::TOTAL_ITEMS;
        }
        
        for(int i = 0; i < (int)PlayMenuItems::TOTAL_ITEMS; i++){
            UILib::UpdateItem(menu_items+i, &selected_item, i);
        }
    }

    //PLAY MENU DRAW
    void DrawMenuItems(){
        for(int i = 0; i < (int)PlayMenuItems::TOTAL_ITEMS; i++){
            UILib::DrawItem(*(menu_items+i));
        }
    }

    //Whole Play Menu draw method
    void Draw(){
        DrawMenuItems();
    }

    void EmptyMemory(){
        for(int i = 0; i < (int)PlayMenuItems::TOTAL_ITEMS; i++){
            UILib::EmptyItemMemory(menu_items+i);
        }
        free(menu_items);
    }
}