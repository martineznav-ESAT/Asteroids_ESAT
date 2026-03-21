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
#include "./GameManager.h"

#ifndef PlayMenu_H
#define PlayMenu_H

namespace PlayMenu{
    
    enum PlayMenuItems{
        SINGLEPLAYER_BTN,
        MULTIPLAYER_BTN,
        TOTAL_ITEMS
    };

    void Init();

    void Load();

    void Update();

    void Draw();

    
    void EmptyMemory();
}

#endif
