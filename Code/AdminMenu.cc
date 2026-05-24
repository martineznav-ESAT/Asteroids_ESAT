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
#include "../Libs/CustomLibs/AudioLib.h"

#include "../Libs/GameplayTech/GameplayTech.h"

#include "./GameManager.h"
#include "./UserManager.h"
#include "./AdminMenu.h"
#include "./MainMenu.h"
#include "./RegisterMenu.h"

namespace AdminMenu{
    //Memory block that holds all the menu items no matter if they are visible or not.
    UILib::UI_Item *menu_items = nullptr;
    int selected_item = -1;

    TList::ListNode *user_page = nullptr;
    int page_number = 0;
    bool is_last_page = false;

    //Loads the users for the specific page.
    //More precisely, puts the ListNode corresponding to the user_page delivered as parameter as the head of the page
    //and updates the corresponding buttons.
    void LoadUserPage(int page = 0){
        TList::ListNode** aux_list = (TList::ListNode**) &(UserManager::user_list);
        TList::ListNode *aux_user_node = nullptr;

        user_page = TList::GetIndexListNode((TList::ListNode*) *aux_list, page_number*10);

        for(int i = 0, u = 0; i < (int)AdminMenuItems::PREV_PAG_BTN; i++){
            aux_user_node = TList::GetIndexListNode(user_page,u);
            if(u < TList::ListLength(user_page)){
                //Both buttons visible in case the user is not an admin
                //In case the user is an admin, the edit button will be hidden and, in case the
                //admin is the same as the one logged in, the delete button will be hidden as well
                (menu_items+i)->item.btn_pa_item.is_visible = 
                (
                    !aux_user_node->info.user_info.is_admin ||
                    aux_user_node->info.user_info.is_admin && i%2 != 0 && strcmp(aux_user_node->info.user_info.username, GameManager::game_status.logged_user->username) != 0
                );
            }else{
                (menu_items+i)->item.btn_pa_item.is_visible = false;
            }

            if((menu_items+i)->item.btn_pa_item.is_visible){
                (menu_items+i)->item.btn_pa_item.action_p = &(aux_user_node->info.user_info);
            }

            if(i%2 != 0){
                u++;
            }
        }

        is_last_page = TList::ListLength(user_page) <= 10;
    }

    //ACTIONS

    //Loads the register menu in admin edit mode for the given user
    void EditAction(void *u){
        RegisterMenu::Load(GameManager::Level::ADMIN_MENU, (UserManager::User*) u);
    }

    //Deletes the given user from the user list
    void DeleteAction(void *u){
        TList::ListNode** aux_list = (TList::ListNode**) &(UserManager::user_list);
        TList::ListInfo aux_info = {NULL};
        aux_info.user_info = *((UserManager::User*) u);
        
        if(TList::ListLength(user_page) <= 1){
            //The actual page will be empty. So before deleting the element
            //nd update/save the deletion, the previous page is loaded 
            //so user_page does not miss its pointer
            LoadUserPage(--page_number);
        }
        TList::DeleteElement(aux_list, aux_info);
        TList::SaveList(aux_list, UserManager::user_list_dat, UserManager::user_list_dat_path);
        LoadUserPage(page_number);
    }

    //Loads the previous page
    void PrevPageAction(){
        page_number--;
        LoadUserPage(page_number);
    }

    //Loads the next page
    void NextPageAction(){
        page_number++;
        LoadUserPage(page_number);
    }

    //Opens the register menu in creation mode with admin permission
    void CreateAction(){
        RegisterMenu::Load(GameManager::Level::ADMIN_MENU);
    }

    //Returns to the previous menu, in this case the main menu
    void BackAction(){
        MainMenu::Load();
    }

    //ADMIN MENU INIT

