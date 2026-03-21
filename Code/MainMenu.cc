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
#include "./MainMenu.h"
#include "./PlayMenu.h"
#include "./HighscoresMenu.h"

namespace MainMenu{
    //Memory block that holds all the menu items no matter if they are visible or not.
    UILib::UI_Item *menu_items = nullptr;
    int selected_item = -1;


    //ACTIONS
    void PlayAction(){
        PlayMenu::Load();
    }

    void HighScoreAction(){
        HighscoresMenu::Load();
    }

    void QuitAction(){
        GameManager::game_status.level = GameManager::Level::QUIT;
    }

    void AdminAction(){
        printf("ADMIN ACTION WIP\n");
    }

    //MAIN MENU INIT

    //Initializes all menu buttons
    void InitButtons(){
        JMATH::Vec2 centered_coord = {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f};
        JMATH::Vec2 margin_y = {0.0f, 50.0f};

        Utils::Collider base_collider = {{centered_coord.x, centered_coord.y-(float)Utils::kBaseFontSize},{centered_coord.x, (centered_coord.y+(float)Utils::kBaseFontSize)}};

        Utils::Color button_color = {0,0,0,255};
        Utils::Color text_color = {255,255,255,255};
        float font_width_aux = ((float)Utils::kBaseFontSize*2)/3.5f;

        UILib::InitButton(
            &((menu_items+((int)MainMenuItems::PLAY_BTN))->item.btn_item),
            {
                {base_collider.P1.x-(strlen((menu_items+((int)MainMenuItems::PLAY_BTN))->item_name.text)*font_width_aux), base_collider.P1.y},
                {base_collider.P2.x+(strlen((menu_items+((int)MainMenuItems::PLAY_BTN))->item_name.text)*font_width_aux), base_collider.P2.y}
            },
            button_color,
            button_color,
            {
                text_color,
                (menu_items+((int)MainMenuItems::PLAY_BTN))->item_name.text,
                (float)Utils::kBaseFontSize*2
            },
            true,
            PlayAction
        );

        UILib::InitButton(
            &((menu_items+((int)MainMenuItems::HIGHSCORES_BTN))->item.btn_item),
            {
                JMATH::Vec2Sum(
                    {base_collider.P1.x-(strlen((menu_items+((int)MainMenuItems::HIGHSCORES_BTN))->item_name.text)*((float)Utils::kBaseFontSize*2)/3.5f), base_collider.P1.y},
                    margin_y
                ),
                JMATH::Vec2Sum(
                    {base_collider.P2.x+(strlen((menu_items+((int)MainMenuItems::HIGHSCORES_BTN))->item_name.text)*((float)Utils::kBaseFontSize*2)/3.5f), base_collider.P2.y},
                    margin_y
                )
            },
            button_color,
            button_color,
            {
                text_color,
                (menu_items+((int)MainMenuItems::HIGHSCORES_BTN))->item_name.text,
                (float)Utils::kBaseFontSize*2
            },
            true,
            HighScoreAction
        );

        UILib::InitButton(
            &((menu_items+((int)MainMenuItems::QUIT_BTN))->item.btn_item),
            {
                JMATH::Vec2Sum(
                    {base_collider.P1.x-(strlen((menu_items+((int)MainMenuItems::QUIT_BTN))->item_name.text)*((float)Utils::kBaseFontSize*2)/3.5f), base_collider.P1.y},
                    JMATH::Vec2Scale(margin_y,2)
                ),
                JMATH::Vec2Sum(
                    {base_collider.P2.x+(strlen((menu_items+((int)MainMenuItems::QUIT_BTN))->item_name.text)*((float)Utils::kBaseFontSize*2)/3.5f), base_collider.P2.y},
                    JMATH::Vec2Scale(margin_y,2)
                )
            },
            button_color,
            button_color,
            {
                text_color,
                (menu_items+((int)MainMenuItems::QUIT_BTN))->item_name.text,
                (float)Utils::kBaseFontSize*2
            },
            true,
            QuitAction
        );

        UILib::InitButton(
            &((menu_items+((int)MainMenuItems::ADMIN_BTN))->item.btn_item),
            {
                {Utils::kWindowWidth-100.0f, Utils::kWindowHeight-80.0f},
                {Utils::kWindowWidth-30.0f, Utils::kWindowHeight-30.0f}
            },
            {100,100,100,200},
            {100,100,100,200},
            {
                text_color,
                (menu_items+((int)MainMenuItems::ADMIN_BTN))->item_name.text,
                (float)Utils::kBaseFontSize
            },
            true,
            AdminAction
        );
        
    }


    //Initializes all UI items
    void InitMenuItems(){

        // text_inputs = (UILib::TextInput*) malloc(sizeof(UILib::TextInput)*(int)MainMenuItems::TOTAL_ITEMS);
        menu_items = (UILib::UI_Item*) malloc(sizeof(UILib::UI_Item)*(int)MainMenuItems::TOTAL_ITEMS);

        UILib::InitItem(
            (menu_items + ((int)MainMenuItems::PLAY_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"PLAY GAME", Utils::kBaseFontSize*2.0f}
        );
        UILib::InitItem(
            (menu_items + ((int)MainMenuItems::HIGHSCORES_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"HIGH SCORES", Utils::kBaseFontSize*2.0f}
        );
        UILib::InitItem(
            (menu_items + ((int)MainMenuItems::QUIT_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"QUIT GAME", Utils::kBaseFontSize*2.0f}
        );
        UILib::InitItem(
            (menu_items + ((int)MainMenuItems::ADMIN_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"ADMIN", Utils::kBaseFontSize*2.0f}
        );
    }

    //Whole Main Menu initializer
    void Init(){
        InitMenuItems();
        InitButtons();
    }

    //MAIN MENU LOAD

    //Loads the main menu
    void Load(){
        GameManager::game_status.level = GameManager::Level::MAIN_MENU;
    }

    //MAIN MENU UPDATE

    //Whole Main Menu update method
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
                selected_item = ((int)MainMenuItems::TOTAL_ITEMS) - 1;
            }else{
                selected_item--;
            }
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_Down) || esat::IsSpecialKeyDown(esat::kSpecialKey_Tab)){
            ++selected_item %= (int)MainMenuItems::TOTAL_ITEMS;
        }
        
        for(int i = 0; i < (int)MainMenuItems::TOTAL_ITEMS; i++){
            UILib::UpdateItem(menu_items+i, &selected_item, i);
        }
    }

    //MAIN MENU DRAW
    void DrawTitle(){
        UILib::Text title = {
            {255,255,255,255},
            "ASTEROIDS",
            Utils::kBaseFontSize*3.5f
        };
        
        esat::DrawSetTextFont("./Assets/Fonts/Neuropol.otf");
        UILib::DrawText(
            (Utils::kWindowWidth*0.5f) - ((strlen(title.text)/2.5f)*Utils::kBaseFontSize*3.5f),
            225, 
            title
        );
        esat::DrawSetTextFont("./Assets/Fonts/Hyperspace.ttf");
    }

    void DrawMenuItems(){
        for(int i = 0; i < (int)MainMenuItems::TOTAL_ITEMS; i++){
            UILib::DrawItem(*(menu_items+i));
        }
    }

    //Whole Main Menu draw method
    void Draw(){
        DrawTitle();
        DrawMenuItems();
    }

    void EmptyMemory(){
        for(int i = 0; i < (int)MainMenuItems::TOTAL_ITEMS; i++){
            UILib::EmptyItemMemory(menu_items+i);
        }
        free(menu_items);
    }
}