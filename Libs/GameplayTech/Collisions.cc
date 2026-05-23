#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include <esat/input.h>

#include "./GameplayTech.h"

#include "../CustomLibs/PolyLibJMATH.h"
#include "../CustomLibs/JMATH.h"
#include "../CustomLibs/Utils.h"
#include "../CustomLibs/AudioLib.h"

#include "../../Code/GameManager.h"
#include "../../Code/Gameplay.h"

namespace Collisions{
    bool show_colliders = false;

    //Checks if the given poly, gets past a window border completely and returns
    //which border it traspassed form. If it does not traspass from anywher, it returns NONE 
    Border CollisionPolyWindowBorderExit(PolyLibJMATH::Poly poly){
        Border collided = Border::NONE;
        float aux_position_x = 0;
        float aux_position_nx = 0;
        float aux_position_y = 0;
        float aux_position_ny = 0;

        aux_position_x = poly.transform.translation.x - poly.transform.scale.x;
        aux_position_nx = poly.transform.translation.x + poly.transform.scale.x;
        aux_position_y = poly.transform.translation.y - poly.transform.scale.y;
        aux_position_ny = poly.transform.translation.y + poly.transform.scale.y;
        if(aux_position_x > Utils::kWindowWidth){
            collided = Border::RIGHT;
        }else{
            if(aux_position_nx < 0){
                collided = Border::LEFT;
            }
        }

        if(aux_position_y > Utils::kWindowHeight){
                collided = Border::BOTTOM;
        }else{
            if(aux_position_ny < 0){
                collided = Border::TOP;
            }
        }
        return collided;
    }

    //Rellocates by teleportation the given poly to the oposite border it traspassed
    void BorderExitRellocation(PolyLibJMATH::Poly *poly){
        Border collided_border = Collisions::CollisionPolyWindowBorderExit(*poly);
        if(collided_border != Border::NONE){
            switch (collided_border){
                case Collisions::Border::TOP:
                    poly->transform.translation.y = Utils::kWindowHeight+(poly->transform.scale.y);
                break;
                case Collisions::Border::RIGHT:
                    poly->transform.translation.x = 0-(poly->transform.scale.x);
                break;
                case Collisions::Border::BOTTOM:
                    poly->transform.translation.y = 0-(poly->transform.scale.y);
                break;
                case Collisions::Border::LEFT:
                    poly->transform.translation.x = Utils::kWindowWidth+(poly->transform.scale.x);
                break;
            }

            //Saves coords before usual Update to prevent collision bug when rellocating
            PolyLibJMATH::SaveDrawCoords(poly);
        }
        
    }

    //Checks if there's an accurate collision between polys and returns the result
    bool CollisionPolyPoly(PolyLibJMATH::Poly p1, PolyLibJMATH::Poly p2){
        bool is_colliding = false;
        int p1_i, p2_i;
        JMATH::Vec2 interpolation_res;
            for (p1_i = 0; p1_i < p1.t_vertices && !is_colliding; p1_i++){
                for (p2_i = 0; p2_i < p2.t_vertices && !is_colliding; p2_i++){
                    interpolation_res = JMATH::CalcInterpolation(
                        *(p1.draw_coords+p1_i), JMATH::Vec2Sub(
                            *(p1.draw_coords+(p1_i+1 == p1.t_vertices ? 0 : p1_i+1)),
                            *(p1.draw_coords+p1_i)
                        ),
                        *(p2.draw_coords+p2_i), JMATH::Vec2Sub(
                            *(p2.draw_coords+(p2_i+1 == p2.t_vertices ? 0 : p2_i+1)),
                            *(p2.draw_coords+p2_i)
                        )
                    );

                    // printf("p1_i %d | p2_i %d\n",p1_i,p2_i);
                    // JMATH::Vec2Print(interpolation_res);
                    // printf("\n");

                    is_colliding = (
                        interpolation_res.x <= 1 && 
                        interpolation_res.x >= 0 &&
                        interpolation_res.y <= 1 && 
                        interpolation_res.y >= 0
                    );

                    //In case there's no actual collision, checks if is a "ghost collision"
                    //by looking for interpolation on the space between actual and previous position
                    //of p1 against p2.
                    if(!is_colliding && p1.prev_draw_coords != nullptr){
                        // printf("PREV COL DETECTOR\n");
                        if(show_colliders){
                            esat::DrawSetStrokeColor(0,255,0);
                            esat::DrawLine((p1.draw_coords+p1_i)->x, (p1.draw_coords+p1_i)->y, (p1.prev_draw_coords+p1_i)->x, (p1.prev_draw_coords+p1_i)->y);
                            esat::DrawLine((p2.draw_coords+p2_i)->x, (p2.draw_coords+p2_i)->y, (p2.draw_coords+p2_i)->x+1, (p2.draw_coords+p2_i)->y+1);
                        }
                        interpolation_res = JMATH::CalcInterpolation(
                            *(p1.draw_coords+p1_i), JMATH::Vec2Sub(
                                *(p1.prev_draw_coords+p1_i),
                                *(p1.draw_coords+p1_i)
                            ),
                            *(p2.draw_coords+p2_i), JMATH::Vec2Sub(
                                *(p2.draw_coords+(p2_i+1 == p2.t_vertices ? 0 : p2_i+1)),
                                *(p2.draw_coords+p2_i)
                            )
                        );

                        // printf("p1_i %d | p2_i %d\n",p1_i,p2_i);
                        // JMATH::Vec2Print(interpolation_res);
                        // printf("\n");

                        is_colliding = (
                            interpolation_res.x <= 1 && 
                            interpolation_res.x >= 0 &&
                            interpolation_res.y <= 1 && 
                            interpolation_res.y >= 0
                        );
                    }
                }
            }

        return is_colliding;
    }

