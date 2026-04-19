#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include <esat/input.h>

#include "./GameplayTech.h"

#include "../CustomLibs/PolyLibJMATH.h"
#include "../CustomLibs/JMATH.h"
#include "../CustomLibs/Utils.h"

namespace Collisions{
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

    void BorderExitRellocation(PolyLibJMATH::Poly *poly){
        switch (Collisions::CollisionPolyWindowBorderExit(*poly)){
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
    }

    //TO_DO COLLISIONS
    // bool CollisionPolyPoint(PolyLibJMATH::Poly poly, JMATH::Vec2 point){
    //     return false;
    // }

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
                }
            }

        return is_colliding;
    }

    bool CollisionAsteroidPlayerShots(TList::ListNode** asteroid_list, Asteroids::Asteroid *asteroid, Players::Player *player){
        bool collision = false;
        // printf("CollisionAsteroidPlayerShots\n");
        for (int i = 0; i < Players::max_player_shots && !collision; i++){
            if((player->ship.shots+i)->is_active && CollisionPolyPoly((player->ship.shots+i)->bullet, asteroid->figure)){
                printf("ACTIVE AND COLLIDED\n");
                printf("LIST %p\n",*asteroid_list);
                printf("Asteroid %d\n",asteroid->id);
                printf("Bullet %d\n\n",i);
                Asteroids::DestroyAsteroid((void**)asteroid_list, asteroid, player);
                (player->ship.shots+i)->is_active = false;
            }
        }
        
        return collision;
    }

    //DEBUG COLLISIONS
    //CREATED FOR TESTING PURPOSES
    bool CollisionPolyOnRClick(PolyLibJMATH::Poly p1){
        return esat::MouseButtonDown(1);
    }
}
