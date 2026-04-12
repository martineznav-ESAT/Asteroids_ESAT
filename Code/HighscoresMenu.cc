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
#include "../Libs/CustomLibs/TList.h"

#include "../Libs/GameplayTech/GameplayTech.h"

#include "./GameManager.h"
#include "./HighscoresMenu.h"
#include "./MainMenu.h"

namespace HighscoresMenu{
    //Memory block that holds all the menu items no matter if they are visible or not.
    UILib::UI_Item *menu_items = nullptr;
    int selected_item = -1;

    TList::ListNode *top_games = nullptr;

    //ACTIONS
    void BackAction(){
        MainMenu::Load();
    }

    //HIGHSCORES MENU INIT
    void InitEmptyHighscores(){
        TList::ClearList(&top_games);
        top_games = TList::CreateList();
        TList::ListInfo aux_info = {NULL};

        for(int i = 0; i < 10 ; i++){
            // printf("i = %d\n",i);
            aux_info.game_info = PlayedGames::NewGame();
            aux_info.game_info.game_id = i-100;
            TList::InsertList(&top_games, TList::ListType::PLAYED_GAME, aux_info);
        }
    }

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

    //Whole Highscores Menu initializer
    void Init(){
        InitEmptyHighscores();
        InitMenuItems();
        InitButtons();
    }

    //HIGHSCORES MENU LOAD

    //Given a ListNode* as an index, moves all the highscores underneath leaving room for the index value to fill with the second parameter
    void DisplaceAndFillHighScores(TList::ListNode *index, TList::ListInfo info){
        TList::ListInfo aux_anterior = {NULL};
        
        for(TList::ListNode *p = index; p!=nullptr; p = p->next){
            if(p->next != nullptr){
                if(p == index){
                    aux_anterior = p->info;
                    p->info = info;
                }else{
                    info = p->info;
                    p->info = aux_anterior;
                    aux_anterior = info;
                }
            }
        }
    }

    //Sets the parameter list with the top 10 PlayedGames with highest scores registered
    void FindHighScores(){
        bool is_comparison_end = false;

        //Searches for the top 10 games on the game_list
        for(TList::ListNode *game = (TList::ListNode*) PlayedGames::game_list; game!=nullptr; game = game->next, is_comparison_end = false){
            for(TList::ListNode *highscore = top_games; highscore!=nullptr && !is_comparison_end; highscore = highscore->next){
                //TO_DO De momento se asume que son todo partidas SP en cuanto a la comparacion de SCORE
                // printf("game->info.game_info.p1.score %d != (*highscore)->next->info.game_info.p1.score %d\n",game->info.game_info.p1.score,(*highscore)->next->info.game_info.p1.score);
                
                //If the score is different starts checking.
                if(game->info.game_info.p1.score != highscore->next->info.game_info.p1.score ){
                    //If is checking the TOP 10, and the score in that moment is bigger, then it gates replaced automatically 
                    if(highscore->next == nullptr){
                        highscore->info = game->info;
                        is_comparison_end = true;
                    }else{
                        //If is checking any value between TOP 1 and TOP 9, and the actual score is bigger than the one getting checked
                        //Moves the scores under the actual one, and sets the score at this current position
                        if(game->info.game_info.p1.score > highscore->next->info.game_info.p1.score){
                            DisplaceAndFillHighScores(highscore, game->info);
                            is_comparison_end = true;
                        }
                    }
                }
            }
        }

        TList::PrintList(top_games);
    }

    //Loads the highscores menu
    void Load(){
        selected_item = -1;
        GameManager::game_status.level = GameManager::Level::HIGHSCORES_MENU;
        FindHighScores();
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
            Utils::kBaseFontSize*3.5f
        };
        
        esat::DrawSetTextFont("./Assets/Fonts/Neuropol.otf");
        UILib::DrawText(
            (Utils::kWindowWidth*0.5f) - ((strlen(title.text)/2.5f)*title.font_size),
            100, 
            title
        );
        esat::DrawSetTextFont("./Assets/Fonts/Hyperspace.ttf");
    }

    void DrawHighScore(JMATH::Vec2 coord, float font_size, PlayedGames::PlayedGame game){

        UILib::DrawText(
            coord, 
            {
                {255,255,255,255},
                "ALIAS",
                font_size
            }
        );
        UILib::DrawText(
            JMATH::Vec2Sum(coord, {font_size*strlen("ALIAS "),0}), 
            {
                {255,255,255,255},
                game.p1_user->alias,
                font_size
            }
        );

        UILib::DrawText(
            JMATH::Vec2Sum(coord, {font_size*strlen("ALIAS AAA    "),0}), 
            {
                {255,255,255,255},
                "SCORE",
                font_size
            }
        );
        UILib::DrawIntToText(
            JMATH::Vec2Sum(coord, {font_size*strlen("ALIAS AAA    SCORE "),0}), 
            {
                {255,255,255,255},
                nullptr,
                font_size
            },
            game.p1.score,6,true
        );
    }

    void DrawGameScores(){
        float list_font_size = Utils::kBaseFontSize * 2.0f;
        int i = 0;
        
        JMATH::Vec2 base_coord = {
            (Utils::kWindowWidth*0.5f) - 
            (strlen("ALIAS AAA    SCORE 000000") * 0.45f * list_font_size),
            200
        };
        JMATH::Vec2 margin_v = {0,65};

        for(TList::ListNode *p = (TList::ListNode*) top_games; p!=nullptr; p = p->next, i++){
            if(p->info.game_info.game_id >= 0){
                DrawHighScore(JMATH::Vec2Sum(base_coord,JMATH::Vec2Scale(margin_v,i)), list_font_size, p->info.game_info);
            }
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

        TList::ClearList(&top_games);
    }
}