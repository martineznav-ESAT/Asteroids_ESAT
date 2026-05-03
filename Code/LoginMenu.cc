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

#include "./GameManager.h"
#include "./UserManager.h"
#include "./LoginMenu.h"
#include "./RegisterMenu.h"
#include "./MainMenu.h"
#include "./PlayMenu.h"
#include "./Gameplay.h"

namespace LoginMenu{
    //Memory block that holds all the menu items no matter if they are visible or not.
    UILib::UI_Item *menu_items = nullptr;
    GameManager::Level prev_level;
    int selected_item = -1;

    UILib::Text error_dialog = {
        {255,0,0,255},
        nullptr,
        (float)Utils::kBaseFontSize*2
    };
    int error_dialog_lt = 3000;
    float error_dialog_ltc = error_dialog_lt;

    void ShowErrorDialog(){
        if(error_dialog_ltc < error_dialog_lt){
            UILib::DrawText(
                {(Utils::kWindowWidth*0.5f) - ((strlen(error_dialog.text)*0.30f)*error_dialog.font_size), 75.0f}, 
                error_dialog
            );
            error_dialog_ltc += 1000/Utils::kFPS;
        }
    }

    bool VerifyLogin(UserManager::User **user){
        TList::ListNode* aux_tn = nullptr;
        bool is_verified = false;
        TList::ListNode** aux_user_list = (TList::ListNode**) &(UserManager::user_list);
        TList::ListInfo aux_ti = {NULL};
        aux_ti.user_info = UserManager::NewUser();
        strcpy(aux_ti.user_info.username, (menu_items+LoginItems::USER_TI)->item.text_item.input_text.text);
        strcpy(aux_ti.user_info.password, (menu_items+LoginItems::PASSWD_TI)->item.text_item.input_text.text);

        aux_tn = TList::FindInList(*aux_user_list, aux_ti);
        if(aux_tn != nullptr){
            if(strcmp(aux_tn->info.user_info.password, aux_ti.user_info.password) == 0){
                if(GameManager::game_status.logged_user != nullptr && (strcmp(aux_tn->info.user_info.username, GameManager::game_status.logged_user->username) == 0)){
                    //In case is verifying a second player...
                    strcpy(error_dialog.text, "USER ALREADY LOGGED");
                    error_dialog_ltc = 0;
                }else{
                    if(aux_tn->info.user_info.credits <= 0){
                        strcpy(error_dialog.text, "NOT ENOUGH CREDITS CONTACT AN ADMIN");
                        error_dialog_ltc = 0;
                    }else{
                        *user = &(aux_tn->info.user_info);
                        is_verified = true;
                    }
                }
            }else{
                // printf("PASSWORD DOES NOT MATCH WITH USERNAME %s\n", aux_ti.user_info.username);
                strcpy(error_dialog.text, "INCORRECT PASSWORD");
                error_dialog_ltc = 0;
            }
        }else{
            // printf("USERNAME %s NOT FOUND\n", aux_ti.user_info.username);
            strcpy(error_dialog.text, "USERNAME NOT FOUND");
            error_dialog_ltc = 0;
        }
        UserManager::EmptyUserMemory(&aux_ti.user_info);

        return is_verified;
    }

