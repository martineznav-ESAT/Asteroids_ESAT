#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "./Ufo.h"

#include "../CustomLibs/PolyLibJMATH.h"

#include "../../Code/Gameplay.h"

namespace Ufo{
    JMATH::Vec3 *ufo_coords = nullptr;

    void Init(){
        ufo_coords = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3)*8);

        *(ufo_coords+0) = {-1.0f, 0.0f, 1.0f};      // 1
        *(ufo_coords+1) = {-0.5f, -0.4f, 1.0f};     // 2
        *(ufo_coords+2) = {-0.25f, -0.85f, 1.0f};   // 3
        *(ufo_coords+3) = {0.25f, -0.85f, 1.0f};    // 4
        *(ufo_coords+4) = {0.5f, -0.4f, 1.0f};      // 5
        *(ufo_coords+5) = {1.0f, 0.0f, 1.0f};       // 6
        *(ufo_coords+6) = {0.55f, 0.4f, 1.0f};      // 7
        *(ufo_coords+7) = {-0.55f, 0.4f, 1.0f};     // 8
    }

    UfoShip NewUfo(){
        UfoShip new_ufo;
        PolyLibJMATH::InitPoly(
            &new_ufo.figure,
            8,
            ufo_coords,
            {20.0f,20.0f},
            0.0f,
            {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f},
            {255,255,255},
            {0.0f,0.0f}
        );
        new_ufo.fwd = {0.0f,0.0f};
        new_ufo.speed_v = {0.0f,0.0f};
        // new_ufo.shot = NewShot();

        return new_ufo;
    }


    void UfoShoot(UfoShip *ufo){
        
    }

    void UpdateUfoShot(UfoShip* ufo){
        
    }

    void UpdateUfoFwd(UfoShip* ufo){
        // float radianBase = JMATH::DegreesToRadians(360.0f/ship->figure.t_vertices);
        // float radianRotation = JMATH::DegreesToRadians(ship->figure.transform.rotation);
        // ship->fwd = {cosf(radianRotation), sinf(radianRotation)};
    }


    void EmptyUfoMemory(UfoShip* ufo){
        PolyLibJMATH::EmptyPolyMemory(&(ufo->figure));
    }
}
