#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "./Shots.h"

#include "../CustomLibs/PolyLibJMATH.h"

#include "../../Code/Gameplay.h"

namespace Shots{

    void Init(){

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
        new_shot.life_time = 1000; //ms
        new_shot.lt_count = 0; //ms timer
        new_shot.is_active = false;
        return new_shot;
    }

    void FireShot(Shot *shot, JMATH::Vec2 spawn_point, float rotation, JMATH::Vec3 direction_v, float speed){
        // printf("DISPARAR\n");
        shot->is_active = true;
        shot->lt_count = 0;
        shot->speed_v = JMATH::Vec3Scale(JMATH::Vec3Norm(direction_v), speed);
        shot->bullet.transform.translation = spawn_point;
        shot->bullet.transform.rotation = rotation+45;
        PolyLibJMATH::SaveDrawCoords(&(shot->bullet));
    }

    void UpdateShot(Shot *shot){
        if(shot->is_active){
            PolyLibJMATH::MovePoly(&(shot->bullet), (shot->speed_v));
            Collisions::BorderExitRellocation(&(shot->bullet));
            PolyLibJMATH::UpdatePoly(&(shot->bullet));
            shot->lt_count += 1000/Utils::kFPS;
            if(shot->lt_count >= shot->life_time){
                shot->is_active = false;
            }
        }
    }

    void DrawShot(Shot *shot){
        if(shot->is_active){
            PolyLibJMATH::DrawPoly(shot->bullet,true);
        }
    }
}
