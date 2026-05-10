#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "./PowerUps.h"

#include "../CustomLibs/PolyLibJMATH.h"
#include "../CustomLibs/Utils.h"

#include "../../Code/GameManager.h"
#include "../../Code/Gameplay.h"

namespace PowerUps{
    JMATH::Vec3 **friendly_fire_icon_coords = nullptr;
    int last_pu_id = -1;

    void InitFriendlyFireIcon(){
        friendly_fire_icon_coords = (JMATH::Vec3**) malloc(sizeof(JMATH::Vec3*) * ((int)FFIcons::FF_TOTAL_ICONS));

        // Ship Bottom Half figure
        *(friendly_fire_icon_coords+FFIcons::FF_ICON_1) = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3)*FFIconsVertices::V_ICON_1);
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_1)+0) =    {-0.6f, -0.3f, 1.0f};     // 1
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_1)+1) =    {-1.0f, -0.6f, 1.0f};     // 2
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_1)+2) =    {0.0f, -0.3f, 1.0f};      // 3
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_1)+3) =    {-0.2f, -0.2f, 1.0f};     // 4
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_1)+4) =    {0.2f, 0.0f, 1.0f};       // 5
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_1)+5) =    {-0.2f, 0.2f, 1.0f};      // 6
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_1)+6) =    {0.0f, 0.3f, 1.0f};       // 7
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_1)+7) =    {-1.0f, 0.6f, 1.0f};      // 8
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_1)+8) =    {-0.6f, 0.3f, 1.0f};      // 9

        
        // Ship Top Half figure
        *(friendly_fire_icon_coords+FFIcons::FF_ICON_2) = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3)*FFIconsVertices::V_ICON_2);
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_2)+0) =    {0.2f, 0.0f, 1.0f};       // 1
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_2)+1) =    {-0.2f, -0.2f, 1.0f};     // 2
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_2)+2) =    {0.0f, -0.3f, 1.0f};      // 3
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_2)+3) =    {1.0f, 0.0f, 1.0f};       // 4
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_2)+4) =    {0.0f, 0.3f, 1.0f};       // 5
        *(*(friendly_fire_icon_coords+FFIcons::FF_ICON_2)+5) =    {-0.2f, 0.2f, 1.0f};      // 6

    }

    void Init(){
        InitFriendlyFireIcon();
    }

    void BuildShotgunIcons(PowerUp *p_up){
        JMATH::Mat3 tr_aux;
        p_up->icon_figures = (PolyLibJMATH::Poly*) malloc(sizeof(PolyLibJMATH::Poly)*ShotgunIcons::SG_TOTAL_ICONS);

        for(int i = 0; i < ShotgunIcons::SG_TOTAL_ICONS; i++){
            PolyLibJMATH::InitPoly(
                (p_up->icon_figures+i),
                2,
                p_up->base_figure.transform.scale,
                p_up->base_figure.transform.rotation+(i*10),
                p_up->base_figure.transform.translation,
                p_up->base_figure.color,
                {0,0}
            );
        }
    }

    
    void BuildFriendlyFireIcons(PowerUp *p_up){
        JMATH::Mat3 tr_aux;
        p_up->icon_figures = (PolyLibJMATH::Poly*) malloc(sizeof(PolyLibJMATH::Poly)*FFIcons::FF_TOTAL_ICONS);

        PolyLibJMATH::InitPoly(
            (p_up->icon_figures+FFIcons::FF_ICON_1),
            FFIconsVertices::V_ICON_1,
            *(friendly_fire_icon_coords+((int)FFIcons::FF_ICON_1)),
            p_up->base_figure.transform.scale,
            p_up->base_figure.transform.rotation,
            p_up->base_figure.transform.translation,
            p_up->base_figure.color,
            {-0.1f,0.1f}
        );

        

        PolyLibJMATH::InitPoly(
            (p_up->icon_figures+FFIcons::FF_ICON_2),
            FFIconsVertices::V_ICON_2,
            *(friendly_fire_icon_coords+((int)FFIcons::FF_ICON_2)),
            p_up->base_figure.transform.scale,
            p_up->base_figure.transform.rotation,
            p_up->base_figure.transform.translation,
            p_up->base_figure.color,
            {0.5f,0.3f}
        );

        //Rotates base local_coords permanently to draw the Bottom Half figure icon properly
        tr_aux = JMATH::Mat3Identity();
        tr_aux = JMATH::Mat3MultMat3(JMATH::Mat3Rotate(JMATH::DegreesToRadians(10)), tr_aux);
        for(int i = 0; i < (p_up->icon_figures+(int)FFIcons::FF_ICON_1)->t_vertices; i++){
            *(((p_up->icon_figures+(int)FFIcons::FF_ICON_1)->local_coords)+i) = JMATH::Mat3MultVec3(tr_aux, {(((p_up->icon_figures+(int)FFIcons::FF_ICON_1)->local_coords)+i)->x, (((p_up->icon_figures+(int)FFIcons::FF_ICON_1)->local_coords)+i)->y, 1.0f});
            // printf("Coord Update local_coords ICON %d: %f - %f\n",i, (*((p_up->icon_figures+(int)FFIcons::ICON_1)->local_coords+i)).x, (*((p_up->icon_figures+(int)FFIcons::ICON_1)->local_coords+i)).y);
        }

        //Rotates base local_coords permanently to draw the Top Half figure icon properly
        tr_aux = JMATH::Mat3Identity();
        tr_aux = JMATH::Mat3MultMat3(JMATH::Mat3Rotate(JMATH::DegreesToRadians(-10)), tr_aux);
        for(int i = 0; i < (p_up->icon_figures+(int)FFIcons::FF_ICON_2)->t_vertices; i++){
            *(((p_up->icon_figures+(int)FFIcons::FF_ICON_2)->local_coords)+i) = JMATH::Mat3MultVec3(tr_aux, {(((p_up->icon_figures+(int)FFIcons::FF_ICON_2)->local_coords)+i)->x, (((p_up->icon_figures+(int)FFIcons::FF_ICON_2)->local_coords)+i)->y, 1.0f});
            // printf("Coord Update local_coords ICON %d: %f - %f\n",i, (*((p_up->icon_figures+(int)FFIcons::ICON_2)->local_coords+i)).x, (*((p_up->icon_figures+(int)FFIcons::ICON_2)->local_coords+i)).y);
        }
    }

    void BuildIcons(PowerUp *p_up){
        switch(p_up->type){
            case SHOTGUN:
                BuildShotgunIcons(p_up);
            break;
            case FRIENDLY_FIRE:
                BuildFriendlyFireIcons(p_up);
            break;

            default:
                p_up->icon_figures = nullptr;
            break;
        }
    }

    void UpdateIcon(PowerUp *p_up){
        PolyLibJMATH::UpdatePoly(&(p_up->base_figure));
        switch (p_up->type){
            case SHOTGUN:
                for (int i = 0; i < ShotgunIcons::SG_TOTAL_ICONS; i++){
                    (p_up->icon_figures+i)->transform.scale = JMATH::Vec2Scale(p_up->base_figure.transform.scale,0.5f);
                    (p_up->icon_figures+i)->transform.rotation = p_up->base_figure.transform.rotation;
                    (p_up->icon_figures+i)->transform.translation = p_up->base_figure.transform.translation;
                    PolyLibJMATH::UpdatePoly((p_up->icon_figures+i));
                }
            break;

            case PU_Type::FRIENDLY_FIRE:
                for (int i = 0; i < FFIcons::FF_TOTAL_ICONS; i++){
                    (p_up->icon_figures+i)->transform.scale = JMATH::Vec2Scale(p_up->base_figure.transform.scale,0.5f);
                    (p_up->icon_figures+i)->transform.rotation = p_up->base_figure.transform.rotation;
                    (p_up->icon_figures+i)->transform.translation = p_up->base_figure.transform.translation;
                    PolyLibJMATH::UpdatePoly((p_up->icon_figures+i));
                }
            break;
        }
    }

    PowerUp NewPowerUp(PU_Type type, JMATH::Vec2 position){
        PowerUp new_powerUp;
        new_powerUp.id = ++last_pu_id;
        new_powerUp.type = type;
        
        PolyLibJMATH::InitPoly(
            &new_powerUp.base_figure,
            4,
            {22.5f,22.5f},
            -135.0f,
            {position.x, position.y},
            {25,25,25},
            {0.0f,0.0f}
        );
        
        BuildIcons(&new_powerUp);

        new_powerUp.duration_lt = 8000;
        new_powerUp.duration_ltc = 0;

        UpdateIcon(&new_powerUp);

        // printf("NEW POWER UP CREATED %d\n",new_powerUp.id);
        return new_powerUp;
    }

    PowerUpTag NewPowerUpTag(PU_Type type){
        PowerUpTag new_powerUpTag;

        new_powerUpTag.type = type;

        switch(type){
            default:
                new_powerUpTag.duration_lt = 5000; //5s default
            break;
        }

        new_powerUpTag.duration_ltc = new_powerUpTag.duration_lt;

        return new_powerUpTag;
    }

    bool IsPowerUpActive(PowerUp powerUp){
        return powerUp.duration_ltc < powerUp.duration_lt;
    }
    
    bool IsPowerUpActive(PowerUpTag powerUpTag){
        return powerUpTag.duration_ltc < powerUpTag.duration_lt;
    }

    bool IsPowerUpBlinking(PowerUp powerUp){
        return (powerUp.duration_ltc >= powerUp.duration_lt-3000);
    }


    void GeneratePowerUp(void* asteroid){
        //At the moment, only when an asteroid is destroyed, power ups are generated.
        //But when incluiding Asteroids to the .h file there's an error, so is passed as void.
        //For the future, in case powerUps cane spawn with different conditions, void* could be useful
        //with a switch and an extra type parameter.
        
        Asteroids::Asteroid aux_asteroid = *((Asteroids::Asteroid*)asteroid);
        TList::ListInfo aux_info = {NULL};

        switch (GameManager::game_status.actual_game->gamemode){
            case PlayedGames::Gamemode::SP :
            case PlayedGames::Gamemode::MP_ALT:
                //Since MP_ALT gamemode is technically 2 separated single players alternating, 
                //the available power ups are essentially the same 

                //Currently no PowerUps 03/05/2026
                if(PowerUps::PU_Type::TOTAL_PU_SP_TYPES > 0){
                    aux_info.powerUp_info = PowerUps::NewPowerUp(
                        (PowerUps::PU_Type)Utils::GenerateRandomNumber(PowerUps::PU_Type::TOTAL_PU_SP_TYPES), 
                        aux_asteroid.figure.transform.translation
                    );
                    TList::InsertList(&Gameplay::spawned_power_ups, TList::ListType::POWER_UP, aux_info);
                }
            break;
        
            case PlayedGames::Gamemode::MP_VS :
            case PlayedGames::Gamemode::MP_COOP :
                if(PowerUps::PU_Type::TOTAL_PU_MP_TYPES > 0){
                    aux_info.powerUp_info = PowerUps::NewPowerUp(
                        (PowerUps::PU_Type)Utils::GenerateRandomNumber(PowerUps::PU_Type::TOTAL_PU_MP_TYPES),
                        aux_asteroid.figure.transform.translation
                    );
                    TList::InsertList(&Gameplay::spawned_power_ups, TList::ListType::POWER_UP, aux_info);
                }
            break;
        }
    }


    //Scales the given PowerUp for the amount of the frame equivalent calculated value of a percentage during an specified amount of miliseconds.
    //It scales up or down based on the is_growing parameter
    void ScalePowerUpFramePercentageEquivalent(PowerUp *powerUp, float percentage_decimals, int time_ms, bool is_gowing){
        if(is_gowing){
            powerUp->base_figure.transform.scale = JMATH::Vec2Scale(powerUp->base_figure.transform.scale, 1 + (1.0f/((time_ms/1000.0f)*Utils::kFPS))*percentage_decimals);
        }else{
            powerUp->base_figure.transform.scale = JMATH::Vec2Scale(powerUp->base_figure.transform.scale, 1 - (1.0f/((time_ms/1000.0f)*Utils::kFPS))*percentage_decimals);
        }
    }

    bool PowerUpCollisions(PowerUp *powerUp){
        bool is_collided = false;

        //PowerUp/Player Collisions in diferent gamemodes
        switch (GameManager::game_status.actual_game->gamemode){
            case PlayedGames::Gamemode::SP:
                is_collided = Collisions::CollisionPowerUpPlayer(powerUp, &(GameManager::game_status.actual_game->p1));
            break;

            case PlayedGames::Gamemode::MP_ALT:
                if(GameManager::game_status.actual_game->is_player1_turn){
                    is_collided = Collisions::CollisionPowerUpPlayer(powerUp, &(GameManager::game_status.actual_game->p1));
                }else{
                    is_collided = Collisions::CollisionPowerUpPlayer(powerUp, &(GameManager::game_status.actual_game->p2));
                }
            break;

            case PlayedGames::Gamemode::MP_COOP:
            case PlayedGames::Gamemode::MP_VS:
                is_collided = Collisions::CollisionPowerUpPlayer(powerUp, &(GameManager::game_status.actual_game->p1));
                if(!is_collided){
                    is_collided = Collisions::CollisionPowerUpPlayer(powerUp, &(GameManager::game_status.actual_game->p2));
                }
            break;
        }

        return is_collided;
    }

    void UpdatePowerUp(PowerUp *powerUp){
        if(GameManager::game_status.level == GameManager::Level::GAMEPLAY){
            if(IsPowerUpActive(*powerUp) && !PowerUpCollisions(powerUp)){
                //Grows for 0.5s and Shrinks for 0.5s
                if((((int)(powerUp->duration_ltc))%1000) < 500){
                    ScalePowerUpFramePercentageEquivalent(powerUp, 0.33f, 500, true);
                }else{
                    ScalePowerUpFramePercentageEquivalent(powerUp, 0.33f, 500, false);
                }

                UpdateIcon(powerUp);

                powerUp->duration_ltc += 1000/Utils::kFPS;
                // printf("pu_duration_ltc %f\n", powerUp->duration_ltc);
            }
        }
    }

    void UpdatePowerUpTag(PowerUpTag *powerUpTag){
        if(
            GameManager::game_status.level == GameManager::Level::GAMEPLAY &&
            IsPowerUpActive(*powerUpTag)
        ){
            powerUpTag->duration_ltc += 1000/Utils::kFPS;
            // printf("UpdatePowerUpTag Type %d Duration %f\n",powerUpTag->type, powerUpTag->duration_ltc);
        }
    }

    void DrawIcon(PowerUp powerUp){
        PolyLibJMATH::DrawPoly(powerUp.base_figure, true, {255,255,255});

        switch (powerUp.type){
            case PU_Type::SHOTGUN:
                for (int i = 0; i < ShotgunIcons::SG_TOTAL_ICONS; i++){
                    PolyLibJMATH::DrawPoly(*(powerUp.icon_figures+i), false, {255,255,255});
                }
            break;
            case PU_Type::FRIENDLY_FIRE:
                for (int i = 0; i < FFIcons::FF_TOTAL_ICONS; i++){
                    PolyLibJMATH::DrawPoly(*(powerUp.icon_figures+i), false, {255,255,255});
                }
            break;
        }
    }

    void DrawPowerUp(PowerUp powerUp){
        if(IsPowerUpActive(powerUp)){
            // printf("DRAWING POWER UP %d\n",powerUp.id);
            if(IsPowerUpBlinking(powerUp)){
                // printf("POWER UP %d BLINKING\n",powerUp.id);
                if((((int)powerUp.duration_ltc) % 300) < 150){
                    DrawIcon(powerUp);
                }
            }else{
                DrawIcon(powerUp);
            }
        }
    }

    void EmptyPowerUpMemory(PowerUp *powerUp){
        PolyLibJMATH::EmptyPolyMemory(&(powerUp->base_figure));
        switch(powerUp->type){
            case FRIENDLY_FIRE:
                for (int i = 0; i < FFIcons::FF_TOTAL_ICONS; i++){
                    PolyLibJMATH::EmptyPolyMemory((powerUp->icon_figures+i));
                }
                free(powerUp->icon_figures);
            break;
        }
    }

    void EmptyMemory(){
        //Memory release code structure with multiple Power Up Types in mind
        for (int type = 0; type < (int)PU_Type::TOTAL_PU_TYPES; type++){
            switch ((PU_Type)type){
                case PU_Type::FRIENDLY_FIRE:
                    for (int i = 0; i < FFIcons::FF_TOTAL_ICONS; i++){
                        free(*(friendly_fire_icon_coords+i));
                    }
                    free(friendly_fire_icon_coords);
                break;
            }
        }
    }
}
