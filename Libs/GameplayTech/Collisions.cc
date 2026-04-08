#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include <esat/input.h>

#include "./Collisions.h"

#include "../CustomLibs/PolyLibJMATH.h"
#include "../CustomLibs/JMATH.h"
#include "../CustomLibs/Utils.h"

namespace Collisions{
    Border CollisionPolyWindowBorderExit(PolyLibJMATH::Poly poly){
        Border collided = Border::NONE;
        float aux_position_x = 0;
        float aux_position_nx = 0;
        float aux_position_y = 0;
        float aux_position_ny = 0;

        aux_position_x = poly.transform.translation.x - poly.transform.scale.x;
        aux_position_nx = poly.transform.translation.x + poly.transform.scale.x;
        aux_position_y = poly.transform.translation.y - poly.transform.scale.y;
        aux_position_ny = poly.transform.translation.y + poly.transform.scale.y;
        if(aux_position_x > Utils::kWindowWidth){
            collided = Border::RIGHT;
        }else{
            if(aux_position_nx < 0){
                collided = Border::LEFT;
            }
        }

        if(aux_position_y > Utils::kWindowHeight){
                collided = Border::BOTTOM;
        }else{
            if(aux_position_ny < 0){
                collided = Border::TOP;
            }
        }
        return collided;
    }

    //TO_DO COLLISIONS
    bool CollisionPolyPoint(PolyLibJMATH::Poly poly, JMATH::Vec2 point){
        return false;
    }

    bool CollisionPolyPoly(PolyLibJMATH::Poly p1, PolyLibJMATH::Poly p2){
        return false;
    }

    //DEBUG COLLISIONS
    //CREATED FOR TESTING PURPOSES
    bool CollisionPolyOnRClick(PolyLibJMATH::Poly p1){
        return esat::MouseButtonDown(1);
    }
}
