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
    extern JMATH::Vec3 *ship_coords;

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
        bool is_active;
        bool is_moving;
        int lifes;
        int score;
        int dead_lt;
        int dead_ltc;
        int inmunity_lt;
        int inmunity_ltc;
    };

    void Init();

    Ship NewShip();

    Player NewPlayer();

    void KillPlayer(Player* player);

    bool IsPlayerDead(Players::Player player);

    bool IsPlayerInmune(Players::Player player);

    void UpdatePlayer(Players::Player* player, bool is_p1);
    
    void DrawPlayerShots(Players::Player player);

    void DrawPlayer(Player player);

    void EmptyPlayerMemory(Player* player);
}

#endif
