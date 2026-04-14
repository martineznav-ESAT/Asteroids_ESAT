#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../CustomLibs/PolyLibJMATH.h"

#ifndef Ufo_H
#define Ufo_H

namespace Ufo{

    struct UfoShip{
        PolyLibJMATH::Poly figure;
        JMATH::Vec3 fwd;
        JMATH::Vec3 speed_v;
        // Shot shot;
    };

    void Init();

    UfoShip NewUfo();

    void UfoShoot(UfoShip *ufo);

    void UpdateUfoShot(UfoShip* ufo);

    void UpdateUfoFwd(UfoShip* ufo);

    void EmptyUfoMemory(UfoShip* ufo);
}

#endif
