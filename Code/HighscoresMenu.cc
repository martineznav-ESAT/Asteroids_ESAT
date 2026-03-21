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
#include "./HighscoresMenu.h"
#include "./MainMenu.h"

namespace HighscoresMenu{
    //Memory block that holds all the menu items no matter if they are visible or not.
    UILib::UI_Item *menu_items = nullptr;
    int selected_item = -1;

    GameManager::PlayedGame *top_games = nullptr;

    //ACTIONS
    void BackAction(){
        MainMenu::Load();
    }

    //HIGHSCORES MENU INIT

    //Initializes all menu buttons
    void InitButtons(){
        Utils::Color text_color = {255,255,255,255};

        UILib::InitButton(
            &((menu_items+((int)HighscoresMenuItems::BACK_BTN))->item.btn_item),
            {
                {Utils::kWindowWidth-100.0f, Utils::kWindowHeight-80.0f},
                {Utils::kWindowWidth-30.0f, Utils::kWindowHeight-30.0f}
            },
            {100,100,100,200},
            {100,100,100,200},
            {
                text_color,
                (menu_items+((int)HighscoresMenuItems::BACK_BTN))->item_name.text,
                (float)Utils::kBaseFontSize
            },
            true,
            BackAction
        );
        
    }


    //Initializes all UI items
    void InitMenuItems(){

        // text_inputs = (UILib::TextInput*) malloc(sizeof(UILib::TextInput)*(int)HighscoresMenuItems::TOTAL_ITEMS);
        menu_items = (UILib::UI_Item*) malloc(sizeof(UILib::UI_Item)*(int)HighscoresMenuItems::TOTAL_ITEMS);

        UILib::InitItem(
            (menu_items + ((int)HighscoresMenuItems::BACK_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"BACK", Utils::kBaseFontSize*2.0f}
        );
    }

    //Initializes the values of the best 10 games ever played
    void InitTopGames(){
        top_games = (GameManager::PlayedGame*) malloc(sizeof(GameManager::PlayedGame)*10);
        //TO_DO
        // top_games = GameManager::FindHighScores();
    }

    //Whole Highscores Menu initializer
    void Init(){
        InitTopGames();
        InitMenuItems();
        InitButtons();
    }

    //HIGHSCORES MENU LOAD

    //Loads the highscores menu
    void Load(){
        GameManager::game_status.level = GameManager::Level::HIGHSCORES_MENU;
    }

    //HIGHSCORES MENU UPDATE

    //Whole Highscores Menu update method
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
                selected_item = ((int)HighscoresMenuItems::TOTAL_ITEMS) - 1;
            }else{
                selected_item--;
            }
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_Down) || esat::IsSpecialKeyDown(esat::kSpecialKey_Tab)){
            ++selected_item %= (int)HighscoresMenuItems::TOTAL_ITEMS;
        }
        
        for(int i = 0; i < (int)HighscoresMenuItems::TOTAL_ITEMS; i++){
            UILib::UpdateItem(menu_items+i, &selected_item, i);
        }
    }

    //HIGHSCORES MENU DRAW
    void DrawTitle(){
        UILib::Text title = {
            {255,255,255,255},
            "TOP 10 SCORES",
            Utils::kBaseFontSize*2.0f
        };
        
        esat::DrawSetTextFont("./Assets/Fonts/Neuropol.otf");
        UILib::DrawText(
            (Utils::kWindowWidth*0.5f) - ((strlen(title.text)/2.5f)*title.font_size),
            50, 
            title
        );
        esat::DrawSetTextFont("./Assets/Fonts/Hyperspace.ttf");
    }

    void DrawHighScore(JMATH::Vec2 coord, GameManager::PlayedGame game){
        UILib::DrawText(
            coord, 
            {
                {255,255,255,255},
                "ALIAS",
                {(float)Utils::kBaseFontSize}
            }
        );
        UILib::DrawText(
            JMATH::Vec2Sum(coord, {(float)Utils::kBaseFontSize*strlen("ALIAS "),0}), 
            {
                {255,255,255,255},
                "AAA",
                {(float)Utils::kBaseFontSize}
            }
        );

        UILib::DrawText(
            JMATH::Vec2Sum(coord, {(float)Utils::kBaseFontSize*strlen("ALIAS AAA        "),0}), 
            {
                {255,255,255,255},
                "SCORE",
                {(float)Utils::kBaseFontSize}
            }
        );
        UILib::DrawIntToText(
            JMATH::Vec2Sum(coord, {(float)Utils::kBaseFontSize*strlen("ALIAS AAA        SCORE "),0}), 
            {
                {255,255,255,255},
                nullptr,
                {(float)Utils::kBaseFontSize}
            },
            0,6,true
        );
    }

    void DrawGameScores(){
        JMATH::Vec2 base_coord = {
            (Utils::kWindowWidth*0.5f) - 
            (strlen("ALIAS AAA        SCORE 000000") * 0.45f * ((float)Utils::kBaseFontSize)),
            100
        };
        JMATH::Vec2 margin_v = {0,45};

        for(int i = 0; i < 10; i++){
            DrawHighScore(JMATH::Vec2Sum(base_coord,JMATH::Vec2Scale(margin_v,i)), *(top_games+i));
        }
    }

    void DrawMenuItems(){
        for(int i = 0; i < (int)HighscoresMenuItems::TOTAL_ITEMS; i++){
            UILib::DrawItem(*(menu_items+i));
        }
    }

    //Whole Highscores Menu draw method
    void Draw(){
        DrawTitle();
        DrawGameScores();
        DrawMenuItems();
    }

    void EmptyMemory(){
        for(int i = 0; i < (int)HighscoresMenuItems::TOTAL_ITEMS; i++){
            UILib::EmptyItemMemory(menu_items+i);
        }
        free(menu_items);
    }
}