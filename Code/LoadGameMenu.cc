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
#include "./LoadGameMenu.h"
#include "./MainMenu.h"
#include "./RegisterMenu.h"
#include "./Gameplay.h"

namespace LoadGameMenu{
    //Memory block that holds all the menu items no matter if they are visible or not.
    UILib::UI_Item *menu_items = nullptr;
    int selected_item = -1;

    TList::ListNode *user_games = nullptr;
    TList::ListNode *loaded_games_page = nullptr;
    int page_number = 0;
    bool is_last_page = false;

    //Loads the unfinished games for the current user for the specific page.
    //More precisely, puts the ListNode corresponding to the loaded_games_page delivered as parameter as the head of the page
    //and updates the corresponding buttons.
    void LoadGamesPage(int page = 0){
        TList::ListNode** aux_list = (TList::ListNode**) &user_games;
        TList::ListNode *aux_game_node = nullptr;

        loaded_games_page = TList::GetIndexListNode((TList::ListNode*) *aux_list, page_number*10);

        for(int i = 0, game_i = 0; i < (int)LoadGameMenuItems::PREV_PAG_BTN; i++){
            aux_game_node = TList::GetIndexListNode(loaded_games_page,game_i);
            (menu_items+i)->item.btn_pa_item.is_visible = game_i<TList::ListLength(loaded_games_page);
            if((menu_items+i)->item.btn_pa_item.is_visible){
                (menu_items+i)->item.btn_pa_item.action_p = &(aux_game_node->info.game_info);
            }

            //Every game loads 2 buttons determined by i
            if(i%2 != 0){
                game_i++;
            }
        }

        is_last_page = TList::ListLength(loaded_games_page) <= 10;
    }

    //ACTIONS
    void PlayAction(void *game){
        Gameplay::Load((PlayedGames::PlayedGame*)game);
    }

    void DeleteAction(void *game){
        TList::ListNode** aux_list = (TList::ListNode**) &(PlayedGames::game_list);
        TList::ListInfo aux_info = {NULL};
        aux_info.game_info = *((PlayedGames::PlayedGame*) game);
        
        if(TList::ListLength(loaded_games_page) <= 1){
            //The actual page will be empty. So before deleting the element
            //nd update/save the deletion, the previous page is loaded 
            //so loaded_games_page does not miss its pointer
            LoadGamesPage(--page_number);
        }
        TList::DeleteElement(aux_list, aux_info);
        TList::DeleteElement(&user_games, aux_info);
        TList::SaveList(aux_list, PlayedGames::game_list_dat, PlayedGames::game_list_dat_path);
        LoadGamesPage(page_number);
    }

    void PrevPageAction(){
        page_number--;
        LoadGamesPage(page_number);
    }

    void NextPageAction(){
        page_number++;
        LoadGamesPage(page_number);
    }

    void BackAction(){
        MainMenu::Load();
    }

    //LOADGAME MENU INIT

