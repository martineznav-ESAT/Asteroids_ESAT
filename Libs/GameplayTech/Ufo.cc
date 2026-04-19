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
        *(ufo_coords+1) = {-0.5f, -0.3f, 1.0f};     // 2
        *(ufo_coords+2) = {-0.25f, -0.70f, 1.0f};   // 3
        *(ufo_coords+3) = {0.25f, -0.70f, 1.0f};    // 4
        *(ufo_coords+4) = {0.5f, -0.3f, 1.0f};      // 5
        *(ufo_coords+5) = {1.0f, 0.0f, 1.0f};       // 6
        *(ufo_coords+6) = {0.55f, 0.3f, 1.0f};      // 7
        *(ufo_coords+7) = {-0.55f, 0.3f, 1.0f};     // 8
    }

    UfoShip NewUfo(){
        UfoShip new_ufo;
        PolyLibJMATH::InitPoly(
            &new_ufo.figure,
            8,
            ufo_coords,
            {40.0f,40.0f},
            0.0f,
            {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f},
            {255,255,255},
            {0.0f,0.0f}
        );
        new_ufo.speed = 1.0f;
        new_ufo.orientation = Orientation::LEFT;
        new_ufo.fwd = {new_ufo.speed,0.0f,0.0f};
       
        Ufo::UfoType::BIG;
        new_ufo.shot = Shots::NewShot();

        return new_ufo;
    }


    void UpdateUfoShot(UfoShip* ufo){
        // printf("UPDATE UFO SHOT\n");
        if(!(ufo->shot.is_active)){
            // printf("FireShot\n");
            FireShot(
                &(ufo->shot),
                ufo->figure.transform.translation,
                ufo->figure.transform.rotation,
                {Utils::GenerateRandomFloatNegative(2),Utils::GenerateRandomFloatNegative(2),0.0f},
                5
            );
        }else{
            Shots::UpdateShot(&(ufo->shot));
        }
    }

    void RandomDirection(UfoShip* ufo){
        int random = Utils::GenerateRandomNumber(250);
        // printf("RANDOM DIRECTION %d\n",random);
        switch (random){
            case 0:
                ufo->fwd = JMATH::Vec3Scale(JMATH::Vec3Norm({(float)ufo->orientation, -1.0f, 0.0f}),ufo->speed); //Up
            break;
            case 1:
                ufo->fwd = JMATH::Vec3Scale(JMATH::Vec3Norm({(float)ufo->orientation, 1.0f, 0.0f}),ufo->speed); //Down
            break;
            case 2:
                ufo->fwd = {ufo->orientation*ufo->speed, 0.0f, 0.0f}; //Straight
            break;
            // 3-249 Keeps current direction
        }
    }

    void UpdateUfo(UfoShip *ufo){
        // printf("UpdateUfo\n");
        UpdateUfoShot(ufo);

        RandomDirection(ufo);
        // JMATH::Vec3Print(ufo->fwd);

        PolyLibJMATH::MovePoly(&(ufo->figure), ufo->fwd);
        Collisions::BorderExitRellocation(&(ufo->figure));
        PolyLibJMATH::UpdatePoly(&(ufo->figure));
    }

    void DrawUfo(UfoShip ufo){
        Shots::DrawShot(&(ufo.shot));

        // printf("DrawUfo\n");
        // for (int i = 0; i < 8; i++){
        //     JMATH::Vec2Print(*(ufo.figure.draw_coords+i));
        // }

        
        PolyLibJMATH::DrawPoly(ufo.figure, false);
        esat::DrawLine(
            (ufo.figure.draw_coords+0)->x,(ufo.figure.draw_coords+0)->y,
            (ufo.figure.draw_coords+5)->x,(ufo.figure.draw_coords+5)->y
        );
        esat::DrawLine(
            (ufo.figure.draw_coords+1)->x,(ufo.figure.draw_coords+1)->y,
            (ufo.figure.draw_coords+4)->x,(ufo.figure.draw_coords+4)->y
        );
    }


    void EmptyUfoMemory(UfoShip* ufo){
        PolyLibJMATH::EmptyPolyMemory(&(ufo->figure));
    }
}