    //Initializes all menu buttons
    void InitButtons(){
        Utils::Color text_color = {255,255,255,255};
        float list_font_size = Utils::kBaseFontSize;
        TList::ListNode *aux_user_node = nullptr;

        JMATH::Vec2 base_button_coord = {
            (Utils::kWindowWidth) - (strlen("EDIT DELETE  ") * list_font_size),
            232.0f
        };

        JMATH::Vec2 button_height = {0,25};
        JMATH::Vec2 margin_v = {0,27.5f};
        JMATH::Vec2 edit_width = {(strlen("EDIT")*list_font_size),0};
        JMATH::Vec2 delete_width_1 = {(strlen("EDIT ")*list_font_size),0};
        JMATH::Vec2 delete_width_2 = {(strlen("EDITDELETE")*list_font_size),0};

        //USER MANAGEMENT BUTTONS
        for(int i = 0, u = 0; i < (int)AdminMenuItems::PREV_PAG_BTN; i++){
            aux_user_node = TList::GetIndexListNode(user_page,u);
            if(i%2 == 0){
                UILib::InitButtonPA(
                    &((menu_items+i)->item.btn_pa_item),
                    {
                        JMATH::Vec2Sum(base_button_coord, JMATH::Vec2Scale(margin_v,i)),
                        JMATH::Vec2Sum(base_button_coord, JMATH::Vec2Sum(button_height,JMATH::Vec2Sum(edit_width, JMATH::Vec2Scale(margin_v,i)))),
                    },
                    {100,100,100,200},
                    {100,100,100,200},
                    {
                        text_color,
                        (menu_items+i)->item_name.text,
                        list_font_size
                    },
                    aux_user_node != nullptr,
                    EditAction,
                    (aux_user_node == nullptr ? nullptr : &(aux_user_node->info.user_info))
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
                    aux_user_node != nullptr,
                    DeleteAction,
                    (aux_user_node == nullptr ? nullptr : &(aux_user_node->info.user_info))
                );

                u++;
            }
        }

        UILib::InitButton(
            &((menu_items+((int)AdminMenuItems::PREV_PAG_BTN))->item.btn_item),
            {
                {50.0f, Utils::kWindowHeight-175.0f},
                {100.0f, Utils::kWindowHeight-125.0f}
            },
            {100,100,100,200},
            {100,100,100,200},
            {
                text_color,
                (menu_items+((int)AdminMenuItems::PREV_PAG_BTN))->item_name.text,
                list_font_size
            },
            true,
            PrevPageAction
        );

        UILib::InitButton(
            &((menu_items+((int)AdminMenuItems::NEXT_PAG_BTN))->item.btn_item),
            {
                {Utils::kWindowWidth-100.0f, Utils::kWindowHeight-175.0f},
                {Utils::kWindowWidth-50.0f, Utils::kWindowHeight-125.0f}
            },
            {100,100,100,200},
            {100,100,100,200},
            {
                text_color,
                (menu_items+((int)AdminMenuItems::NEXT_PAG_BTN))->item_name.text,
                list_font_size
            },
            true,
            NextPageAction
        );

        UILib::InitButton(
            &((menu_items+((int)AdminMenuItems::CREATE_BTN))->item.btn_item),
            {
                {50.0f, Utils::kWindowHeight-80.0f},
                {150.0f, Utils::kWindowHeight-30.0f}
            },
            {100,100,100,200},
            {100,100,100,200},
            {
                text_color,
                (menu_items+((int)AdminMenuItems::CREATE_BTN))->item_name.text,
                list_font_size
            },
            true,
            CreateAction
        );

        UILib::InitButton(
            &((menu_items+((int)AdminMenuItems::BACK_BTN))->item.btn_item),
            {
                {Utils::kWindowWidth-150.0f, Utils::kWindowHeight-80.0f},
                {Utils::kWindowWidth-50.0f, Utils::kWindowHeight-30.0f}
            },
            {100,100,100,200},
            {100,100,100,200},
            {
                text_color,
                (menu_items+((int)AdminMenuItems::BACK_BTN))->item_name.text,
                list_font_size
            },
            true,
            BackAction
        );

    }


