#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../CustomLibs/PolyLibJMATH.h"
#include "./Shots.h"

#ifndef Ufo_H
#define Ufo_H

namespace Ufo{
    enum Orientation{
        LEFT = -1,
        RIGHT = 1
    };

    enum UfoType{
        SMALL,
        BIG,
        NONE
    };

    struct UfoShip{
        PolyLibJMATH::Poly figure;
        JMATH::Vec3 fwd;
        float speed;
        UfoType type;
        Orientation orientation;
        int direction_change_lt = 1000;
        int direction_change_ltc = 0;
        int spawn_lt = 15000;
        int spawn_ltc = 0;
        Shots::Shot shot;
        bool is_first_shot = true;
    };

    void Init();

    UfoShip NewUfo();

    void DestroyUfo(UfoShip* ufo);

    void SpawnUfo(UfoShip* ufo);

    void UfoShoot(UfoShip *ufo);

    void UpdateUfoShot(UfoShip* ufo);

    void UpdateUfoFwd(UfoShip* ufo);

    void UpdateUfo(UfoShip* ufo);

    void DrawUfo(UfoShip ufo);

    void EmptyUfoMemory(UfoShip* ufo);
}

#endif
