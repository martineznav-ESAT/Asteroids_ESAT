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

#ifndef LoginMenu_H
#define LoginMenu_H

namespace LoginMenu{
    
    enum LoginItems{
        USER_TI,
        PASSWD_TI,
        LOGIN_BTN,
        VARIABLE_BTN,
        TOTAL_ITEMS
    };

    void Init();

    void Load(GameStatus::Level prev_level);

    void Update();

    void Draw();

    
    void EmptyMemory();
}

#endif
