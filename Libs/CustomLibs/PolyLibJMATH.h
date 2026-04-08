#include <stdio.h>
#include <math.h>

#include "./JMATH.h"

#ifndef PolyLibJMATH_H
#define PolyLibJMATH_H

namespace PolyLibJMATH{
    struct Transform{
        JMATH::Vec2 translation;
        JMATH::Vec2 scale;
        float rotation;
    };

    struct Poly {
        int t_vertices;
        JMATH::Vec3 *local_coords;
        JMATH::Vec2 *draw_coords;
        Transform transform;
        JMATH::Vec2 center_offset;
        JMATH::Vec3 color;
    };

    void InitPoly(Poly *p, int vertices, JMATH::Vec2 scale, float rotation, JMATH::Vec2 translation, JMATH::Vec3 color = {255,0,0}, JMATH::Vec2 center_offset = {0,0});

    void InitPoly(Poly *p, int vertices, JMATH::Vec3 *custom_coords, JMATH::Vec2 scale, float rotation, JMATH::Vec2 translation, JMATH::Vec3 color = {255,0,0}, JMATH::Vec2 center_offset = {0,0});

    JMATH::Mat3 GetTransformMat3(Transform tr, JMATH::Vec2 center_offset = {0,0});

    void MovePoly(Poly *p, JMATH::Vec3 speed_v);

    void UpdatePoly(Poly *p);
    
    void DrawPoly(Poly p, bool solid);

    void EmptyPolyMemory(Poly *p);
}


#endif