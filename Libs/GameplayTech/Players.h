#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Code/UserManager.h"

#include "../CustomLibs/PolyLibJMATH.h"
#include "../CustomLibs/JMATH.h"

#ifndef Players_H
#define Players_H

namespace Players{
    const int max_player_shots = 5;

    struct Shot{
        PolyLibJMATH::Poly bullet;
        JMATH::Vec3 speed_v;
        float life_time, lt_count;
        bool is_active;
    };

    struct Ship{
        PolyLibJMATH::Poly figure;
        JMATH::Vec3 fwd;
        JMATH::Vec3 speed_v;
        float max_speed;
        float accel, decel;
        Shot* shots;
    };

    struct Player{
        Ship ship;
        int lifes;
        int score;
    };

    void Init();

    Shot NewShot();

    Ship NewShip();

    Player NewPlayer();
}

#endif
