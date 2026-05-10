#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../CustomLibs/PolyLibJMATH.h"
// #include "./Asteroids.h"

#ifndef PowerUps_H
#define PowerUps_H

namespace PowerUps{
    extern int last_pu_id;

    enum PU_Type{
        //SP
        SHOTGUN,

        //MP (Includes SP PowerUps)
        FRIENDLY_FIRE,
        TOTAL_PU_SP_TYPES = 1,
        TOTAL_PU_MP_TYPES = 2,
        TOTAL_PU_TYPES = 2,
    };

    enum FFIconsVertices{
        V_ICON_1 = 9,
        V_ICON_2 = 6
    };

    enum FFIcons{
        FF_ICON_1,
        FF_ICON_2,
        FF_TOTAL_ICONS
    };

    enum ShotgunIcons{
        SG_ICON_1,
        SG_ICON_2,
        SG_ICON_3,
        SG_TOTAL_ICONS
    };

    struct PowerUp{
        int id = 0;
        PolyLibJMATH::Poly base_figure;
        PolyLibJMATH::Poly *icon_figures = nullptr;
        PU_Type type;
        float duration_lt, duration_ltc;
    };

    struct PowerUpTag{
        PU_Type type;
        float duration_lt, duration_ltc;
    };

    void Init();

    PowerUp NewPowerUp(PU_Type type, JMATH::Vec2 position);
    
    PowerUpTag NewPowerUpTag(PU_Type type);
    
    bool IsPowerUpActive(PowerUp powerUp);
    bool IsPowerUpActive(PowerUpTag powerUpTag);
    
    bool IsPowerUpBlinking(PowerUp powerUp);

    void GeneratePowerUp(void* asteroid);

    void UpdatePowerUp(PowerUp *powerUp);

    void UpdatePowerUpTag(PowerUpTag *powerUpTag);

    void DrawPowerUp(PowerUp powerUp);

    void EmptyPowerUpMemory(PowerUp *powerUp);

    void EmptyMemory();

}

#endif
