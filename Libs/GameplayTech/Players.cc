#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Code/UserManager.h"
#include "../CustomLibs/Utils.h"

#include "./Players.h"

namespace Players{
    JMATH::Vec3 *ship_coords = nullptr;

    void Init(){
        ship_coords = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3)*5);
        *(ship_coords+0) = {0.0f,-1.0f};     // 1
        *(ship_coords+1) = {-0.6f,1.0f};   // 2
        *(ship_coords+2) = {-0.3f,0.6f};   // 3
        *(ship_coords+3) = {0.3f,0.6f};    // 4
        *(ship_coords+4) = {0.6f,1.0f};    // 5
    }

    Shot NewShot(){
        Shot new_shot;
        PolyLibJMATH::InitPoly(
            &new_shot.bullet,
            4,
            {2.0f,2.0f},
            45.0f,
            {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f},
            {255,255,255},
            {0.0f,0.0f}
        );
        new_shot.speed_v = {0.0f,0.0f};
        new_shot.life_time = 2000; //ms
        new_shot.lt_count = 0; //ms timer
        new_shot.is_active = false;
        return new_shot;
    }

    Ship NewShip(){
        Ship new_ship;
        PolyLibJMATH::InitPoly(
            &new_ship.figure,
            5,
            ship_coords,
            {20.0f,20.0f},
            0.0f,
            {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f},
            {255,255,255},
            {0.0f,-0.4f}
        );
        new_ship.fwd = {0.0f,0.0f};
        new_ship.speed_v = {0.0f,0.0f};
        new_ship.max_speed = 3.0f;
        new_ship.accel = 0.1f;
        new_ship.decel = 0.1f;
        new_ship.shots = (Shot*) malloc(sizeof(Shot)*max_player_shots);
        for(int i = 0; i < max_player_shots; i++){
            *(new_ship.shots+i) = NewShot();
        }
        return new_ship;
    }

    Player NewPlayer(){
        Player new_player;
        new_player.ship = NewShip();
        new_player.lifes = 3;
        new_player.score = 0;

        return new_player;
    }

    void EmptyPlayerMemory(Player* player){
        for(int i = 0; i < max_player_shots; i++){
            PolyLibJMATH::EmptyPolyMemory(&((player->ship.shots+i)->bullet));
        }
        PolyLibJMATH::EmptyPolyMemory(&(player->ship.figure));
    }
}
