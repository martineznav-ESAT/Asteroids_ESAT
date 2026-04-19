#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Code/UserManager.h"
#include "../CustomLibs/Utils.h"

#include "./GameplayTech.h"

namespace Players{
    JMATH::Vec3 *ship_coords = nullptr;

    void Init(){
        ship_coords = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3)*5);
        *(ship_coords+0) = {1.0f, 0.0f};      // 1
        *(ship_coords+1) = {-1.0f, -0.6f};    // 2
        *(ship_coords+2) = {-0.6f, -0.3f};    // 3
        *(ship_coords+3) = {-0.6f, 0.3f};     // 4
        *(ship_coords+4) = {-1.0f, 0.6f};     // 5
    }


    Ship NewShip(){
        Ship new_ship;
        PolyLibJMATH::InitPoly(
            &new_ship.figure,
            5,
            ship_coords,
            {25.0f,25.0f},
            -90.0f,
            {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f},
            {255,255,255},
            {0.1f,0.0f}
        );
        new_ship.fwd = {0.0f,0.0f};
        new_ship.speed_v = {0.0f,0.0f};
        new_ship.max_speed = 7.0f;
        new_ship.accel = 15.0f;
        new_ship.decel = 0.99f;
        new_ship.shots = (Shots::Shot*) malloc(sizeof(Shots::Shot)*max_player_shots);
        for(int i = 0; i < max_player_shots; i++){
            *(new_ship.shots+i) = Shots::NewShot();
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

    void RotateShip(Ship *ship, float degreesSecond){
        ship->figure.transform.rotation += degreesSecond/Utils::kFPS;
    }

    void AccelerateShip(Ship *ship){
        ship->speed_v = JMATH::Vec3Sum(ship->speed_v, JMATH::Vec3Scale(ship->fwd, ship->accel/Utils::kFPS));
        if(JMATH::Vec2Length(JMATH::Vec3ToVec2(ship->speed_v)) > ship->max_speed){
            ship->speed_v = JMATH::Vec3Scale(JMATH::Vec3Norm(ship->speed_v), ship->max_speed);
        }
    }

    void DecelerateShip(Ship *ship){
        ship->speed_v = JMATH::Vec3Scale(ship->speed_v, ship->decel);
    }

    JMATH::Vec2 GetShipHeadPoint(Ship *ship){
        return JMATH::Vec2Sum(
            ship->figure.transform.translation, 
            JMATH::Vec3ToVec2(JMATH::Vec3Scale(ship->fwd,ship->figure.transform.scale.x))
        );
    }

    void ShipShoot(Ship *ship){
        int i;
        bool exists_unshot = false;

        for(i = 0; i < max_player_shots && !exists_unshot; i++){
            exists_unshot = !(ship->shots+i)->is_active;
        }

        if(exists_unshot){
            i--;
            Shots::FireShot((ship->shots)+i, GetShipHeadPoint(ship), ship->figure.transform.rotation, ship->fwd, 10);
        }
    }

    void PlayerInput(Player* p, bool is_p1){
        if(is_p1){
            //PLAYER 1 INPUT CONTROL

            if(esat::IsKeyPressed('W')){
                // printf("MOVE FORWARD\n");
                AccelerateShip(&(p->ship));
            }
            
            if(esat::IsKeyPressed('A')){
                // printf("ROTATE LEFT\n");
                RotateShip(&(p->ship),-360);
            }

            if(esat::IsKeyPressed('D')){
                // printf("ROTATE RIGHT\n");
                RotateShip(&(p->ship),360);
            }

            if(esat::IsSpecialKeyDown(esat::SpecialKey::kSpecialKey_Space)){
                // printf("SHOOT\n");
                ShipShoot(&(p->ship));
            }

            //DEBUG INPUT
            if(esat::IsKeyDown('Q')){
                p->lifes--;
            }


        }else{
            //PLAYER 2 INPUT CONTROL

            if(esat::IsSpecialKeyPressed(esat::SpecialKey::kSpecialKey_Up)){
                // printf("MOVE FORWARD\n");
                AccelerateShip(&(p->ship));
            }
            DecelerateShip(&(p->ship));

            if(esat::IsSpecialKeyPressed(esat::SpecialKey::kSpecialKey_Left)){
                // printf("ROTATE LEFT\n");
                RotateShip(&(p->ship),-360);
            }

            if(esat::IsSpecialKeyPressed(esat::SpecialKey::kSpecialKey_Right)){
                // printf("ROTATE RIGHT\n");
                RotateShip(&(p->ship),360);
            }

            if(esat::IsSpecialKeyDown(esat::SpecialKey::kSpecialKey_Enter)){
                // printf("SHOOT\n");
                ShipShoot(&(p->ship));
            }

            //DEBUG INPUT
            if(esat::IsSpecialKeyDown(esat::SpecialKey::kSpecialKey_Backspace)){
                p->lifes--;
            }
        }
    }

    void UpdatePlayerShots(Players::Player* player){
        for(int i = 0; i < Players::max_player_shots; i++){
            if(((player->ship.shots)+i)->is_active){
                PolyLibJMATH::MovePoly(&(((player->ship.shots)+i)->bullet), (((player->ship.shots)+i)->speed_v));
                Collisions::BorderExitRellocation(&(((player->ship.shots)+i)->bullet));
                PolyLibJMATH::UpdatePoly(&(((player->ship.shots)+i)->bullet));
                ((player->ship.shots)+i)->lt_count += 1000/Utils::kFPS;
                if(((player->ship.shots)+i)->lt_count >= ((player->ship.shots)+i)->life_time){
                    ((player->ship.shots)+i)->is_active = false;
                }
            }
        }
    }

    void UpdateShipFwd(Ship* ship){
        float radianBase = JMATH::DegreesToRadians(360.0f/ship->figure.t_vertices);
        float radianRotation = JMATH::DegreesToRadians(ship->figure.transform.rotation);
        ship->fwd = {cosf(radianRotation), sinf(radianRotation)};
    }

    void UpdatePlayer(Players::Player* player, bool is_p1){
        UpdateShipFwd(&(player->ship));

        PlayerInput(player, is_p1);
        DecelerateShip(&(player->ship));
        // printf("SPEED V LENGTH = %.2f\n", JMATH::Vec3Length(player->ship.speed_v));
        // JMATH::Vec3Print(player->ship.speed_v);
        PolyLibJMATH::MovePoly(&(player->ship.figure), player->ship.speed_v);
        Collisions::BorderExitRellocation(&(player->ship.figure));
        PolyLibJMATH::UpdatePoly(&(player->ship.figure));
        UpdatePlayerShots(player);
    }


    void EmptyPlayerMemory(Player* player){
        for(int i = 0; i < max_player_shots; i++){
            PolyLibJMATH::EmptyPolyMemory(&((player->ship.shots+i)->bullet));
        }
        PolyLibJMATH::EmptyPolyMemory(&(player->ship.figure));
    }
    
}
