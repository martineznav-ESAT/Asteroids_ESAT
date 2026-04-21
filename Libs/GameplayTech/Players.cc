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
            PolyLibJMATH::UpdatePoly(&((new_ship.shots+i)->bullet));
        }
        return new_ship;
    }

    Player NewPlayer(){
        Player new_player;
        new_player.ship = NewShip();
        new_player.is_active = true;
        new_player.is_moving = false;
        new_player.lifes = 4;
        new_player.score = 0;
        new_player.dead_lt = 3000;
        new_player.dead_ltc = 3000;
        new_player.inmunity_lt = 2000;
        new_player.inmunity_ltc = 2000;

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
            Shots::FireShot((ship->shots)+i, GetShipHeadPoint(ship), ship->figure.transform.rotation, ship->fwd, ship->max_speed*3);
        }
    }

    void RespawnPlayer(Player* player){
        player->inmunity_ltc = 0;

        player->ship.figure.transform.rotation = -90.0f;
        player->ship.figure.transform.translation = {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f};

        player->ship.fwd = {0.0f,0.0f};
        player->ship.speed_v = {0.0f,0.0f};
        player->ship.max_speed = 7.0f;
        player->ship.accel = 15.0f;
        player->ship.decel = 0.99f;
    }

    void KillPlayer(Player* player){
        player->is_active = false;
        player->dead_ltc = 0;
        player->lifes--;
        RespawnPlayer(player);
    }

    void PlayerInput(Player* p, bool is_p1){
        if(is_p1){
            //PLAYER 1 INPUT CONTROL

            if(esat::IsKeyPressed('W')){
                // printf("MOVE FORWARD\n");
                p->is_moving = true;
                AccelerateShip(&(p->ship));
            }else{
                p->is_moving = false;
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

            if(esat::IsKeyPressed('G')){
                // printf("HYPERSPACE\n");
            }


            //DEBUG INPUT
            if(esat::IsKeyDown('Q')){
                KillPlayer(p);
            }


        }else{
            //PLAYER 2 INPUT CONTROL

            if(esat::IsSpecialKeyPressed(esat::SpecialKey::kSpecialKey_Up)){
                // printf("MOVE FORWARD\n");
                p->is_moving = true;
                AccelerateShip(&(p->ship));
            }else{
                p->is_moving = false;
            }

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

            if(esat::IsKeyPressed('L')){
                // printf("HYPERSPACE\n");
            }

            //DEBUG INPUT
            if(esat::IsSpecialKeyDown(esat::SpecialKey::kSpecialKey_Backspace)){
                KillPlayer(p);
            }
        }
    }

    void UpdatePlayerShots(Players::Player* player){
        for(int i = 0; i < Players::max_player_shots; i++){
            Shots::UpdateShot(((player->ship.shots)+i));
        }
    }

    void UpdateShipFwd(Ship* ship){
        float radianBase = JMATH::DegreesToRadians(360.0f/ship->figure.t_vertices);
        float radianRotation = JMATH::DegreesToRadians(ship->figure.transform.rotation);
        ship->fwd = {cosf(radianRotation), sinf(radianRotation)};
    }

    bool IsPlayerDead(Players::Player player){
        return player.dead_ltc < player.dead_lt;
    }

    bool IsPlayerInmune(Players::Player player){
        return player.inmunity_ltc < player.inmunity_lt;
    }

    void UpdatePlayer(Players::Player* player, bool is_p1){
        if(player->is_active){
            if(IsPlayerInmune(*player)){
                player->inmunity_ltc += 1000/Utils::kFPS;
            }
            UpdateShipFwd(&(player->ship));

            PlayerInput(player, is_p1);
            DecelerateShip(&(player->ship));
            // printf("SPEED V LENGTH = %.2f\n", JMATH::Vec3Length(player->ship.speed_v));
            // JMATH::Vec3Print(player->ship.speed_v);
            PolyLibJMATH::MovePoly(&(player->ship.figure), player->ship.speed_v);
            Collisions::BorderExitRellocation(&(player->ship.figure));
            PolyLibJMATH::UpdatePoly(&(player->ship.figure));
        }else{
            if(player->lifes > 0){
                player->dead_ltc += 1000/Utils::kFPS;
                player->is_active = !IsPlayerDead(*player);
            }
        }

        UpdatePlayerShots(player);
    }
    

    void DrawPlayerShots(Players::Player player){
        for(int i = 0; i < Players::max_player_shots; i++){
            Shots::DrawShot(((player.ship.shots)+i));
        }
    }
    
    void DrawPlayerPropeller(Player player){
        JMATH::Vec2 line1_P1, line2_P1, lines_P2;
        JMATH::Vec2 aux_v;
        if(player.is_moving && ((int)esat::Time()%150) < 75){
            line1_P1 = *(player.ship.figure.draw_coords+2);
            line2_P1 = *(player.ship.figure.draw_coords+3);
            aux_v = JMATH::Vec2Scale(JMATH::Vec2Sub(line2_P1,line1_P1),0.5f);
            lines_P2 = JMATH::Vec2Sum(line1_P1,aux_v);
            aux_v = JMATH::Vec2Scale(JMATH::Vec2Sub(lines_P2,*(player.ship.figure.draw_coords+0)),0.5f);
            lines_P2 = JMATH::Vec2Sum(lines_P2,aux_v);

            esat::DrawLine(
                line1_P1.x, line1_P1.y, lines_P2.x, lines_P2.y
            );
            esat::DrawLine(
                line2_P1.x, line2_P1.y, lines_P2.x, lines_P2.y
            );
        }
    }

    void DrawPlayer(Player player){
        if(player.is_active){
            if(!IsPlayerInmune(player) || (player.inmunity_ltc % 500) < 250){
                PolyLibJMATH::DrawPoly(player.ship.figure,false);
                DrawPlayerPropeller(player);
            }
        }
        DrawPlayerShots(player);
    }


    void EmptyPlayerMemory(Player* player){
        for(int i = 0; i < max_player_shots; i++){
            PolyLibJMATH::EmptyPolyMemory(&((player->ship.shots+i)->bullet));
        }
        PolyLibJMATH::EmptyPolyMemory(&(player->ship.figure));
    }
    
}
