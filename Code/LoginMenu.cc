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
#include "./LoginMenu.h"
#include "./RegisterMenu.h"
#include "./MainMenu.h"
#include "./PlayMenu.h"

namespace LoginMenu{
    //Memory block that holds all the menu items no matter if they are visible or not.
    UILib::UI_Item *menu_items = nullptr;
    GameManager::Level prev_level;
    int selected_item = -1;


    //ACTIONS
    void LoginAction(){
        switch(prev_level){
            case GameManager::Level::PLAY_MENU:
                //TO_DO
                // Game::Load(Multiplayer);
            break;
            default:
                MainMenu::Load();
            break;
        }
    }

    void RegisterAction(){
        RegisterMenu::Load(GameManager::Level::LOGIN_MENU);
    }

    void BackAction(){
        switch(prev_level){
            case GameManager::Level::MAIN_MENU:
                MainMenu::Load();
            break;
            case GameManager::Level::PLAY_MENU:
                PlayMenu::Load();
            break;
        }
    }

    //LOGIN MENU INIT

    //Initializes all menu buttons
    void InitButtons(){
        JMATH::Vec2 centered_bottom_coords = {Utils::kWindowWidth*0.5f, Utils::kWindowHeight-50.0f};
        Utils::Collider left_button = 
        {
            {centered_bottom_coords.x-150-50, centered_bottom_coords.y-50},
            {centered_bottom_coords.x-50, centered_bottom_coords.y},
        };
        Utils::Collider right_button = 
        {
            {centered_bottom_coords.x+50, centered_bottom_coords.y-50},
            {centered_bottom_coords.x+50+150, centered_bottom_coords.y},
        };

        Utils::Color button_color = {100,100,100,200};
        Utils::Color text_color = {255,255,255,255};

        UILib::InitButton(
            &((menu_items+((int)LoginItems::LOGIN_BTN))->item.btn_item),
            left_button,
            button_color,
            button_color,
            {
                text_color,
                "LOGIN",
                (float)Utils::kBaseFontSize
            },
            true,
            LoginAction
        );

        UILib::InitButton(
            &((menu_items+((int)LoginItems::VARIABLE_BTN))->item.btn_item),
            right_button,
            button_color,
            button_color,
            {
                text_color,
                "REGISTER",
                (float)Utils::kBaseFontSize
            },
            true,
            RegisterAction
        );
    }

    //Initializes all menu text Inputs
    void InitTextInputs(){
        Utils::Collider first_left_side = {{(Utils::kWindowWidth*0.5f)-200, 200}, JMATH::Vec2Sub({(Utils::kWindowWidth*0.5f)-50, 250},{0.0f, (menu_items)->item_name.font_size*0.33f})};
        Utils::Collider first_right_side = {{(Utils::kWindowWidth*0.5f)+40, 200}, {(Utils::kWindowWidth*0.5f)+40 + (Utils::kBaseFontSize*14), 250}};

        UILib::InitTextInput(
            &((menu_items + ((int)LoginItems::USER_TI))->item.text_item),
            {first_left_side.P1, first_left_side.P2},
            first_right_side,
            {255,255,255,255},
            {0,0,0,255},
            {{255,255,255,255},nullptr, (float)Utils::kBaseFontSize},
            true,
            true,
            false
        );

        UILib::InitTextInput(
            &((menu_items + ((int)LoginItems::PASSWD_TI))->item.text_item),
            {JMATH::Vec2Sum(first_left_side.P1, {0.0f, 75.0f}),JMATH::Vec2Sum(first_left_side.P2, {0.0f, 75.0f})},
            {JMATH::Vec2Sum(first_right_side.P1, {0.0f, 75.0f}),JMATH::Vec2Sum(first_right_side.P2, {0.0f, 75.0f})},
            {255,255,255,255},
            {0,0,0,255},
            {{255,255,255,255},nullptr, (float)Utils::kBaseFontSize},
            true,
            true,
            false
        );
    }

    //Initializes all UI items
    void InitMenuItems(){

        // text_inputs = (UILib::TextInput*) malloc(sizeof(UILib::TextInput)*(int)LoginItems::TOTAL_ITEMS);
        menu_items = (UILib::UI_Item*) malloc(sizeof(UILib::UI_Item)*(int)LoginItems::TOTAL_ITEMS);

        UILib::InitItem(
            (menu_items + ((int)LoginItems::USER_TI)),
            UILib::ItemType::TEXT_INPUT,
            {{255,255,255,255},"USERNAME", Utils::kBaseFontSize*2.0f}
        );
        UILib::InitItem(
            (menu_items + ((int)LoginItems::PASSWD_TI)),
            UILib::ItemType::TEXT_INPUT,
            {{255,255,255,255},"PASSWORD", Utils::kBaseFontSize*2.0f}
        );
        UILib::InitItem(
            (menu_items + ((int)LoginItems::LOGIN_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"LOGIN", Utils::kBaseFontSize*2.0f}
        );
        UILib::InitItem(
            (menu_items + ((int)LoginItems::VARIABLE_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"REGISTER", Utils::kBaseFontSize*2.0f}
        );
    }

    //Whole Login Menu initializer
    void Init(){
        InitMenuItems();
        InitTextInputs();
        InitButtons();
    }

    //LOGIN MENU LOAD

    //Based on the level/screen you come from, the Login Menu will be loaded differently
    void Load(GameManager::Level level_p){
        prev_level = level_p;
        switch(level_p){
            case GameManager::Level::PLAY_MENU:
            case GameManager::Level::MAIN_MENU:
                (menu_items+((int)LoginItems::VARIABLE_BTN))->item.btn_item.action = BackAction;
                (menu_items+((int)LoginItems::VARIABLE_BTN))->item.btn_item.button_text.text = "BACK";
            break;
            default:
                (menu_items+((int)LoginItems::VARIABLE_BTN))->item.btn_item.action = RegisterAction;
                (menu_items+((int)LoginItems::VARIABLE_BTN))->item.btn_item.button_text.text = "REGISTER";
            break;
        }

        GameManager::game_status.level = GameManager::Level::LOGIN_MENU;
    }

    //LOGIN MENU UPDATE

    //Whole Login Menu update method
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
                selected_item = ((int)LoginItems::TOTAL_ITEMS) - 1;
            }else{
                selected_item--;
            }
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_Down) || esat::IsSpecialKeyDown(esat::kSpecialKey_Tab)){
            ++selected_item %= (int)LoginItems::TOTAL_ITEMS;
        }
        
        for(int i = 0; i < (int)LoginItems::TOTAL_ITEMS; i++){
            UILib::UpdateItem(menu_items+i, &selected_item, i);
        }
    }

    //LOGIN MENU DRAW

    void DrawMenuItems(){
        for(int i = 0; i < (int)LoginItems::TOTAL_ITEMS; i++){
            UILib::DrawItem(*(menu_items+i));
        }
    }

    //Whole Login Menu draw method
    void Draw(){
        DrawMenuItems();
    }

    void EmptyMemory(){
        for(int i = 0; i < (int)LoginItems::TOTAL_ITEMS; i++){
            UILib::EmptyItemMemory(menu_items+i);
        }
        free(menu_items);
    }
}