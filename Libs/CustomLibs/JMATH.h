#include <stdio.h>
#include <math.h>

#ifndef JMATH_H
#define JMATH_H

namespace JMATH{
    extern const float kPI;

    //VEC2
    struct Vec2{
        float x, y;
    };

    void Vec2Print(Vec2 v);

    Vec2 Vec2Sum(Vec2 v1, Vec2 v2);

    Vec2 Vec2Sub(Vec2 v1, Vec2 v2);

    Vec2 Vec2Scale(Vec2 v, float s);

    Vec2 Vec2Unscale(Vec2 v, float s);

    float Vec2Length(Vec2 v);

    Vec2 Vec2Norm(Vec2 v);

    bool Vec2IsNorm(Vec2 v);

    Vec2 Vec2GetPerp1(Vec2 v);

    Vec2 Vec2GetPerp2(Vec2 v);

    //VEC3
    struct Vec3{
        float x, y, z;
    };

    void Vec3Print(Vec3 v);

    Vec3 Vec3Sum(Vec3 v1, Vec3 v2);

    Vec3 Vec3Sub(Vec3 v1, Vec3 v2);

    Vec3 Vec3Scale(Vec3 v, float s);

    Vec3 Vec3Unscale(Vec3 v, float s);

    float Vec3Length(Vec3 v);

    Vec3 Vec3Norm(Vec3 v);

    bool Vec3IsNorm(Vec3 v);

    Vec2 Vec3ToVec2(Vec3 v);
    
    //VEC4
    struct Vec4{
        float x, y, z, w;
    };

    void Vec4Print(Vec4 v);

    Vec4 Vec4Sum(Vec4 v1, Vec4 v2);

    Vec4 Vec4Sub(Vec4 v1, Vec4 v2);

    Vec4 Vec4Scale(Vec4 v, float s);

    Vec4 Vec4Unscale(Vec4 v, float s);

    float Vec4Length(Vec4 v);

    Vec4 Vec4Norm(Vec4 v);

    bool Vec4IsNorm(Vec4 v);

    //Mat2
    struct Mat2{
        float d[4] = {0,0,0,0};
    };

    int Mat2Length();

    int Mat2Columns();

    int Mat2Index(int r, int c);

    void Mat2Print(Mat2 m1);

    Mat2 Mat2Sum(Mat2 m1, Mat2 m2);

    Mat2 Mat2Sub(Mat2 m1, Mat2 m2);

    Mat2 Mat2Scale(Mat2 m1, float s);

    Mat2 Mat2Unscale(Mat2 m1, float s);

    Mat2 Mat2MultMat2(Mat2 m1, Mat2 m2);

    Vec2 Mat2MultVec2(Mat2 m1, Vec2 v1);

    Mat2 Mat2Identity();

    //Mat3
    struct Mat3{
        float d[9] = {
            0,0,0,
            0,0,0,
            0,0,0
        };
    };

    int Mat3Length();

    int Mat3Columns();

    int Mat3Index(int r, int c);

    void Mat3Print(Mat3 m1);

    Mat3 Mat3Sum(Mat3 m1, Mat3 m2);

    Mat3 Mat3Sub(Mat3 m1, Mat3 m2);

    Mat3 Mat3Scale(Mat3 m1, float s);

    Mat3 Mat3Unscale(Mat3 m1, float s);

    Mat3 Mat3MultMat3(Mat3 m1, Mat3 m2);

    Vec3 Mat3MultVec3(Mat3 m1, Vec3 v1);

    Mat3 Mat3Identity();

    Mat3 Mat3Scale(float sx, float sy);

    Mat3 Mat3Rotate(float radians);

    Mat3 Mat3Translate(float tx, float ty);

    //Mat4
    struct Mat4{
        float d[16] = {
            0,0,0,0,
            0,0,0,0,
            0,0,0,0,
            0,0,0,0
        };
    };

    int Mat4Length();

    int Mat4Columns();

    int Mat4Index(int r, int c);

    void Mat4Print(Mat4 m1);

    Mat4 Mat4Sum(Mat4 m1, Mat4 m2);

    Mat4 Mat4Sub(Mat4 m1, Mat4 m2);

    Mat4 Mat4Scale(Mat4 m1, float s);

    Mat4 Mat4Unscale(Mat4 m1, float s);

    Mat4 Mat4MultMat4(Mat4 m1, Mat4 m2);

    Vec4 Mat4MultVec4(Mat4 m1, Vec4 v1);

    Mat4 Mat4Identity();

    float DegreesToRadians(float degrees);

    float RadiansToDegrees(float radians);
}

#endif