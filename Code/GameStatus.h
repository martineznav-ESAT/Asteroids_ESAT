#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef GameStatus_H
#define GameStatus_H

//Holds the game information needed globally, that may be needed to save and the Level/Screen management 
namespace GameStatus{

    //Level/Screen enum 
    enum Level{
        LOGIN_MENU,
        REGISTER_MENU,
        MAIN_MENU,
        HIGHSCORES,
        ADMIN_MENU,
        PLAY_MENU,
        GAME
    };

    struct GameStatus{
        Level level = LOGIN_MENU;
    };

    extern GameStatus game_status;

    void DebugUpdate();
}

#endif