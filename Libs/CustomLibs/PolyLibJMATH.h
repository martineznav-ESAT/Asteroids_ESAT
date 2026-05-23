#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <esat/draw.h>

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
        JMATH::Vec2 *prev_draw_coords;
        Transform transform;
        JMATH::Vec2 center_offset;
        JMATH::Vec3 color;
    };

    void InitPoly(Poly *p, int vertices, JMATH::Vec2 scale, float rotation, JMATH::Vec2 translation, JMATH::Vec3 color = {255,0,0}, JMATH::Vec2 center_offset = {0,0});

    void InitPoly(Poly *p, int vertices, JMATH::Vec3 *custom_coords, JMATH::Vec2 scale, float rotation, JMATH::Vec2 translation, JMATH::Vec3 color = {255,0,0}, JMATH::Vec2 center_offset = {0,0});

    JMATH::Mat3 GetTransformMat3(Transform tr, JMATH::Vec2 center_offset = {0,0});

    void MovePoly(Poly *p, JMATH::Vec3 speed_v);

    //Updates only the prev draw coords based on the actual draw coords 
    void SavePrevDrawCoords(Poly *p);

    //Updates the draw coords based on the poly data. Also saves the actual draw coords as previous draw coords
    void SaveDrawCoords(Poly *p);

    void UpdatePoly(Poly *p);
    
    void DrawPoly(Poly p, bool solid);
    void DrawPoly(Poly p, bool solid, JMATH::Vec3 border_color);

    void EmptyPolyMemory(Poly *p);
}


#endif