#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <esat/draw.h>

#include "./JMATH.h"
#include "./PolyLibJMATH.h"
#include "./Utils.h"

#include "../GameplayTech/GameplayTech.h"

namespace PolyLibJMATH{
    void InitPoly(Poly *p, int vertices, JMATH::Vec2 scale, float rotation, JMATH::Vec2 translation, JMATH::Vec3 color, JMATH::Vec2 center_offset){
        float angle = 6.28f / (float) vertices;
        p->t_vertices = vertices;
        p->local_coords = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3) * p->t_vertices);
        p->draw_coords = (JMATH::Vec2*) malloc(sizeof(JMATH::Vec2) * p->t_vertices);

        //Por defecto almacena un círculo
        for(int i = 0; i < p->t_vertices; i++){
            *((p->local_coords)+i) = {cosf(angle*i), sinf(angle*i), 1.0f};
            // printf("Coord %d: %f - %f\n",i, (*(p->local_coords+i)).x, (*(p->local_coords+i)).y);
        }

        p->transform.scale = scale;
        p->transform.rotation = rotation;
        p->transform.translation = translation;
        p->color = color;
        p->center_offset = center_offset;
    }

    void InitPoly(Poly *p, int vertices, JMATH::Vec3 *custom_coords, JMATH::Vec2 scale, float rotation, JMATH::Vec2 translation, JMATH::Vec3 color, JMATH::Vec2 center_offset){
        float angle = 6.28f / (float) vertices;
        p->t_vertices = vertices;
        p->local_coords = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3) * p->t_vertices);
        p->draw_coords = (JMATH::Vec2*) malloc(sizeof(JMATH::Vec2) * p->t_vertices);

        //Por defecto almacena un círculo
        for(int i = 0; i < p->t_vertices; i++){
            *((p->local_coords)+i) = *(custom_coords+i);
            // printf("Coord %d: %f - %f\n",i, (*(p->local_coords+i)).x, (*(p->local_coords+i)).y);
        }

        p->transform.scale = scale;
        p->transform.rotation = rotation;
        p->transform.translation = translation;
        p->color = color;
        p->center_offset = center_offset;
    }

    JMATH::Mat3 GetTransformMat3(Transform tr, JMATH::Vec2 center_offset){
        JMATH::Mat3 tr_aux = JMATH::Mat3Identity();

        tr_aux = JMATH::Mat3MultMat3(JMATH::Mat3Translate(center_offset.x, center_offset.y), tr_aux);
        // printf("Mat3Translate\n");
        // JMATH::Mat3Print(JMATH::Mat3Translate(center_offset.x, center_offset.y));
        tr_aux = JMATH::Mat3MultMat3(JMATH::Mat3Scale(tr.scale.x, tr.scale.y), tr_aux);
        // printf("Mat3Scale\n");
        // JMATH::Mat3Print(JMATH::Mat3Scale(tr.scale.x, tr.scale.y));
        tr_aux = JMATH::Mat3MultMat3(JMATH::Mat3Rotate(tr.rotation), tr_aux);
        // printf("Mat3Rotate\n");
        // JMATH::Mat3Print(JMATH::Mat3Rotate(tr.rotation));
        tr_aux = JMATH::Mat3MultMat3(JMATH::Mat3Translate(tr.translation.x, tr.translation.y), tr_aux);

        // printf("GETTRANSFORM\n");
        // JMATH::Mat3Print(tr_aux);
        return tr_aux;
    }

    void MovePoly(Poly *p, JMATH::Vec3 speed_v){
        

        p->transform.translation = JMATH::Vec2Sum(
            p->transform.translation,
            {speed_v.x, speed_v.y}
        );

        //TO_DO Replace with border colision at Collision.h

        switch (Collisions::CollisionPolyWindowBorderExit(*p)){
            case Collisions::Border::TOP:
                p->transform.translation.y = Utils::kWindowHeight+(p->transform.scale.y);
            break;
            case Collisions::Border::RIGHT:
                p->transform.translation.x = 0-(p->transform.scale.x);
            break;
            case Collisions::Border::BOTTOM:
                p->transform.translation.y = 0-(p->transform.scale.y);
            break;
            case Collisions::Border::LEFT:
                p->transform.translation.x = Utils::kWindowWidth+(p->transform.scale.x);
            break;
        }
    }

    void UpdatePoly(Poly *p){
        JMATH::Mat3 tr_aux = JMATH::Mat3Identity();

        tr_aux = JMATH::Mat3MultMat3(JMATH::Mat3Translate(p->center_offset.x, p->center_offset.y), tr_aux);
        tr_aux = JMATH::Mat3MultMat3(JMATH::Mat3Scale(p->transform.scale.x, p->transform.scale.y), tr_aux);
        tr_aux = JMATH::Mat3MultMat3(JMATH::Mat3Rotate(p->transform.rotation), tr_aux);
        tr_aux = JMATH::Mat3MultMat3(JMATH::Mat3Translate(p->transform.translation.x, p->transform.translation.y), tr_aux);

        for(int i = 0; i < p->t_vertices; i++){
            *((p->draw_coords)+i) = JMATH::Vec3ToVec2(JMATH::Mat3MultVec3(tr_aux, {((p->local_coords)+i)->x, ((p->local_coords)+i)->y, 1.0f}));
            // printf("Coord Update %d: %f - %f\n",i, (*(p->draw_coords+i)).x, (*(p->draw_coords+i)).y);
        }
    }

    void DrawPoly(Poly p, bool solid){
        if(solid){
            esat::DrawSetFillColor(p.color.x,p.color.y,p.color.z);
        }else{
            esat::DrawSetFillColor(0,0,0,0);
        }
        esat::DrawSetStrokeColor(p.color.x,p.color.y,p.color.z);
        esat::DrawSolidPath(&(p.draw_coords->x), p.t_vertices);
    }

    void EmptyPolyMemory(Poly *p){
        free(p->draw_coords);
        free(p->local_coords);
    }
}
