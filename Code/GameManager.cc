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
#include "./LoginMenu.h"
#include "./RegisterMenu.h"
#include "./MainMenu.h"
#include "./PlayMenu.h"
#include "./HighscoresMenu.h"
#include "./AdminMenu.h"

//Holds the game information needed globally, that may be needed for Level/Screen management 
namespace GameManager{
    GameStatus game_status = {
        LOGIN_MENU,
        nullptr,
        // PlayedGames::PlayedGame actual_game;
    };

    void LoadInitLevel(){
        // printf("LoadRegisteredUsers\n");
        if (UserManager::LoadRegisteredUsers()){
            // printf("LoginMenu\n");
            LoginMenu::Load(GameManager::Level::LOGIN_MENU);
        }else{
            // printf("RegisterMenu\n");
            RegisterMenu::Load(GameManager::Level::REGISTER_MENU);
        }
    }

    //Debug Level switch by F1-F7 keyboard input
    void DebugUpdate(){
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F1)){
            LoginMenu::Load(game_status.level);
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F2)){
            RegisterMenu::Load(game_status.level);
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F3)){
            MainMenu::Load();
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F4)){
            PlayMenu::Load();
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F5)){
            HighscoresMenu::Load();
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F6)){
            AdminMenu::Load();
        }
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_F7)){
            game_status.level = GAMEPLAY;
        }
    }

}