#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "../Libs/CustomLibs/Utils.h"
#include "../Libs/CustomLibs/UILib.h"

#include "./GameStatus.h"
#include "./LoginMenu.h"

namespace LoginMenu{
    //Memory block that holds all the buttons no matter if they are visible or not.
    UILib::Button *buttons = nullptr;
    UILib::TextInput *text_inputs = nullptr;

    //Button Actions
    void LoginAction(){
        
    }

    void RegisterAction(){
        
    }
    
    //Given a button as parameter, fills it with the rest of the parameters. Created mainly for readability
    void InitButton(UILib::Button *b, Utils::Collider coll, Utils::Color color, UILib::Text b_text, bool is_visible, void (*action)()){
        *b = {
            coll,
            color,
            b_text,
            is_visible,
            action
        };
    }

    //Initializes all posible buttons
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

        buttons = (UILib::Button*) malloc(sizeof(UILib::Button)*(int)LoginButtons::TOTAL_BUTTONS);

        InitButton(
            (buttons+((int)LoginButtons::LOGIN)),
            left_button,
            button_color,
            {
                text_color,
                "LOGIN",
                (float)Utils::kBaseFontSize
            },
            true,
            nullptr
        );

        InitButton(
            (buttons+((int)LoginButtons::REGISTER)),
            right_button,
            button_color,
            {
                text_color,
                "REGISTER",
                (float)Utils::kBaseFontSize
            },
            true,
            nullptr
        );

        InitButton(
            (buttons+((int)LoginButtons::BACK)),
            right_button,
            button_color,
            {
                text_color,
                "BACK",
                (float)Utils::kBaseFontSize
            },
            false,
            nullptr
        );

        InitButton(
            (buttons+((int)LoginButtons::BACK_ADMIN)),
            right_button,
            button_color,
            {
                text_color,
                "BACK ADMIN",
                (float)Utils::kBaseFontSize
            },
            false,
            nullptr
        );
    }

    //Whole Login Menu initializer
    void Init(){
        InitButtons();
    }

    //Based on the level/screen you come from, the Login Menu will be loaded differently
    void Load(GameStatus::Level prev_level){
        switch(prev_level){
            case GameStatus::Level::PLAY_MENU:
                (buttons+((int)LoginButtons::REGISTER))->is_visible = false;
                (buttons+((int)LoginButtons::BACK))->is_visible = true;
                (buttons+((int)LoginButtons::BACK_ADMIN))->is_visible = false;
            break;

            case GameStatus::Level::ADMIN_MENU:
                (buttons+((int)LoginButtons::REGISTER))->is_visible = false;
                (buttons+((int)LoginButtons::BACK))->is_visible = false;
                (buttons+((int)LoginButtons::BACK_ADMIN))->is_visible = true;
            break;
            
            default:
                (buttons+((int)LoginButtons::REGISTER))->is_visible = true;
                (buttons+((int)LoginButtons::BACK))->is_visible = false;
                (buttons+((int)LoginButtons::BACK_ADMIN))->is_visible = false;
            break;
        }
    }

    //Whole Login Menu update method
    void Update(){
        UILib::ButtonUpdate(buttons, (int)LoginButtons::TOTAL_BUTTONS);
    }

    //Whole Login Menu draw method
    void DrawButtons(){
        for(int i = 0; i < (int)LoginButtons::TOTAL_BUTTONS; i++){
            UILib::DrawButton(*(buttons+i));
        }
    }

    void Draw(){
        DrawButtons();
    }
}