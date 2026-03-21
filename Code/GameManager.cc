#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "./GameManager.h"
#include "./UserManager.h"

//Holds the game information needed globally, that may be needed for Level/Screen management 
namespace GameManager{
    GameStatus game_status;

    //Debug Level switch by F1-F7 keyboard input
    void DebugUpdate(){

        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F1)){
            game_status.level = LOGIN_MENU;
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F2)){
            game_status.level = REGISTER_MENU;
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F3)){
            game_status.level = MAIN_MENU;
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F4)){
            game_status.level = HIGHSCORES_MENU;
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F5)){
            game_status.level = ADMIN_MENU;
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F6)){
            game_status.level = PLAY_MENU;
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F7)){
            game_status.level = GAME;
        }
    }

}