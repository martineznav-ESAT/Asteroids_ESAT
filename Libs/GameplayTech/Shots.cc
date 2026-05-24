#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "./Shots.h"

#include "../CustomLibs/PolyLibJMATH.h"
#include "../CustomLibs/AudioLib.h"

#include "../../Code/Gameplay.h"

namespace Shots{

    //Initializes Shots.cc values (Currently unused but kept for code structuration)
    void Init(){

    }

    //Creates a new shot struct and returns it with default values
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

    //Fires the given shot based on the rest of the parameters
    void FireShot(Shot *shot, JMATH::Vec2 spawn_point, float rotation, JMATH::Vec3 direction_v, float speed){
        // printf("DISPARAR\n");
        AudioLib::PlaySound(AudioLib::SoundsType::FIRE);
        shot->is_active = true;
        shot->lt_count = 0;
        shot->speed_v = JMATH::Vec3Scale(JMATH::Vec3Norm(direction_v), speed);
        shot->bullet.transform.translation = spawn_point;
        shot->bullet.transform.rotation = rotation+45;
        PolyLibJMATH::SaveDrawCoords(&(shot->bullet));
    }

    //Deactivates the given shot properly
    void DestroyShot(Shot *shot){
        shot->is_active = false;
        shot->lt_count = shot->life_time;
    }

    //Updates the current shot status
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

    //Draws the given shot on screen
    void DrawShot(Shot *shot){
        if(shot->is_active){
            PolyLibJMATH::DrawPoly(shot->bullet,true);
            // esat::DrawSetStrokeColor(255,0,0);
            // esat::DrawLine(shot->speed_v.x, shot->speed_v.y, JMATH::Vec3Scale(shot->speed_v,10).x, JMATH::Vec3Scale(shot->speed_v,10).y);
        }
    }
}
