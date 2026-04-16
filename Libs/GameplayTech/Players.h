#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Code/UserManager.h"

#include "../CustomLibs/PolyLibJMATH.h"
#include "../CustomLibs/JMATH.h"

#include "./Shots.h"

#ifndef Players_H
#define Players_H

namespace Players{
    const int max_player_shots = 5;

    struct Ship{
        PolyLibJMATH::Poly figure;
        JMATH::Vec3 fwd;
        JMATH::Vec3 speed_v;
        float max_speed;
        float accel, decel;
        Shots::Shot* shots;
    };

    struct Player{
        Ship ship;
        int lifes;
        int score;
    };

    void Init();

    Ship NewShip();

    Player NewPlayer();

    void UpdatePlayer(Players::Player* player, bool is_p1);

    void EmptyPlayerMemory(Player* player);
}

#endif
