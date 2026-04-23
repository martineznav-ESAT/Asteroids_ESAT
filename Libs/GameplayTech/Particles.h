#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../CustomLibs/PolyLibJMATH.h"

#ifndef Particles_H
#define Particles_H

namespace Particles{
    enum ParticleType{
        PLAYER_DEATH,
        ENEMY_DEATH
    };

    struct Particle{
        PolyLibJMATH::Poly figure;
        JMATH::Vec3 fwd;
        JMATH::Vec3 speed_v;
        ParticleType type;
        float life_time, lt_count;
        float max_speed, accel, decel;
        bool is_active;
    };

    Particle NewParticle(ParticleType type);

    void LoadParticle(Particle *particle, JMATH::Vec2 position);

    void UpdateParticle(Particle *particle);

    void DrawParticle(Particle *particle);

    void EmptyParticleMemory(Particle* particle);
}

#endif
