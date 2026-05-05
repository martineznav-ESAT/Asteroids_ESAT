#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Code/UserManager.h"

#include "../CustomLibs/PolyLibJMATH.h"
#include "../CustomLibs/JMATH.h"

#include "./Shots.h"
#include "./Particles.h"
#include "./PowerUps.h"


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
        Particles::Particle* death_particles;
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
        int round = 0;
        int consecutive_hs = 0;
        PowerUps::PowerUpTag* pu_tags;
    };

    void Init();

    Ship NewShip();

    Player NewPlayer();

    void AddPoints(Player* player, int points);

    void RespawnPlayer(Player* player);

    void KillPlayer(Player* player);

    bool IsPlayerDead(Players::Player player);

    bool IsPlayerImmune(Players::Player player);

    void UpdatePlayer(Players::Player* player);
    
    void DrawPlayerShots(Players::Player player);

    void DrawPlayer(Player player);

    void EmptyPlayerMemory(Player* player);
}

#endif
