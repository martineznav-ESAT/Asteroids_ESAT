#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../CustomLibs/PolyLibJMATH.h"
#include "./Asteroids.h"

#ifndef PowerUps_H
#define PowerUps_H

namespace PowerUps{
    extern int last_pu_id;

    enum PU_Type{
        //SP

        //MP (Includes SP PowerUps)
        FRIENDLY_FIRE,
        TOTAL_PU_SP_TYPES = 0,
        TOTAL_PU_MP_TYPES = 1,
        TOTAL_PU_TYPES = 1,
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

    PowerUp NewPowerUp(PU_Type type, JMATH::Vec2 position);

    bool IsPowerUpActive(PowerUp powerUp);
    bool IsPowerUpBlinking(PowerUp powerUp);

    void GeneratePowerUp(Asteroids::Asteroid asteroid);

    void UpdatePowerUp(PowerUp *powerUp);

    void DrawPowerUp(PowerUp powerUp);

    void EmptyPowerUpMemory(PowerUp *powerUp);

    void EmptyMemory();

}

#endif
