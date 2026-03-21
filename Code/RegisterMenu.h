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

#ifndef RegisterMenu_H
#define RegisterMenu_H

namespace RegisterMenu{
    
    enum RegisterItems{
        USERNAME_TI,
        PASSWORD_TI,
        ALIAS_TI,
        EMAIL_TI,
        NAME_TI,
        SURNAME_TI,
        DOB_DAY_TI,
        DOB_MONTH_TI,
        DOB_YEAR_TI,
        COUNTRY_TI,
        PROVINCE_TI,
        CREDITS_TI,
        ADMIN_CHK,
        SAVE_BTN,
        BACK_BTN,
        TOTAL_ITEMS
    };

    void Init();

    void Load(GameManager::Level prev_level);

    void Update();

    void Draw();

    void EmptyMemory();
}

#endif
