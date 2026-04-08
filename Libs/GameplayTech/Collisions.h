#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "../CustomLibs/PolyLibJMATH.h"
#include "../CustomLibs/JMATH.h"

namespace Collisions{
    enum Border{
        TOP,
        RIGHT,
        BOTTOM,
        LEFT,
        NONE
    };

    Border CollisionPolyWindowBorderExit(PolyLibJMATH::Poly poly);

    bool CollisionPolyPoint(PolyLibJMATH::Poly poly, JMATH::Vec2 point);

    bool CollisionPolyPoly(PolyLibJMATH::Poly p1, PolyLibJMATH::Poly p2);

    //DEBUG COLLISIONS
    //CREATED FOR TESTING PURPOSES
    bool CollisionPolyOnRClick(PolyLibJMATH::Poly p1);
}
