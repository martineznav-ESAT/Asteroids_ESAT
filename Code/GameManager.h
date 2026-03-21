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
        GAME,
        QUIT
    };

    //Gamemode enum 
    enum Gamemode{
        ONE_PLAYER,
        TWO_PLAYER_COOP,
        TWO_PLAYER_VS,
    };

    struct Player{
        // Ship ship
        int lifes;
        int score;
    };

    struct PlayedGame{
        Gamemode gm = ONE_PLAYER;
        UserManager::User p1_user;
        UserManager::User p2_user;
        Player p1;
        Player p2;
        bool is_ended;
    };

    struct GameStatus{
        Level level = LOGIN_MENU;
        UserManager::User actual_user;
    };

    extern GameStatus game_status;

    void DebugUpdate();
}

#endif