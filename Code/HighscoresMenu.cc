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
    FILE *highscores_dat = nullptr;
    char *highscores_dat_path = "./Assets/Files/Data/highscores.dat";
    
    TList::ListNode *top_games = nullptr;

    //Memory block that holds all the menu items no matter if they are visible or not.
    UILib::UI_Item *menu_items = nullptr;
    int selected_item = -1;


    //ACTIONS
    void BackAction(){
        MainMenu::Load();
    }

    //HIGHSCORES MENU INIT

    //Given a ListNode* as an index, moves all the highscores underneath leaving room for the index value to fill with the second parameter
    void DisplaceAndFillHighScores(TList::ListNode *index, TList::ListInfo info){
        TList::ListInfo aux = {NULL};
        
        for(TList::ListNode *p = index; p!=nullptr; p = p->next){
            aux = p->info;
            p->info = info;
            info = aux;
        }
    }


    bool AddHighScoreGame(TList::ListInfo game){
        bool is_comparison_end = false;
        
        if(game.game_info.is_finished){
            for(TList::ListNode *highscore = top_games; highscore!=nullptr && !is_comparison_end; highscore = highscore->next){
                //If the Compare Score is bigger or equal than the highscore one being checked
                //Sets the value to the current position and displaces the rest of the rows underneath
                if(game.game_info.compare_score >= highscore->info.game_info.compare_score){
                    // printf("%d >= %d\n",game.game_info.compare_score, highscore->info.game_info.compare_score);
                    DisplaceAndFillHighScores(highscore, game);
                    is_comparison_end = true;
                }
            }
        }
        
        return is_comparison_end;
    }

    //Sets the parameter list with the top 10 PlayedGames with highest scores registered based on all games played;
    void UpdateHighScores(){
        //Searches for the top 10 games on the game_list of all played games
        for(TList::ListNode *game = (TList::ListNode*) PlayedGames::game_list; game!=nullptr; game = game->next){
            //For every game, its compared with the highscore list
            AddHighScoreGame(game->info);
            // printf("NEXT GAME\n");
        }

        // TList::PrintList(top_games);
    }

    void InitEmptyHighscores(){
        TList::ClearList(&top_games);
        top_games = TList::CreateList();
        TList::ListInfo aux_info = {NULL};
        TList::ListInfo aux_user = {NULL};

        if(!TList::LoadList(&top_games, TList::ListType::PLAYED_GAME, highscores_dat, highscores_dat_path)){
            //If there is no highscores list to load, creates an empty one
            for(int i = 0; i < 10 ; i++){
                // printf("i = %d\n",i);
                aux_info.game_info = PlayedGames::NewGame();
                
                aux_info.game_info.game_id = i-100;
                
                aux_info.game_info.p1_user = &(UserManager::empty_user);
                aux_info.game_info.p2_user = &(UserManager::empty_user);

                aux_info.game_info.is_finished = true;

                TList::InsertList(&top_games, TList::ListType::PLAYED_GAME, aux_info);
            }
            UpdateHighScores();
            printf("SAVE UpdateHighScores\n");
            TList::SaveList(&top_games, highscores_dat, highscores_dat_path);
            printf("SAVEED UpdateHighScores\n");
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
        // printf("InitEmptyHighscores\n");
        InitMenuItems();
        // printf("InitMenuItems\n");
        InitButtons();
        // printf("InitButtons\n");
    }

    //HIGHSCORES MENU LOAD

    //Loads the highscores menu
    void Load(){
        selected_item = -1;
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

        switch (game.gamemode){
            case PlayedGames::Gamemode::SP:
                UILib::DrawText(
                    JMATH::Vec2Sum(coord, {font_size*strlen("ALIAS"),0}), 
                    {
                        {255,255,255,255},
                        game.p1_user->alias,
                        font_size
                    }
                );
            break;

            case PlayedGames::Gamemode::MP_ALT:
            case PlayedGames::Gamemode::MP_VS:
                if(game.compare_score == game.p1.score && game.compare_score == game.p2.score){
                    UILib::DrawText(
                        JMATH::Vec2Sum(coord, {font_size*strlen("ALIAS"),0}), 
                        {
                            {255,255,255,255},
                            game.p1_user->alias,
                            font_size
                        }
                    );
                    UILib::DrawText(
                        JMATH::Vec2Sum(coord, {font_size*strlen("ALIASAA"),0}), 
                        {
                            {255,255,255,255},
                            "=",
                            font_size
                        }
                    );
                    UILib::DrawText(
                        JMATH::Vec2Sum(coord, {font_size*strlen("ALIASAA+"),0}), 
                        {
                            {255,255,255,255},
                            game.p2_user->alias,
                            font_size
                        }
                    );
                }else{
                    if(game.compare_score == game.p1.score){
                        UILib::DrawText(
                            JMATH::Vec2Sum(coord, {font_size*strlen("ALIAS"),0}), 
                            {
                                {255,255,255,255},
                                game.p1_user->alias,
                                font_size
                            }
                        );
                    }else{
                        UILib::DrawText(
                            JMATH::Vec2Sum(coord, {font_size*strlen("ALIAS"),0}), 
                            {
                                {255,255,255,255},
                                game.p2_user->alias,
                                font_size
                            }
                        );
                    }
                }
            break;
        
            case PlayedGames::Gamemode::MP_COOP:
                UILib::DrawText(
                        JMATH::Vec2Sum(coord, {font_size*strlen("ALIAS"),0}), 
                        {
                            {255,255,255,255},
                            game.p1_user->alias,
                            font_size
                        }
                    );
                    UILib::DrawText(
                        JMATH::Vec2Sum(coord, {font_size*strlen("ALIASAA"),0}), 
                        {
                            {255,255,255,255},
                            "+",
                            font_size
                        }
                    );
                    UILib::DrawText(
                        JMATH::Vec2Sum(coord, {font_size*strlen("ALIASAA+"),0}), 
                        {
                            {255,255,255,255},
                            game.p2_user->alias,
                            font_size
                        }
                    );
            break;
        }
        

        UILib::DrawText(
            JMATH::Vec2Sum(coord, {font_size*strlen("ALIAS AAA     "),0}), 
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
            game.compare_score,6,true
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
            DrawHighScore(JMATH::Vec2Sum(base_coord,JMATH::Vec2Scale(margin_v,i)), list_font_size, p->info.game_info);
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

    void CloseFiles(){
        if(highscores_dat != nullptr){
            fclose(highscores_dat);
        }
    }

    void EmptyMemory(){
        for(int i = 0; i < (int)HighscoresMenuItems::TOTAL_ITEMS; i++){
            UILib::EmptyItemMemory(menu_items+i);
        }
        free(menu_items);

        TList::ClearList(&top_games);
    }
}