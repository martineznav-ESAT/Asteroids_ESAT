#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "./Particles.h"

#include "../CustomLibs/PolyLibJMATH.h"

namespace Particles{

    void Init(){

    }

    Particle NewParticle(ParticleType type){
        Particle new_particle;
        switch(type){
            case ParticleType::PLAYER_DEATH:
                PolyLibJMATH::InitPoly(
                    &new_particle.figure,
                    2,
                    {25.0f,25.0f},
                    0.0f,
                    {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f},
                    {255,255,255},
                    {0.0f,0.0f}
                );
                
                new_particle.life_time = 2000; //ms
                new_particle.max_speed = 2;
                new_particle.accel = new_particle.max_speed;
                new_particle.decel = 0.99;
            break;

            case ParticleType::ENEMY_DEATH:
                PolyLibJMATH::InitPoly(
                    &new_particle.figure,
                    4,
                    {2.0f,2.0f},
                    45.0f,
                    {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f},
                    {255,255,255},
                    {0.0f,0.0f}
                );
                new_particle.life_time = 3000; //ms
                new_particle.max_speed = 5;
                new_particle.accel = new_particle.max_speed;
                new_particle.decel = 0.985;
            break;
        }
        
        new_particle.fwd = {0.0f,0.0f};
        new_particle.speed_v = {0.0f,0.0f};
        new_particle.type = type;
        new_particle.lt_count = 0; //ms timer
        
        new_particle.is_active = false;
        return new_particle;
    }

    void AccelerateParticle(Particle *particle){
        particle->speed_v = JMATH::Vec3Sum(particle->speed_v, JMATH::Vec3Scale(particle->fwd, particle->accel/Utils::kFPS));
        if(JMATH::Vec2Length(JMATH::Vec3ToVec2(particle->speed_v)) > particle->max_speed){
            particle->speed_v = JMATH::Vec3Scale(JMATH::Vec3Norm(particle->speed_v), particle->max_speed);
        }
    }

    void DecelerateParticle(Particle *particle){
        particle->speed_v = JMATH::Vec3Scale(particle->speed_v, particle->decel);
    }

    void LoadParticle(Particle *particle, JMATH::Vec2 position){
        
        switch(particle->type){
            case ParticleType::PLAYER_DEATH:
            case ParticleType::ENEMY_DEATH:
                particle->fwd = {
                    Utils::GenerateRandomFloatNegative(2),
                    Utils::GenerateRandomFloatNegative(2)
                };
                AccelerateParticle(particle);
                particle->lt_count = 0; //ms timer
                particle->figure.transform.translation = position;
                particle->is_active = true;
            break;
        }
    }

    void UpdateParticle(Particle *particle){
        if(particle->is_active){
            switch(particle->type){
                case ParticleType::PLAYER_DEATH:
                case ParticleType::ENEMY_DEATH:
                    DecelerateParticle(particle);
                    particle->lt_count += 1000/Utils::kFPS;
                    if(particle->lt_count >= particle->life_time){
                        
                    }
                break;
            }
        }
    }

    void DrawParticle(Particle *particle){
        if(particle->is_active){
            PolyLibJMATH::DrawPoly(particle->figure,true);
        }
    }
}
