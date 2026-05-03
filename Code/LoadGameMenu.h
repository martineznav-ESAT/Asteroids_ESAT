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

#include "../Libs/GameplayTech/GameplayTech.h"

#ifndef LoadGameMenu_H
#define LoadGameMenu_H

namespace LoadGameMenu{
    
    enum LoadGameMenuItems{
        PLAY0_BTN,
        DELETE_0_BTN,
        PLAY1_BTN,
        DELETE_1_BTN,
        PLAY2_BTN,
        DELETE_2_BTN,
        PLAY3_BTN,
        DELETE_3_BTN,
        PLAY4_BTN,
        DELETE_4_BTN,
        PLAY5_BTN,
        DELETE_5_BTN,
        PLAY6_BTN,
        DELETE_6_BTN,
        PLAY7_BTN,
        DELETE_7_BTN,
        PLAY8_BTN,
        DELETE_8_BTN,
        PLAY9_BTN,
        DELETE_9_BTN,
        PREV_PAG_BTN,
        NEXT_PAG_BTN,
        BACK_BTN,
        TOTAL_ITEMS
    };

    void Init();

    void Load();

    void Update();

    void Draw();

    
    void EmptyMemory();
}

#endif
