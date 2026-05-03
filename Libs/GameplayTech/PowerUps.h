#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../CustomLibs/PolyLibJMATH.h"

#ifndef PowerUps_H
#define PowerUps_H

namespace PowerUps{
    extern int last_pu_id;

    enum PU_Type{
        FRIENDLY_FIRE,
        TOTAL_PU_TYPES
    };

    enum FFIconsVertices{
        V_ICON_1 = 9,
        V_ICON_2 = 6
    };

    enum FFIcons{
        ICON_1,
        ICON_2,
        TOTAL_ICONS
    };

    struct PowerUp{
        int id = 0;
        PolyLibJMATH::Poly base_figure;
        PolyLibJMATH::Poly *icon_figures = nullptr;
        PU_Type type;
        float duration_lt, duration_ltc;
    };

    void Init();

    PowerUp NewPowerUp(PU_Type type);

    bool IsPowerUpActive(PowerUp powerUp);
    bool IsPowerUpBlinking(PowerUp powerUp);

    void UpdatePowerUp(PowerUp *powerUp);

    void DrawPowerUp(PowerUp powerUp);

    void EmptyPowerUpMemory(PowerUp *powerUp);

    void EmptyMemory();

}

#endif