    //Initializes all menu buttons
    void InitButtons(){
        Utils::Color text_color = {255,255,255,255};
        float list_font_size = Utils::kBaseFontSize;
        TList::ListNode *aux_game_node = nullptr;

        JMATH::Vec2 base_button_coord = {
            (Utils::kWindowWidth) - (strlen("PLAY DELETE  ") * list_font_size),
            232.0f
        };

        JMATH::Vec2 button_height = {0,25};
        JMATH::Vec2 margin_v = {0,27.5f};
        JMATH::Vec2 play_width = {(strlen("PLAY")*list_font_size),0};
        JMATH::Vec2 delete_width_1 = {(strlen("PLAY ")*list_font_size),0};
        JMATH::Vec2 delete_width_2 = {(strlen("PLAYDELETE")*list_font_size),0};

        //GAME MANAGEMENT BUTTONS
        for(int i = 0, u = 0; i < (int)LoadGameMenuItems::PREV_PAG_BTN; i++){
            aux_game_node = TList::GetIndexListNode(loaded_games_page,u);
            if(i%2 == 0){
                UILib::InitButtonPA(
                    &((menu_items+i)->item.btn_pa_item),
                    {
                        JMATH::Vec2Sum(base_button_coord, JMATH::Vec2Scale(margin_v,i)),
                        JMATH::Vec2Sum(base_button_coord, JMATH::Vec2Sum(button_height,JMATH::Vec2Sum(play_width, JMATH::Vec2Scale(margin_v,i)))),
                    },
                    {100,100,100,200},
                    {100,100,100,200},
                    {
                        text_color,
                        (menu_items+i)->item_name.text,
                        list_font_size
                    },
                    aux_game_node != nullptr,
                    PlayAction,
                    (aux_game_node == nullptr ? nullptr : &(aux_game_node->info.game_info))
                );
            }else{
                UILib::InitButtonPA(
                    &((menu_items+i)->item.btn_pa_item),
                    {
                        JMATH::Vec2Sum(base_button_coord, JMATH::Vec2Sum(delete_width_1, JMATH::Vec2Scale(margin_v,i-1))),
                        JMATH::Vec2Sum(base_button_coord, JMATH::Vec2Sum(button_height,JMATH::Vec2Sum(delete_width_2, JMATH::Vec2Scale(margin_v,i-1)))),
                    },
                    {100,100,100,200},
                    {100,100,100,200},
                    {
                        text_color,
                        (menu_items+i)->item_name.text,
                        list_font_size
                    },
                    aux_game_node != nullptr,
                    DeleteAction,
                    (aux_game_node == nullptr ? nullptr : &(aux_game_node->info.game_info))
                );

                u++;
            }
        }

        UILib::InitButton(
            &((menu_items+((int)LoadGameMenuItems::PREV_PAG_BTN))->item.btn_item),
            {
                {50.0f, Utils::kWindowHeight-175.0f},
                {100.0f, Utils::kWindowHeight-125.0f}
            },
            {100,100,100,200},
            {100,100,100,200},
            {
                text_color,
                (menu_items+((int)LoadGameMenuItems::PREV_PAG_BTN))->item_name.text,
                list_font_size
            },
            true,
            PrevPageAction
        );

        UILib::InitButton(
            &((menu_items+((int)LoadGameMenuItems::NEXT_PAG_BTN))->item.btn_item),
            {
                {Utils::kWindowWidth-100.0f, Utils::kWindowHeight-175.0f},
                {Utils::kWindowWidth-50.0f, Utils::kWindowHeight-125.0f}
            },
            {100,100,100,200},
            {100,100,100,200},
            {
                text_color,
                (menu_items+((int)LoadGameMenuItems::NEXT_PAG_BTN))->item_name.text,
                list_font_size
            },
            true,
            NextPageAction
        );

        UILib::InitButton(
            &((menu_items+((int)LoadGameMenuItems::BACK_BTN))->item.btn_item),
            {
                {Utils::kWindowWidth-150.0f, Utils::kWindowHeight-80.0f},
                {Utils::kWindowWidth-50.0f, Utils::kWindowHeight-30.0f}
            },
            {100,100,100,200},
            {100,100,100,200},
            {
                text_color,
                (menu_items+((int)LoadGameMenuItems::BACK_BTN))->item_name.text,
                list_font_size
            },
            true,
            BackAction
        );

    }


    //Initializes all UI items
    void InitMenuItems(){

        // text_inputs = (UILib::TextInput*) malloc(sizeof(UILib::TextInput)*(int)LoadGameMenuItems::TOTAL_ITEMS);
        menu_items = (UILib::UI_Item*) malloc(sizeof(UILib::UI_Item)*(int)LoadGameMenuItems::TOTAL_ITEMS);

        //GAME MANAGEMENT BUTTONS
        for(int i = 0; i < (int)LoadGameMenuItems::PREV_PAG_BTN; i++){
            if(i%2 == 0){
                UILib::InitItem(
                    (menu_items + i),
                    UILib::ItemType::BUTTON_PA,
                    {{255,255,255,255},"PLAY", (float)Utils::kBaseFontSize}
                );
            }else{
                UILib::InitItem(
                    (menu_items + i),
                    UILib::ItemType::BUTTON_PA,
                    {{255,255,255,255},"DELETE", (float)Utils::kBaseFontSize}
                );
            }
        }

        UILib::InitItem(
            (menu_items + ((int)LoadGameMenuItems::PREV_PAG_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"<", Utils::kBaseFontSize*2.0f}
        );

        UILib::InitItem(
            (menu_items + ((int)LoadGameMenuItems::NEXT_PAG_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},">", Utils::kBaseFontSize*2.0f}
        );

        UILib::InitItem(
            (menu_items + ((int)LoadGameMenuItems::BACK_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"BACK", Utils::kBaseFontSize*2.0f}
        );
    }

    //Whole Admin Menu initializer
    void Init(){
        InitMenuItems();
        InitButtons();
    }

    //LOADGAME MENU LOAD


    void LoadUserGames(){
        page_number = 0;
        TList::ClearList(&user_games);
        for(TList::ListNode *aux = (TList::ListNode*)PlayedGames::game_list ; aux != nullptr; aux = aux->next){
            if(aux->info.game_info.p1_user == GameManager::game_status.logged_user){
                TList::InsertList(&user_games, TList::ListType::PLAYED_GAME, aux->info);
            }
        }
    }

    //Loads the Admin menu
    void Load(){
        LoadUserGames();
        LoadGamesPage();
        selected_item = -1;
        GameManager::game_status.level = GameManager::Level::LOAD_MENU;
    }

    //LOADGAME MENU UPDATE

    //Whole Admin Menu update method
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
            do{
                if(selected_item <= 0){
                    selected_item = ((int)LoadGameMenuItems::TOTAL_ITEMS) - 1;
                }else{
                    selected_item--;
                }
            }while(!UILib::IsItemVisible(*(menu_items+selected_item)));
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_Down) || esat::IsSpecialKeyDown(esat::kSpecialKey_Tab)){
            do{
                ++selected_item %= (int)LoadGameMenuItems::TOTAL_ITEMS;
            }while(!UILib::IsItemVisible(*(menu_items+selected_item)));
        }

