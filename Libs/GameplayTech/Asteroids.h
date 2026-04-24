#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../CustomLibs/JMATH.h"
#include "../CustomLibs/PolyLibJMATH.h"
// #include "../CustomLibs/TList.h"

#include "./GameplayTech.h"

#ifndef Asteroids_H
#define Asteroids_H

namespace Asteroids{
    extern int last_asteroid_id;
    
    enum AsteroidType{
        A1,
        A2,
        A3,
        A4,
        TOTAL_ASTEROIDS
    };

    enum AsteroidTypeVertices{
        A1_TV = 12,
        A2_TV = 11,
        A3_TV = 10,
        A4_TV = 12
    };

    struct Asteroid{
        int id = 0;
        AsteroidType type;
        PolyLibJMATH::Poly figure;
        JMATH::Vec3 speed_v;
        Particles::Particle* destroy_particles;
        int size_level = 1;
    };

    void Init();

    int GetAsteroidTypeVertices(AsteroidType type);

    Asteroid NewAsteroid(AsteroidType type, int size_level);

    void DestroyAsteroid(void **asteroid_list, void **particle_list, Asteroid *asteroid, Players::Player *player);

    void EmptyMemory();
}

#endif