    //Checks if there's collision between the given asteroid and any of the player shots and returns the result.
    //This function also manages the collision event.
    bool CollisionAsteroidPlayerShots(TList::ListNode** asteroid_list, TList::ListNode** particle_list, Asteroids::Asteroid *asteroid, Players::Player *player){
        bool collision = false;
        // printf("CollisionAsteroidPlayerShots\n");
        for (int i = 0; i < Players::max_player_shots && !collision; i++){
            if((player->ship.shots+i)->is_active && CollisionPolyPoly((player->ship.shots+i)->bullet, asteroid->figure)){
                // printf("ACTIVE AND COLLIDED\n");
                // printf("LIST %p\n",*asteroid_list);
                // printf("Asteroid %d\n",asteroid->id);
                // printf("Bullet %d\n\n",i);
                Shots::DestroyShot((player->ship.shots+i));

                Asteroids::DestroyAsteroid((void**)asteroid_list, (void**)particle_list, asteroid, player);
                collision = true;
            }
        }
        
        return collision;
    }

    //Checks if there's collision between the given asteroid and the given player ship and returns the result.
    //This function also manages the collision event.
    bool CollisionAsteroidPlayer(TList::ListNode** asteroid_list, TList::ListNode** particle_list, Asteroids::Asteroid *asteroid, Players::Player *player){
        bool collision = false;
        // printf("CollisionAsteroidPlayer\n");
        if(!Players::IsPlayerDead(*player) && !Players::IsPlayerImmune(*player) && CollisionPolyPoly(player->ship.figure, asteroid->figure)){
            Asteroids::DestroyAsteroid((void**)asteroid_list, (void**)particle_list, asteroid, player);
            Players::KillPlayer(player);
            collision = true;
        }
        
        return collision;
    }

    //Checks if there's collision between the given asteroid and the given ufo shot and returns the result.
    //This function also manages the collision event.
    bool CollisionAsteroidUfoShot(TList::ListNode** asteroid_list, TList::ListNode** particle_list, Asteroids::Asteroid *asteroid, Ufo::UfoShip *ufo){
        bool collision = false;
        // printf("CollisionAsteroidUfoShot\n");
        if(ufo->shot.is_active && CollisionPolyPoly(ufo->shot.bullet, asteroid->figure)){
            Asteroids::DestroyAsteroid((void**)asteroid_list, (void**)particle_list, asteroid, nullptr);
            Shots::DestroyShot(&(ufo->shot));
            
            collision = true;
        }
        
        return collision;
    }

    //Checks if there's collision between the given asteroid and the given ufo and returns the result.
    //This function also manages the collision event.
    bool CollisionAsteroidUfo(TList::ListNode** asteroid_list, TList::ListNode** particle_list, Asteroids::Asteroid *asteroid, Ufo::UfoShip *ufo){
        bool collision = false;
        // printf("CollisionAsteroidUfo\n");
        if(ufo->type != Ufo::UfoType::NONE && CollisionPolyPoly(ufo->figure, asteroid->figure)){
            Asteroids::DestroyAsteroid((void**)asteroid_list, (void**)particle_list, asteroid, nullptr);
            Ufo::DestroyUfo(ufo);
            collision = true;
        }
        
        return collision;
    }