    //Initializes all UI items
    void InitMenuItems(){

        // text_inputs = (UILib::TextInput*) malloc(sizeof(UILib::TextInput)*(int)AdminMenuItems::TOTAL_ITEMS);
        menu_items = (UILib::UI_Item*) malloc(sizeof(UILib::UI_Item)*(int)AdminMenuItems::TOTAL_ITEMS);

        //USER MANAGEMENT BUTTONS
        for(int i = 0; i < (int)AdminMenuItems::PREV_PAG_BTN; i++){
            if(i%2 == 0){
                UILib::InitItem(
                    (menu_items + i),
                    UILib::ItemType::BUTTON_PA,
                    {{255,255,255,255},"EDIT", (float)Utils::kBaseFontSize}
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
            (menu_items + ((int)AdminMenuItems::PREV_PAG_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"<", Utils::kBaseFontSize*2.0f}
        );

        UILib::InitItem(
            (menu_items + ((int)AdminMenuItems::NEXT_PAG_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},">", Utils::kBaseFontSize*2.0f}
        );

        UILib::InitItem(
            (menu_items + ((int)AdminMenuItems::CREATE_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"CREATE", Utils::kBaseFontSize*2.0f}
        );

        UILib::InitItem(
            (menu_items + ((int)AdminMenuItems::BACK_BTN)),
            UILib::ItemType::BUTTON,
            {{255,255,255,255},"BACK", Utils::kBaseFontSize*2.0f}
        );
    }

    //Whole Admin Menu initializer
    void Init(){
        InitMenuItems();
        InitButtons();
    }

    //ADMIN MENU LOAD

    //Loads the Admin menu
    void Load(){
        page_number = 0;
        selected_item = -1;
        LoadUserPage();
        GameManager::game_status.level = GameManager::Level::ADMIN_MENU;
    }

    //ADMIN MENU UPDATE

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

            AudioLib::PlaySound(AudioLib::SoundsType::ACTION);

            do{
                if(selected_item <= 0){
                    selected_item = ((int)AdminMenuItems::TOTAL_ITEMS) - 1;
                }else{
                    selected_item--;
                }
            }while(!UILib::IsItemVisible(*(menu_items+selected_item)));
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_Down) || esat::IsSpecialKeyDown(esat::kSpecialKey_Tab)){
            AudioLib::PlaySound(AudioLib::SoundsType::ACTION);

            do{
                ++selected_item %= (int)AdminMenuItems::TOTAL_ITEMS;
            }while(!UILib::IsItemVisible(*(menu_items+selected_item)));
        }

        for(int i = 0; i < (int)AdminMenuItems::TOTAL_ITEMS; i++){
            switch((AdminMenuItems)i){
                case AdminMenuItems::PREV_PAG_BTN:
                    if(page_number > 0){
                        UILib::UpdateItem(menu_items+i, &selected_item, i);
                    }
                break;
                case AdminMenuItems::NEXT_PAG_BTN:
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

    //ADMIN MENU DRAW

    //Draws the menu main title
    void DrawTitle(){
        UILib::Text title = {
            {255,255,255,255},
            "REGISTERED USERS",
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

    //Draws the menu items related with a given user
    void DrawUserItem(JMATH::Vec2 coord, float font_size, UserManager::User user){
        UILib::DrawText(
            coord,
            {
                {255,255,255,255},
                user.username,
                font_size
            }
        );

        UILib::DrawText(
            JMATH::Vec2Sum(coord, {font_size*strlen("0123456789    "),0}),
            {
                {255,255,255,255},
                user.alias,
                font_size
            }
        );

        UILib::DrawIntToText(
            JMATH::Vec2Sum(coord, {font_size*strlen("0123456789   ALIAS  "),0}),
            {
                {255,255,255,255},
                nullptr,
                font_size
            },
            ((int)user.credits),2,true
        );

        if(user.is_admin){
            UILib::DrawText(
                JMATH::Vec2Sum(coord, {font_size*strlen("0123456789   ALIAS  CREDITS"),0}),
                {
                    {255,255,255,255},
                    "X",
                    font_size
                }
            );
        }

    }

    //Draws all the menu items related for all the users in the current page
    void DrawUserItems(JMATH::Vec2 base_coord, JMATH::Vec2 margin_v, float list_font_size){
        TList::ListNode* aux_list = user_page;

        //Starts with 1 to apply margin from the begining and draws a maximum of 10
        for(int i = 0; aux_list != nullptr && i < 10; aux_list = aux_list->next ,i++){
            DrawUserItem(JMATH::Vec2Sum(base_coord,JMATH::Vec2Scale(margin_v,i+1)), list_font_size, aux_list->info.user_info);
        }
    }

    //Draws the whole user list table of the current
    void DrawUserList(){
        float list_font_size = Utils::kBaseFontSize * 1.5f;

        JMATH::Vec2 base_coord = {
            (Utils::kWindowWidth*0.5f) -
            (strlen("USERNAME             ALIAS   CREDITS   ADMIN") * 0.45f * list_font_size),
            200
        };
        JMATH::Vec2 margin_v = {0,55};

        UILib::DrawText(
            base_coord,
            {
                {255,255,255,255},
                "USERNAME               ALIAS   CREDITS      ADMIN",
                {list_font_size}
            }
        );

        DrawUserItems(base_coord, margin_v, list_font_size);
    }

    //Draws all the interactive menu items based on their current state
    void DrawMenuItems(){
        for(int i = 0; i < (int)AdminMenuItems::TOTAL_ITEMS; i++){
            switch((AdminMenuItems)i){
                case AdminMenuItems::PREV_PAG_BTN:
                    if(page_number <= 0){
                        (menu_items+i)->item.btn_item.is_visible = false;
                    }else{
                        (menu_items+i)->item.btn_item.is_visible = true;
                    }
                break;
                case AdminMenuItems::NEXT_PAG_BTN:
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
        DrawUserList();
        DrawMenuItems();
    }

    //Releases all the AdminMenu.cc dynamic memory used
    void EmptyMemory(){
        for(int i = 0; i < (int)AdminMenuItems::TOTAL_ITEMS; i++){
            UILib::EmptyItemMemory(menu_items+i);
        }
        free(menu_items);
    }
}