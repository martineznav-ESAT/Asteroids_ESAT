#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "../CustomLibs/PolyLibJMATH.h"
#include "../CustomLibs/JMATH.h"
#include "../CustomLibs/TList.h"

namespace Collisions{
    enum Border{
        TOP,
        RIGHT,
        BOTTOM,
        LEFT,
        NONE
    };

    Border CollisionPolyWindowBorderExit(PolyLibJMATH::Poly poly);
    void BorderExitRellocation(PolyLibJMATH::Poly *poly);

    bool CollisionPolyPoly(PolyLibJMATH::Poly p1, PolyLibJMATH::Poly p2);

    bool CollisionAsteroidPlayerShots(TList::ListNode** asteroid_list ,Asteroids::Asteroid *asteroid, Players::Player *player);

    //DEBUG COLLISIONS
    //CREATED FOR TESTING PURPOSES
    bool CollisionPolyOnRClick(PolyLibJMATH::Poly p1);

}