    //ACTIONS
    void LoginAction(){
        UserManager::User* aux_p2 = nullptr;
        switch(prev_level){
            case GameManager::Level::PLAY_MENU:
                if(VerifyLogin(&(aux_p2))){
                    printf("LOGGED P2 -> %s %s \n", aux_p2->username, aux_p2->alias);
                    Gameplay::Load(GameManager::game_status.load_game_aux_gamemode, aux_p2);
                }
            break;
            default:
                if(VerifyLogin(&(GameManager::game_status.logged_user))){
                    printf("LOGGED USER -> %s %s ", GameManager::game_status.logged_user->username, GameManager::game_status.logged_user->alias);
                    if(GameManager::game_status.logged_user->is_admin){
                        printf("IS_ADMIN ");
                    }
                    printf("\n");
                    MainMenu::Load();
                }
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
        JMATH::Vec2 centered_bottom_coords = {Utils::kWindowWidth*0.5f, Utils::kWindowHeight-200.0f};
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
        JMATH::Vec2 centered_coords = {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f};
        JMATH::Vec2 margin_y = {0,50};
        UILib::UI_Item *aux_item = nullptr;
        centered_coords = JMATH::Vec2Sub(centered_coords, margin_y);
        

        aux_item = (menu_items + ((int)LoginItems::USER_TI));
        UILib::InitTextInput(
            &(aux_item->item.text_item),
            {
                //TagCollider P1
                JMATH::Vec2Sub(
                    JMATH::Vec2Sub(
                        centered_coords, 
                        JMATH::Vec2Scale(margin_y,3)
                    ), 
                    {(strlen(aux_item->item_name.text)/3.5f)*Utils::kBaseFontSize*2,0}
                ), 
                //TagCollider P2
                JMATH::Vec2Sum(
                    JMATH::Vec2Sub(
                        centered_coords, 
                        JMATH::Vec2Scale(margin_y,2)
                    ), 
                    {(strlen(aux_item->item_name.text)/3.5f)*Utils::kBaseFontSize*2,0}
                )
            },
            {
                //InputCollider P1
                JMATH::Vec2Sub(
                    JMATH::Vec2Sub(
                        centered_coords, 
                        JMATH::Vec2Scale(margin_y,1.5)
                    ), 
                    {((UserManager::kDefaultStrL+1)/3.0f)*Utils::kBaseFontSize,0}
                ), 
                //InputCollider P2
                JMATH::Vec2Sum(
                    JMATH::Vec2Sub(
                        centered_coords, 
                        JMATH::Vec2Scale(margin_y,0.5)
                    ), 
                    {((UserManager::kDefaultStrL+1)/3.0f)*Utils::kBaseFontSize,0}
                )
            },
            {255,255,255,255},
            {0,0,0,255},
            {{255,255,255,255},nullptr, (float)Utils::kBaseFontSize},
            true,
            true,
            false,
            false
        );

        aux_item = (menu_items + ((int)LoginItems::PASSWD_TI));
        UILib::InitTextInput(
            &(aux_item->item.text_item),
            {
                //TagCollider P1
                JMATH::Vec2Sub(
                    JMATH::Vec2Sum(
                        centered_coords, 
                        JMATH::Vec2Scale(margin_y,0)
                    ), 
                    {(strlen(aux_item->item_name.text)/3.5f)*Utils::kBaseFontSize*2,0}
                ), 
                //TagCollider P2
                JMATH::Vec2Sum(
                    JMATH::Vec2Sum(
                        centered_coords, 
                        JMATH::Vec2Scale(margin_y,1)
                    ), 
                    {(strlen(aux_item->item_name.text)/3.5f)*Utils::kBaseFontSize*2,0}
                )
            },
            {
                //InputCollider P1
                JMATH::Vec2Sub(
                    JMATH::Vec2Sum(
                        centered_coords, 
                        JMATH::Vec2Scale(margin_y,1.5)
                    ), 
                    {((UserManager::kDefaultStrL+1)/3.0f)*Utils::kBaseFontSize,0}
                ), 
                //InputCollider P2
                JMATH::Vec2Sum(
                    JMATH::Vec2Sum(
                        centered_coords, 
                        JMATH::Vec2Scale(margin_y,2.5)
                    ), 
                    {((UserManager::kDefaultStrL+1)/3.0f)*Utils::kBaseFontSize,0}
                )
            },
            {255,255,255,255},
            {0,0,0,255},
            {{255,255,255,255},nullptr, (float)Utils::kBaseFontSize},
            true,
            true,
            false,
            true
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
        error_dialog.text = (char*) malloc(sizeof(char)*51);
        Utils::StringFillWithChar(error_dialog.text,51,'\0', -1);
        InitMenuItems();
        InitTextInputs();
        InitButtons();
    }

    //LOGIN MENU LOAD
    
    void CleanForm(bool is_admin = false){
        strcpy((menu_items + LoginItems::USER_TI)->item.text_item.input_text.text, "\0");
        strcpy((menu_items + LoginItems::USER_TI)->item.text_item.pointer, "|\0");

        strcpy((menu_items + LoginItems::PASSWD_TI)->item.text_item.input_text.text, "\0");
        strcpy((menu_items + LoginItems::PASSWD_TI)->item.text_item.pointer, "|\0");
    }

    //Based on the level/screen you come from, the Login Menu will be loaded differently
    void Load(GameManager::Level level_p){
        prev_level = level_p;
        selected_item = -1;
        error_dialog_ltc = error_dialog_lt;

        CleanForm();
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
            esat::ResetBufferdKeyInput();
            if(selected_item <= 0){
                selected_item = ((int)LoginItems::TOTAL_ITEMS) - 1;
            }else{
                selected_item--;
            }
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_Down) || esat::IsSpecialKeyDown(esat::kSpecialKey_Tab)){
            esat::ResetBufferdKeyInput();
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
        ShowErrorDialog();
        
    }

    void EmptyMemory(){
        //printf("EmptyItemMemory FINO?\n");
        for(int i = 0; i < (int)LoginItems::TOTAL_ITEMS; i++){
            UILib::EmptyItemMemory(menu_items+i);
            // printf("EmptyItemMemory FINO\n");
        }    

        free(menu_items);
        // printf("menu_items FINO\n");
        free(error_dialog.text);
        // printf("error_dialog FINO\n");
    }
}