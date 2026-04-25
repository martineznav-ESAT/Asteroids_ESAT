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
#include "./Gameplay.h"

//Holds the game information needed globally, that may be needed for Level/Screen management 
namespace GameManager{
    GameStatus game_status = {
        LOGIN_MENU,
        nullptr,
        nullptr,
        PlayedGames::Gamemode::SP
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

    void AlternateActivePlayer(){
        if(game_status.actual_game->gamemode == PlayedGames::Gamemode::MP_ALT){
            game_status.actual_game->is_player1_turn = !game_status.actual_game->is_player1_turn;
            
            if(game_status.actual_game->is_player1_turn){
                game_status.actual_game->p1.is_active = true;
                game_status.actual_game->p2.is_active = false;
            }else{
                game_status.actual_game->p1.is_active = false;
                game_status.actual_game->p2.is_active = true;
            }
            Gameplay::LoadGameplayLevel(true);
        }
    }

    bool IsPlayer1(Players::Player *player){
        return &(GameManager::game_status.actual_game->p1) == player;
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