        for(int i = 0; i < (int)LoadGameMenuItems::TOTAL_ITEMS; i++){
            switch((LoadGameMenuItems)i){
                case LoadGameMenuItems::PREV_PAG_BTN:
                    if(page_number > 0){
                        UILib::UpdateItem(menu_items+i, &selected_item, i);
                    }
                break;
                case LoadGameMenuItems::NEXT_PAG_BTN:
                    if(!is_last_page){
                        UILib::UpdateItem(menu_items+i, &selected_item, i);
                    }
                break;
                default:
                    UILib::UpdateItem(menu_items+i, &selected_item, i);
                break;
            }
        }
    }

    //LOADGAME MENU DRAW
    void DrawTitle(){
        UILib::Text title = {
            {255,255,255,255},
            "UNFINISHED GAMES",
            Utils::kBaseFontSize*3.5f
        };

        esat::DrawSetTextFont("./Assets/Fonts/Neuropol.otf");
        UILib::DrawText(
            (Utils::kWindowWidth*0.5f) - ((strlen(title.text)*0.425)*title.font_size),
            100,
            title
        );
        esat::DrawSetTextFont("./Assets/Fonts/Hyperspace.ttf");
    }

    void DrawGameItem(JMATH::Vec2 coord, float font_size, PlayedGames::PlayedGame game){
        UILib::DrawText(
            coord,
            {
                {255,255,255,255},
                *(PlayedGames::gamemode_texts+(int)game.gamemode),
                font_size
            }
        );


        if(game.gamemode != PlayedGames::Gamemode::SP){
            UILib::DrawText(
                JMATH::Vec2Sum(coord, {font_size*strlen("MPCOOPERATIV"),0}),
                {
                    {255,255,255,255},
                    game.p2_user->username,
                    font_size
                }
            );
        }

        UILib::DrawText(
            JMATH::Vec2Sum(coord, {font_size*strlen("                          "),0}),
            {
                {255,255,255,255},
                "01/01/2026 12:00",
                font_size*0.5f
            }
        );

    }

    void DrawGameItems(JMATH::Vec2 base_coord, JMATH::Vec2 margin_v, float list_font_size){
        TList::ListNode* aux_list = loaded_games_page;

        //Starts with 1 to apply margin from the begining and draws a maximum of 10
        for(int i = 0; aux_list != nullptr && i < 10; aux_list = aux_list->next ,i++){
            DrawGameItem(JMATH::Vec2Sum(base_coord,JMATH::Vec2Scale(margin_v,i+1)), list_font_size, aux_list->info.game_info);
        }
    }

    void DrawGameList(){
        float list_font_size = Utils::kBaseFontSize * 1.5f;

        JMATH::Vec2 base_coord = {
            (Utils::kWindowWidth*0.5f) -
            (strlen("GAMEMODE           P2 USERNAME    DATE TIME") * 0.45f * list_font_size),
            200
        };
        JMATH::Vec2 margin_v = {0,55};

        UILib::DrawText(
            base_coord,
            {
                {255,255,255,255},
                "GAMEMODE           P2 USERNAME              DATE TIME",
                {list_font_size}
            }
        );

        DrawGameItems(base_coord, margin_v, list_font_size);
    }

    void DrawMenuItems(){
        for(int i = 0; i < (int)LoadGameMenuItems::TOTAL_ITEMS; i++){
            switch((LoadGameMenuItems)i){
                case LoadGameMenuItems::PREV_PAG_BTN:
                    if(page_number <= 0){
                        (menu_items+i)->item.btn_item.is_visible = false;
                    }else{
                        (menu_items+i)->item.btn_item.is_visible = true;
                    }
                break;
                case LoadGameMenuItems::NEXT_PAG_BTN:
                    if(is_last_page){
                        (menu_items+i)->item.btn_item.is_visible = false;
                    }else{
                        (menu_items+i)->item.btn_item.is_visible = true;
                    }
                break;
            }
            UILib::DrawItem(*(menu_items+i));
        }
    }

    //Whole Admin Menu draw method
    void Draw(){
        DrawTitle();
        DrawGameList();
        DrawMenuItems();
    }

    void EmptyMemory(){
        for(int i = 0; i < (int)LoadGameMenuItems::TOTAL_ITEMS; i++){
            UILib::EmptyItemMemory(menu_items+i);
        }
        free(menu_items);
        TList::ClearList(&user_games);
    }
}