#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "./UserManager.h"
#include "../Libs/GameplayTech/GameplayTech.h"

#ifndef GameManager_H
#define GameManager_H

//Holds the game information needed globally, that may be needed to save and the Level/Screen management 
namespace GameManager{

    //Level/Screen enum 
    enum Level{
        LOGIN_MENU,
        REGISTER_MENU,
        MAIN_MENU,
        PLAY_MENU,
        HIGHSCORES_MENU,
        ADMIN_MENU,
        GAMEPLAY,
        QUIT
    };

    struct GameStatus{
        Level level = LOGIN_MENU;
        UserManager::User *logged_user;
        PlayedGames::PlayedGame *actual_game;
        PlayedGames::Gamemode load_game_aux_mp_gamemode;
    };

    extern GameStatus game_status;

    void LoadInitLevel();

    void AlternateActivePlayer();

    bool IsPlayer1(Players::Player *player);

    void DebugUpdate();
}

#endif