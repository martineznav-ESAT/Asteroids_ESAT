#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../CustomLibs/JMATH.h"
#include "../CustomLibs/PolyLibJMATH.h"

#ifndef Asteroids_H
#define Asteroids_H

namespace Asteroids{
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
        int size_level = 1;
    };

    void Init();

    int GetAsteroidTypeVertices(AsteroidType type);

    Asteroid NewAsteroid(AsteroidType type, int size_level);

    void EmptyMemory();
}

#endif
