#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../CustomLibs/PolyLibJMATH.h"

#ifndef Ufo_H
#define Ufo_H

namespace Ufo{
    enum Orientation{
        LEFT = -1,
        RIGHT = 1
    };

    enum UfoType{
        SMALL,
        BIG
    };

    struct UfoShip{
        PolyLibJMATH::Poly figure;
        JMATH::Vec3 fwd;
        float speed;
        UfoType type;
        Orientation orientation;
        int direction_change_lt = 1000;
        int direction_change_ltc = 0;
        // Shot shot;
    };

    void Init();

    UfoShip NewUfo();

    void UfoShoot(UfoShip *ufo);

    void UpdateUfoShot(UfoShip* ufo);

    void UpdateUfoFwd(UfoShip* ufo);

    void UpdateUfo(UfoShip* ufo);

    void DrawUfo(UfoShip ufo);

    void EmptyUfoMemory(UfoShip* ufo);
}

#endif
