#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "../CustomLibs/PolyLibJMATH.h"
#include "../CustomLibs/JMATH.h"
#include "../CustomLibs/TList.h"

#include "./Ufo.h"

#ifndef Collisions_H
#define Collisions_H

namespace Collisions{

    enum Border{
        TOP,
        RIGHT,
        BOTTOM,
        LEFT,
        NONE
    };

    extern bool show_colliders;

    Border CollisionPolyWindowBorderExit(PolyLibJMATH::Poly poly);
    void BorderExitRellocation(PolyLibJMATH::Poly *poly);

    bool CollisionPolyPoly(PolyLibJMATH::Poly p1, PolyLibJMATH::Poly p2);

    bool CollisionAsteroidPlayerShots(TList::ListNode** asteroid_list, TList::ListNode** particle_list ,Asteroids::Asteroid *asteroid, Players::Player *player);
    
    bool CollisionAsteroidPlayer(TList::ListNode** asteroid_list, TList::ListNode** particle_list, Asteroids::Asteroid *asteroid, Players::Player *player);

    bool CollisionAsteroidUfoShot(TList::ListNode** asteroid_list, TList::ListNode** particle_list, Asteroids::Asteroid *asteroid, Ufo::UfoShip *ufo);
    
    bool CollisionAsteroidUfo(TList::ListNode** asteroid_list, TList::ListNode** particle_list, Asteroids::Asteroid *asteroid, Ufo::UfoShip *ufo);

    bool CollisionUfoPlayer(Ufo::UfoShip *ufo, Players::Player *player);
    
    bool CollisionUfoPlayerShots(Ufo::UfoShip *ufo, Players::Player *player);
    
    bool CollisionUfoShotPlayer(Ufo::UfoShip *ufo, Players::Player *player);

    bool CollisionPowerUpPlayer(PowerUps::PowerUp *pu, Players::Player *player);

    bool CollisionPlayerShotsPlayer(Players::Player *player_shooting, Players::Player *other_player);

    
    //DEBUG COLLISIONS
    //CREATED FOR TESTING PURPOSES
    bool CollisionPolyOnRClick(PolyLibJMATH::Poly p1);

}

#endif
