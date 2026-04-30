#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../CustomLibs/PolyLibJMATH.h"

#ifndef PowerUps_H
#define PowerUps_H

namespace PowerUps{
    enum PU_Type{
        FRIENDLY_FIRE,
        TOTAL_PU_TYPES
    };

    struct PowerUp{
        PolyLibJMATH::Poly base_figure;
        PolyLibJMATH::Poly icon_figure;
        PU_Type type;
        float duration_lt, duration_ltc;
    };

    PowerUp NewPowerUp();

    void UpdatePowerUp(PowerUp *powerUp);

    void UpdatePowerUp(PowerUp *powerUp);

    void DrawPowerUp(PowerUp powerUp);
}

#endif
