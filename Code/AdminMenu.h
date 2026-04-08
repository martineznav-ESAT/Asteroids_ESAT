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

#ifndef AdminMenu_H
#define AdminMenu_H

namespace AdminMenu{
    
    enum AdminMenuItems{
        EDIT_0_BTN,
        DELETE_0_BTN,
        EDIT_1_BTN,
        DELETE_1_BTN,
        EDIT_2_BTN,
        DELETE_2_BTN,
        EDIT_3_BTN,
        DELETE_3_BTN,
        EDIT_4_BTN,
        DELETE_4_BTN,
        EDIT_5_BTN,
        DELETE_5_BTN,
        EDIT_6_BTN,
        DELETE_6_BTN,
        EDIT_7_BTN,
        DELETE_7_BTN,
        EDIT_8_BTN,
        DELETE_8_BTN,
        EDIT_9_BTN,
        DELETE_9_BTN,
        PREV_PAG_BTN,
        NEXT_PAG_BTN,
        CREATE_BTN,
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