    //Checks if there's collision between the given UFO and the given player ship and returns the result.
    //This function also manages the collision event.
    bool CollisionUfoPlayer(Ufo::UfoShip *ufo, Players::Player *player){
        bool collision = false;
        // printf("CollisionUfoPlayer\n");
        if(ufo->type != Ufo::UfoType::NONE && 
            !Players::IsPlayerDead(*player) && 
            !Players::IsPlayerImmune(*player) && 
            CollisionPolyPoly(ufo->figure, player->ship.figure)
        ){
            Players::KillPlayer(player);
            collision = true;
        }
        
        return collision;
    }

    //Checks if there's collision between the given UFO and any of the given player shots and returns the result.
    //This function also manages the collision event.
    bool CollisionUfoPlayerShots(Ufo::UfoShip *ufo, Players::Player *player){
        bool collision = false;
        // printf("CollisionUfoPlayerShots\n");
        if(ufo->type != Ufo::UfoType::NONE){
            for (int i = 0; i < Players::max_player_shots && !collision; i++){
                if((player->ship.shots+i)->is_active && CollisionPolyPoly((player->ship.shots+i)->bullet, ufo->figure)){
                    switch (ufo->type){
                        case Ufo::UfoType::BIG:
                            Players::AddPoints(player,200);
                        break;
                        case Ufo::UfoType::SMALL:
                            Players::AddPoints(player,1000);
                        break;
                    }

                    Ufo::DestroyUfo(ufo);
                    Shots::DestroyShot((player->ship.shots+i));

                    collision = true;
                }
            }
        }
        
        return collision;
    }

    //Checks if there's collision between the given UFO shot and the given player ship and returns the result.
    //This function also manages the collision event.
    bool CollisionUfoShotPlayer(Ufo::UfoShip *ufo, Players::Player *player){
        bool collision = false;
        // printf("CollisionUfoShotPlayer\n");
        if(ufo->shot.is_active && 
            !Players::IsPlayerDead(*player) && 
            !Players::IsPlayerImmune(*player) && 
            CollisionPolyPoly(ufo->shot.bullet, player->ship.figure)
        ){
            Shots::DestroyShot(&(ufo->shot));

            Players::KillPlayer(player);
            collision = true;
        }
        
        return collision;
    }

    //Checks if there's collision between the given PowerUp and the given player ship and returns the result.
    //This function also manages the collision event.
    bool CollisionPowerUpPlayer(PowerUps::PowerUp *pu, Players::Player *player){
        TList::ListInfo aux_info = {NULL};
        bool collision = false;
        // printf("CollisionPowerUpPlayer\n");
        if(PowerUps::IsPowerUpActive(*pu) && 
            !Players::IsPlayerDead(*player) && 
            !Players::IsPlayerImmune(*player) && 
            CollisionPolyPoly(pu->base_figure, player->ship.figure)
        ){
            switch (pu->type){
                case PowerUps::PU_Type::SHOTGUN:
                    // printf("SHOTGUN PICKED UP\n");
                    (player->pu_tags + PowerUps::PU_Type::SHOTGUN)->duration_ltc = 0;
                break;
                case PowerUps::PU_Type::FRIENDLY_FIRE:
                    // printf("FRIENDLY FIRE PICKED UP\n");
                    (player->pu_tags + PowerUps::PU_Type::FRIENDLY_FIRE)->duration_ltc = 0;
                break;
            }

            aux_info.powerUp_info = *pu;
            TList::DeleteElement(&Gameplay::spawned_power_ups, aux_info);
            collision = true;
            AudioLib::PlaySound(AudioLib::ACTION);
        }
        
        return collision;
    }

    //Checks if there's collision between any of the first parameter player shots and the second parameter player ship and returns the result.
    //Mainly used when the shooting player has the FriendlyFire power up active.
    //This function also manages the collision event.
    bool CollisionPlayerShotsPlayer(Players::Player *player_shooting, Players::Player *other_player){
        bool collision = false;
        // printf("CollisionPlayerShotsPlayer\n");
        if(!Players::IsPlayerDead(*other_player) && !IsPlayerImmune(*other_player)){
            for (int i = 0; i < Players::max_player_shots && !collision; i++){
                if((player_shooting->ship.shots+i)->is_active && CollisionPolyPoly((player_shooting->ship.shots+i)->bullet, other_player->ship.figure)){
                    RemovePoints(other_player, 1000);
                    other_player->lifes++;
                    Players::KillPlayer(other_player);
                    Shots::DestroyShot((player_shooting->ship.shots+i));

                    collision = true;
                }
            }
        }
        
        return collision;
    }

}
