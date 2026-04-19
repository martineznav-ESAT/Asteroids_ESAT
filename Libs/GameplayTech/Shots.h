#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../CustomLibs/PolyLibJMATH.h"

#ifndef Shots_H
#define Shots_H

namespace Shots{
    struct Shot{
        PolyLibJMATH::Poly bullet;
        JMATH::Vec3 speed_v;
        float life_time, lt_count;
        bool is_active;
    };

    Shot NewShot();

    void FireShot(Shot *shot, JMATH::Vec2 spawn_point, float rotation, JMATH::Vec3 direction_v, float speed);

    void UpdateShot(Shot *shot);

    void DrawShot(Shot *shot);
}

#endif
