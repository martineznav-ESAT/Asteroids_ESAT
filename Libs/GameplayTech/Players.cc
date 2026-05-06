#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Code/UserManager.h"
#include "../../Code/GameManager.h"
#include "../../Code/MainMenu.h"
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
        new_ship.death_particles = (Particles::Particle*) malloc(sizeof(Particles::Particle)*4);
        for(int i = 0; i < 4; i++){
            *(new_ship.death_particles+i) = Particles::NewParticle(Particles::PLAYER_DEATH);
            PolyLibJMATH::UpdatePoly(&((new_ship.death_particles+i)->figure));
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
        new_player.life_up_score = 0;
        new_player.dead_lt = 3000;
        new_player.dead_ltc = 3000;
        new_player.inmunity_lt = 2000;
        new_player.inmunity_ltc = 2000;
        new_player.round = 0;
        new_player.consecutive_hs = 0;
        new_player.pu_tags = nullptr;

        return new_player;
    }

    bool IsPlayerDead(Players::Player player){
        return player.dead_ltc < player.dead_lt;
    }

    bool IsPlayerImmune(Players::Player player){
        return player.inmunity_ltc < player.inmunity_lt;
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

    void AddPoints(Player* player, int points){
        player->life_up_score += points;
        // printf("LIFE UP SCORE %d\n", player->life_up_score/10000);

        player->lifes += player->life_up_score/10000;
        player->life_up_score %= 10000;

        player->score += points;
        if(player->score > 999999){
            player->score = 999999;
        }
        
    }

    void RemovePoints(Player* player, int points){
        player->score -= points;
        if(player->score < 0){
            player->score = 0;
        }
    }

    void ResetPowerUpTagsTime(PowerUps::PU_Type total, PowerUps::PowerUpTag *tags){
        // printf("ResetPowerUpTagsTime\n");
        for(int i = 0; i < total; i++){
            *(tags+i) = PowerUps::NewPowerUpTag((PowerUps::PU_Type)i); 
            // printf("TAG -> %d duration LTC %f\n",i,(tags+i)->duration_ltc);
        }
    }

    void ResetPlayerPowerUps(Player* player){
        if(player->pu_tags){
            switch (GameManager::game_status.actual_game->gamemode){
                case PlayedGames::Gamemode::SP:
                case PlayedGames::Gamemode::MP_ALT:
                    ResetPowerUpTagsTime(PowerUps::PU_Type::TOTAL_PU_SP_TYPES,player->pu_tags);
                break;
            
                case PlayedGames::Gamemode::MP_VS:
                case PlayedGames::Gamemode::MP_COOP:
                    ResetPowerUpTagsTime(PowerUps::PU_Type::TOTAL_PU_MP_TYPES,player->pu_tags);
                break;
            }
        }else{
            switch (GameManager::game_status.actual_game->gamemode){
                case PlayedGames::Gamemode::SP:
                case PlayedGames::Gamemode::MP_ALT:
                    if(PowerUps::PU_Type::TOTAL_PU_SP_TYPES > 0){
                        player->pu_tags = (PowerUps::PowerUpTag*) malloc(sizeof(PowerUps::PowerUpTag) * (int)PowerUps::PU_Type::TOTAL_PU_SP_TYPES);
                        ResetPowerUpTagsTime(PowerUps::PU_Type::TOTAL_PU_SP_TYPES,player->pu_tags);
                    }
                break;
            
                case PlayedGames::Gamemode::MP_VS:
                case PlayedGames::Gamemode::MP_COOP:
                    if(PowerUps::PU_Type::TOTAL_PU_MP_TYPES > 0){
                        player->pu_tags = (PowerUps::PowerUpTag*) malloc(sizeof(PowerUps::PowerUpTag) * (int)PowerUps::PU_Type::TOTAL_PU_MP_TYPES);
                        ResetPowerUpTagsTime(PowerUps::PU_Type::TOTAL_PU_MP_TYPES,player->pu_tags);
                    }
                break;
            }
        }
    }

    void RespawnPlayer(Player* player){
        player->inmunity_ltc = 0;

        player->ship.figure.transform.rotation = -90.0f;

        switch (GameManager::game_status.actual_game->gamemode){
            case PlayedGames::Gamemode::SP:
            case PlayedGames::Gamemode::MP_ALT:
                player->ship.figure.transform.translation = {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f};
            break;
        
            case PlayedGames::Gamemode::MP_VS:
            case PlayedGames::Gamemode::MP_COOP:
                if(GameManager::IsPlayer1(player)){
                    player->ship.figure.transform.translation = {Utils::kWindowWidth*0.33f, Utils::kWindowHeight*0.5f};
                }else{
                    player->ship.figure.transform.translation = {Utils::kWindowWidth*0.66f, Utils::kWindowHeight*0.5f};
                }
            break;
        }

        player->ship.fwd = {0.0f,0.0f};
        player->ship.speed_v = {0.0f,0.0f};
        player->ship.max_speed = 7.0f;
        player->ship.accel = 15.0f;
        player->ship.decel = 0.99f;

        ResetPlayerPowerUps(player);

        PolyLibJMATH::UpdatePoly(&(player->ship.figure));
    }

    void KillPlayer(Player* player){
        for(int i = 0; i < 4; i++){
            Particles::LoadParticle(
                (player->ship.death_particles+i),
                player->ship.figure.transform.translation
            );
        }
        player->is_active = false;
        player->dead_ltc = 0;
        player->lifes--;
        player->consecutive_hs = 0;
        RespawnPlayer(player);
    }

    void HyperSpacePlayer(Player* player){
        player->ship.figure.transform.translation = {
            Utils::GenerateRandomNumber(Utils::kWindowWidth-(player->ship.figure.transform.scale.x * 2))+player->ship.figure.transform.scale.x,
            Utils::GenerateRandomNumber(Utils::kWindowHeight-(player->ship.figure.transform.scale.y * 2))+player->ship.figure.transform.scale.y 
        };

        //If the Hyperspace is used while immune, adds 1 to the consecutive Hyperspaces debuff
        if(IsPlayerImmune(*player)){
            player->consecutive_hs++;
        }

        //While the debuff is less or equal than 3. Grants inmunity on Hyperspace 
        if(player->consecutive_hs <= 3){
            player->inmunity_ltc = 0;
        }else{
            player->inmunity_ltc = player->inmunity_lt;
        }
    }

    int GetPlayerActiveShots(Player *p){
        int count = 0;
        for(int i = 0; i < max_player_shots; i++){
            if((p->ship.shots+i)->is_active){
                count++;
            }
        }
        // printf("ACTIVE SHOTS %d\n", count);
        return count;
    }

    void PlayerInput(Player* p){
        if(GameManager::IsPlayer1(p)){
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

            if(!IsPlayerImmune(*p) ){
                if(esat::IsSpecialKeyPressed(esat::SpecialKey::kSpecialKey_Space) && GetPlayerActiveShots(p) < 3){
                    // printf("IsSpecialKeyPressed\n");
                    if(((int)esat::Time()%100) <= 25){
                        // printf("esat::Time\n");
                        ShipShoot(&(p->ship));
                        // printf("ShipShoot\n");
                    }
                }else{
                    if(esat::IsSpecialKeyDown(esat::SpecialKey::kSpecialKey_Space) && GetPlayerActiveShots(p) >= 3){
                        // printf("IsSpecialKeyDown\n");
                        ShipShoot(&(p->ship));
                        // printf("ShipShoot\n");
                    }
                }
            }

            if(esat::IsKeyDown('G')){
                HyperSpacePlayer(p);
            }


            //DEBUG INPUT
            if(esat::IsKeyDown('Q')){
                KillPlayer(p);
            }

            if(esat::IsKeyDown('Z')){
                p->lifes++;
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

            if(!IsPlayerImmune(*p) ){
                if(esat::IsSpecialKeyPressed(esat::SpecialKey::kSpecialKey_Enter) && GetPlayerActiveShots(p) < 3){
                    // printf("IsSpecialKeyPressed\n");
                    if(((int)esat::Time()%100) <= 25){
                        // printf("esat::Time\n");
                        ShipShoot(&(p->ship));
                        // printf("ShipShoot\n");
                    }
                }

                if(esat::IsSpecialKeyDown(esat::SpecialKey::kSpecialKey_Enter) && GetPlayerActiveShots(p) >= 3){
                    // printf("IsSpecialKeyDown\n");
                    ShipShoot(&(p->ship));
                    // printf("ShipShoot\n");
                }
            }

            if(esat::IsKeyDown('P')){
                HyperSpacePlayer(p);
            }

            //DEBUG INPUT
            if(esat::IsSpecialKeyDown(esat::SpecialKey::kSpecialKey_Backspace)){
                KillPlayer(p);
            }

            if(esat::IsKeyDown('L')){
                p->lifes++;
            }
        }
    }

    void UpdatePlayerShots(Players::Player* player){
        for(int i = 0; i < Players::max_player_shots; i++){
            Shots::UpdateShot(((player->ship.shots)+i));
        }
    }

    void UpdatePlayerParticles(Players::Player* player){
        for(int i = 0; i < 4; i++){
            Particles::UpdateParticle((player->ship.death_particles+i));
        }
    }

    void UpdateShipFwd(Ship* ship){
        float radianBase = JMATH::DegreesToRadians(360.0f/ship->figure.t_vertices);
        float radianRotation = JMATH::DegreesToRadians(ship->figure.transform.rotation);
        ship->fwd = {cosf(radianRotation), sinf(radianRotation)};
    }

    void UpdatePowerUpTags(PowerUps::PU_Type total, PowerUps::PowerUpTag *tags){
        for(int i = 0; i < total; i++){
            PowerUps::UpdatePowerUpTag(tags+i);
        }
    }

    void UpdatePlayerPowerUps(Player* player){
        if(player->pu_tags){
            // printf("UpdatePlayerPowerUps Player %d\n", player->score);
            switch (GameManager::game_status.actual_game->gamemode){
                case PlayedGames::Gamemode::SP:
                case PlayedGames::Gamemode::MP_ALT:
                    UpdatePowerUpTags(PowerUps::PU_Type::TOTAL_PU_SP_TYPES,player->pu_tags);
                break;
            
                case PlayedGames::Gamemode::MP_VS:
                case PlayedGames::Gamemode::MP_COOP:
                    UpdatePowerUpTags(PowerUps::PU_Type::TOTAL_PU_MP_TYPES,player->pu_tags);
                break;
            }
            // printf("END POWERUP UPDATE %d\n\n\n", player->score);
        }
    }
    

    void PlayerCollisions(Player* player){
        if(GameManager::game_status.level == GameManager::Level::GAMEPLAY){

            //Ufo/Player Collisions in diferent gamemodes
            //Ufo/Asteroids collisions is managed by Gameplay method "UpdateGameAsteroids()"
            switch (GameManager::game_status.actual_game->gamemode){
                case PlayedGames::Gamemode::MP_COOP:
                case PlayedGames::Gamemode::MP_VS:
                    //FRIENDLY_FIRE COLLISIONS IF POWER UP ACTIVE
                    if(PowerUps::IsPowerUpActive(*(player->pu_tags+PowerUps::PU_Type::FRIENDLY_FIRE))){
                        // printf("ACTUALIZANDO COLISIONES DISPAROS POWERUP FRIENDLY_FIRE\n");
                        Collisions::PlayerShotsPlayerCollision(player, GameManager::GetOtherPlayer(player));
                    }
                break;
            }
        }
    }

    void UpdatePlayer(Player* player){
        // SAVE AND RETURN TO MAIN MENU INPUT
        if(esat::IsKeyDown('M')){
            time(&(GameManager::game_status.actual_game->save_time));
            MainMenu::Load();
        }

        if(player->is_active){
            if(IsPlayerImmune(*player)){
                player->inmunity_ltc += 1000/Utils::kFPS;
            }

            UpdatePlayerPowerUps(player);
            PlayerCollisions(player);

            UpdateShipFwd(&(player->ship));

            PlayerInput(player);
            DecelerateShip(&(player->ship));
            // printf("SPEED V LENGTH = %.2f\n", JMATH::Vec3Length(player->ship.speed_v));
            // JMATH::Vec3Print(player->ship.speed_v);
            PolyLibJMATH::MovePoly(&(player->ship.figure), player->ship.speed_v);
            Collisions::BorderExitRellocation(&(player->ship.figure));
            PolyLibJMATH::UpdatePoly(&(player->ship.figure));

        }else{
            
            switch (GameManager::game_status.actual_game->gamemode){
                case PlayedGames::Gamemode::MP_ALT:
                    if(player->lifes >= 0){
                        player->dead_ltc += 1000/Utils::kFPS;

                        if(!IsPlayerDead(*player)){
                            if(
                                GameManager::IsPlayer1(player) ? 
                                    GameManager::game_status.actual_game->p2.lifes > 0 : 
                                    GameManager::game_status.actual_game->p1.lifes > 0
                            ){
                                GameManager::AlternateActivePlayer();
                            }else{
                                if(player->lifes > 0){
                                    player->is_active = true;
                                }
                            }
                        }
                    }
                break;

                default:
                    if(player->lifes > 0){
                        player->dead_ltc += 1000/Utils::kFPS;
                        player->is_active = !IsPlayerDead(*player);
                    }
                break;
            }
        }

        UpdatePlayerShots(player);
        UpdatePlayerParticles(player);
    }
    

    void DrawPlayerShots(Players::Player player){
        for(int i = 0; i < Players::max_player_shots; i++){
            Shots::DrawShot(((player.ship.shots)+i));
        }
    }

    void DrawPlayerParticles(Players::Player player){
        for(int i = 0; i < 4; i++){
            Particles::DrawParticle((player.ship.death_particles+i));
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

    bool IsDrawPlayerSolid(Player player){
        bool is_draw_solid = false;
        switch (GameManager::game_status.actual_game->gamemode){
            case PlayedGames::Gamemode::SP:
            case PlayedGames::Gamemode::MP_ALT:
                //Current mechanics do not affect solid draw on SinglePlayer-like gameplay mechanics
                //MP_ALT is essentialy 2 different singleplayer games played with alternation on dead
                //which means the mechanics are the same as single player

                //Switch structure remains in case of future updates

            break;
            case PlayedGames::Gamemode::MP_COOP:
            case PlayedGames::Gamemode::MP_VS:
                is_draw_solid = PowerUps::IsPowerUpActive(*(player.pu_tags+PowerUps::PU_Type::FRIENDLY_FIRE));
            break;
        }
        return is_draw_solid;
    }

    void DrawPlayer(Player player){
        // printf("DrawPlayer INIT\n");
        if(player.is_active){
            // printf("player.is_active\n");
            if(!IsPlayerImmune(player) || (player.inmunity_ltc % 500) < 250){
                // printf("!IsPlayerImmune\n");
                PolyLibJMATH::DrawPoly(
                    player.ship.figure,
                    IsDrawPlayerSolid(player)
                );
                // printf("DrawPoly END\n");
                DrawPlayerPropeller(player);
                // printf("DrawPlayerPropeller END\n");
            }
        }
        DrawPlayerShots(player);
        // printf("DrawPlayerShots END\n");
        DrawPlayerParticles(player);
        // printf("DrawPlayerParticles END\n");
        // printf("DrawPlayer END\n");
    }


    void EmptyPlayerMemory(Player* player){
        if(player->ship.shots){
            for(int i = 0; i < max_player_shots; i++){
                PolyLibJMATH::EmptyPolyMemory(&((player->ship.shots+i)->bullet));
            }
        }
        
        
        if(player->ship.death_particles){
            for(int i = 0; i < 4; i++){
                Particles::EmptyParticleMemory((player->ship.death_particles+i));
            }
        }

        free(player->pu_tags);
        PolyLibJMATH::EmptyPolyMemory(&(player->ship.figure));
    }
    
}
