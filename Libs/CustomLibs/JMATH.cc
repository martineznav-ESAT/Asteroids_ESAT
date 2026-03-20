#include <stdio.h>
#include <math.h>

#include "./JMATH.h"

namespace JMATH{
    float PI = 3.141592;

    //VEC2
    void Vec2Print(Vec2 v){
        printf("[X] %.2f [Y] %.2f\n", v.x, v.y);
    }

    Vec2 Vec2Sum(Vec2 v1, Vec2 v2){
        return {v1.x+v2.x, v1.y+v2.y};
    }

    Vec2 Vec2Sub(Vec2 v1, Vec2 v2){
        return {v1.x-v2.x, v1.y-v2.y};
    }

    Vec2 Vec2Scale(Vec2 v, float s){
        return {v.x*s, v.y*s};
    }

    Vec2 Vec2Unscale(Vec2 v, float s){
        return {v.x/s, v.y/s};
    }

    float Vec2Length(Vec2 v){
        return sqrtf((v.x*v.x) + (v.y*v.y));
    }

    Vec2 Vec2Norm(Vec2 v){
        float vl = Vec2Length(v);
        return {v.x/vl , v.y/vl};
    }

    bool Vec2IsNorm(Vec2 v){
        float vl = Vec2Length(v);
        return (vl<1.2 && vl>0.8);
    }

    Vec2 Vec2GetPerp1(Vec2 v){
        return {v.y*-1, v.x};
    }

    Vec2 Vec2GetPerp2(Vec2 v){
        return {v.y, v.x*-1};
    }

    //VEC3
    void Vec3Print(Vec3 v){
        printf("[X] %.2f [Y] %.2f [Z] %.2f\n", v.x, v.y, v.z);
    }

    Vec3 Vec3Sum(Vec3 v1, Vec3 v2){
        return {v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
    }

    Vec3 Vec3Sub(Vec3 v1, Vec3 v2){
        return {v1.x-v2.x, v1.y-v2.y, v1.z-v2.z};
    }

    Vec3 Vec3Scale(Vec3 v, float s){
        return {v.x*s, v.y*s, v.z*s};
    }

    Vec3 Vec3Unscale(Vec3 v, float s){
        return {v.x/s, v.y/s, v.z/s};
    }

    float Vec3Length(Vec3 v){
        return sqrtf((v.x*v.x) + (v.y*v.y) + (v.z*v.z));
    }

    Vec3 Vec3Norm(Vec3 v){
        float vl = Vec3Length(v);
        return {v.x/vl , v.y/vl, v.z/vl};
    }

    bool Vec3IsNorm(Vec3 v){
        float vl = Vec3Length(v);
        return (vl<1.2 && vl>0.8);
    }

    //VEC4

    void Vec4Print(Vec4 v){
        printf("[X] %.2f [Y] %.2f [Z] %.2f [W] %.2f\n", v.x, v.y, v.z, v.w);
    }

    Vec4 Vec4Sum(Vec4 v1, Vec4 v2){
        return {v1.x+v2.x, v1.y+v2.y, v1.z+v2.z, v1.w+v2.w};
    }

    Vec4 Vec4Sub(Vec4 v1, Vec4 v2){
        return {v1.x-v2.x, v1.y-v2.y, v1.z-v2.z, v1.w-v2.w};
    }

    Vec4 Vec4Scale(Vec4 v, float s){
        return {v.x*s, v.y*s, v.z*s, v.w*s};
    }

    Vec4 Vec4Unscale(Vec4 v, float s){
        return {v.x/s, v.y/s, v.z/s, v.w/s};
    }

    float Vec4Length(Vec4 v){
        return sqrtf((v.x*v.x) + (v.y*v.y) + (v.z*v.z) + (v.w*v.w));
    }

    Vec4 Vec4Norm(Vec4 v){
        float vl = Vec4Length(v);
        return {v.x/vl , v.y/vl, v.z/vl, v.w/vl};
    }

    bool Vec4IsNorm(Vec4 v){
        float vl = Vec4Length(v);
        return (vl<1.2 && vl>0.8);
    }

    //Mat2

    int Mat2Length(){
        return 4;
    }

    int Mat2Columns(){
        return 2;
    }

    int Mat2Index(int r, int c){
        return ((r*Mat2Columns())+c);
    }

    void Mat2Print(Mat2 m1){
        int m_length = Mat2Length();
        int m_c = Mat2Columns();
        printf("Matriz L[%d]:\n",m_length);
        for(int i = 0, j=0; i < m_length; i++, ++j %= m_c){
            printf("[%.2f] ",m1.d[i]);
            if(i != 0 && j >= m_c-1){
                printf("\n");
            }
        }
    }

    Mat2 Mat2Sum(Mat2 m1, Mat2 m2){
        Mat2 result;
        int mat_length = Mat2Length();
        for(int i = 0; i < mat_length; i++){
            result.d[i] = m1.d[i] + m2.d[i]; 
        }
        return result;
    }

    Mat2 Mat2Sub(Mat2 m1, Mat2 m2){
        Mat2 result;
        int mat_length = Mat2Length();
        for(int i = 0; i < mat_length; i++){
            result.d[i] = m1.d[i] - m2.d[i]; 
        }
        return result;
    }

    Mat2 Mat2Scale(Mat2 m1, float s){
        Mat2 result;
        int mat_length = Mat2Length();
        for(int i = 0; i < mat_length; i++){
            result.d[i] = m1.d[i] * s; 
        }
        return result;
    }

    Mat2 Mat2Unscale(Mat2 m1, float s){
        Mat2 result;
        int mat_length = Mat2Length();
        for(int i = 0; i < mat_length; i++){
            result.d[i] = m1.d[i] / s; 
        }
        return result;
    }

    Mat2 Mat2MultMat2(Mat2 m1, Mat2 m2){
        Mat2 result;
        
        for (int i = 0; i < 2; i++){
            result.d[Mat2Index(i,0)] = 
                (m1.d[Mat2Index(i,0)]*m2.d[Mat2Index(0,0)]) + 
                (m1.d[Mat2Index(i,1)]*m2.d[Mat2Index(1,0)]);
        
            result.d[Mat2Index(i,1)] = 
                (m1.d[Mat2Index(i,0)]*m2.d[Mat2Index(0,1)]) + 
                (m1.d[Mat2Index(i,1)]*m2.d[Mat2Index(1,1)]);
        }
        
        return result;
    }

    Vec2 Mat2MultVec2(Mat2 m1, Vec2 v1){
        Vec2 result;

        result.x = (m1.d[Mat2Index(0,0)]*v1.x) + (m1.d[Mat2Index(0,1)]*v1.y);
        result.y = (m1.d[Mat2Index(1,0)]*v1.x) + (m1.d[Mat2Index(1,1)]*v1.y);
        return result;
    }

    Mat2 Mat2Identity(){
        return
        {
            1,0,
            0,1
        };
    }

    //Mat3

    int Mat3Length(){
        return 9;
    }

    int Mat3Columns(){
        return 3;
    }

    int Mat3Index(int r, int c){
        return ((r*Mat3Columns())+c);
    }

    void Mat3Print(Mat3 m1){
        int m_length = Mat3Length();
        int m_c = Mat3Columns();
        printf("Matriz L[%d]:\n",m_length);
        for(int i = 0, j=0; i < m_length; i++, ++j %= m_c){
            printf("[%.2f] ",m1.d[i]);
            if(i != 0 && j >= m_c-1){
                printf("\n");
            }
        }
    }

    Mat3 Mat3Sum(Mat3 m1, Mat3 m2){
        Mat3 result;
        int mat_length = Mat3Length();
        for(int i = 0; i < mat_length; i++){
            result.d[i] = m1.d[i] + m2.d[i]; 
        }
        return result;
    }

    Mat3 Mat3Sub(Mat3 m1, Mat3 m2){
        Mat3 result;
        int mat_length = Mat3Length();
        for(int i = 0; i < mat_length; i++){
            result.d[i] = m1.d[i] - m2.d[i]; 
        }
        return result;
    }

    Mat3 Mat3Scale(Mat3 m1, float s){
        Mat3 result;
        int mat_length = Mat3Length();
        for(int i = 0; i < mat_length; i++){
            result.d[i] = m1.d[i] * s; 
        }
        return result;
    }

    Mat3 Mat3Unscale(Mat3 m1, float s){
        Mat3 result;
        int mat_length = Mat3Length();
        for(int i = 0; i < mat_length; i++){
            result.d[i] = m1.d[i] / s; 
        }
        return result;
    }

    Mat3 Mat3MultMat3(Mat3 m1, Mat3 m2){
        Mat3 result;
        for (int i = 0; i < 3; i++){
            result.d[Mat3Index(i,0)] = 
                (m1.d[Mat3Index(i,0)]*m2.d[Mat3Index(0,0)]) + 
                (m1.d[Mat3Index(i,1)]*m2.d[Mat3Index(1,0)]) +
                (m1.d[Mat3Index(i,2)]*m2.d[Mat3Index(2,0)]);
        
            result.d[Mat3Index(i,1)] = 
                (m1.d[Mat3Index(i,0)]*m2.d[Mat3Index(0,1)]) + 
                (m1.d[Mat3Index(i,1)]*m2.d[Mat3Index(1,1)]) +
                (m1.d[Mat3Index(i,2)]*m2.d[Mat3Index(2,1)]);
            
            result.d[Mat3Index(i,2)] = 
                (m1.d[Mat3Index(i,0)]*m2.d[Mat3Index(0,2)]) + 
                (m1.d[Mat3Index(i,1)]*m2.d[Mat3Index(1,2)]) +
                (m1.d[Mat3Index(i,2)]*m2.d[Mat3Index(2,2)]);
        }
        
        return result;
    }

    Vec3 Mat3MultVec3(Mat3 m1, Vec3 v1){
        Vec3 result;
        result.x = (m1.d[Mat3Index(0,0)]*v1.x) + (m1.d[Mat3Index(0,1)]*v1.y) + (m1.d[Mat3Index(0,2)]*v1.z);
        result.y = (m1.d[Mat3Index(1,0)]*v1.x) + (m1.d[Mat3Index(1,1)]*v1.y) + (m1.d[Mat3Index(1,2)]*v1.z);
        result.z = (m1.d[Mat3Index(2,0)]*v1.x) + (m1.d[Mat3Index(2,1)]*v1.y) + (m1.d[Mat3Index(2,2)]*v1.z);

        return result;
    }

    Mat3 Mat3Identity(){
        return
        {
            1,0,0,
            0,1,0,
            0,0,1
        };
    }

    Mat3 Mat3Scale(float sx, float sy){
        Mat3 mat = Mat3Identity();
        mat.d[Mat3Index(0,0)] = sx;
        mat.d[Mat3Index(1,1)] = sy;
        return mat;
    }

    Mat3 Mat3Rotate(float radians){
        Mat3 mat = Mat3Identity();
        mat.d[Mat3Index(0,0)] = cosf(radians);
        mat.d[Mat3Index(0,1)] = sinf(radians)*-1.0f;
        mat.d[Mat3Index(1,0)] = sinf(radians);
        mat.d[Mat3Index(1,1)] = cosf(radians);
        return mat;
    }


    Mat3 Mat3Translate(float tx, float ty){
        Mat3 mat = Mat3Identity();
        mat.d[Mat3Index(0,2)] = tx;
        mat.d[Mat3Index(1,2)] = ty;
        return mat;
    }


    //Mat4

    int Mat4Length(){
        return 16;
    }

    int Mat4Columns(){
        return 4;
    }

    int Mat4Index(int r, int c){
        return ((r*Mat4Columns())+c);
    }

    void Mat4Print(Mat4 m1){
        int m_length = Mat4Length();
        int m_c = Mat4Columns();
        printf("Matriz L[%d]:\n",m_length);
        for(int i = 0, j=0; i < m_length; i++, ++j %= m_c){
            printf("[%.2f] ",m1.d[i]);
            if(i != 0 && j >= m_c-1){
                printf("\n");
            }
        }
    }

    Mat4 Mat4Sum(Mat4 m1, Mat4 m2){
        Mat4 result;
        int mat_length = Mat4Length();
        for(int i = 0; i < mat_length; i++){
            result.d[i] = m1.d[i] + m2.d[i]; 
        }
        return result;
    }

    Mat4 Mat4Sub(Mat4 m1, Mat4 m2){
        Mat4 result;
        int mat_length = Mat4Length();
        for(int i = 0; i < mat_length; i++){
            result.d[i] = m1.d[i] - m2.d[i]; 
        }
        return result;
    }

    Mat4 Mat4Scale(Mat4 m1, float s){
        Mat4 result;
        int mat_length = Mat4Length();
        for(int i = 0; i < mat_length; i++){
            result.d[i] = m1.d[i] * s; 
        }
        return result;
    }

    Mat4 Mat4Unscale(Mat4 m1, float s){
        Mat4 result;
        int mat_length = Mat4Length();
        for(int i = 0; i < mat_length; i++){
            result.d[i] = m1.d[i] / s; 
        }
        return result;
    }

    Mat4 Mat4MultMat4(Mat4 m1, Mat4 m2){
        Mat4 result;
        for (int i = 0; i < 4; i++){
            result.d[Mat4Index(i,0)] = 
                (m1.d[Mat4Index(i,0)]*m2.d[Mat4Index(0,0)]) + 
                (m1.d[Mat4Index(i,1)]*m2.d[Mat4Index(1,0)]) +
                (m1.d[Mat4Index(i,2)]*m2.d[Mat4Index(2,0)]) +
                (m1.d[Mat4Index(i,3)]*m2.d[Mat4Index(3,0)]);
        
            result.d[Mat4Index(i,1)] = 
                (m1.d[Mat4Index(i,0)]*m2.d[Mat4Index(0,1)]) + 
                (m1.d[Mat4Index(i,1)]*m2.d[Mat4Index(1,1)]) +
                (m1.d[Mat4Index(i,2)]*m2.d[Mat4Index(2,1)]) +
                (m1.d[Mat4Index(i,3)]*m2.d[Mat4Index(3,1)]);
            
            result.d[Mat4Index(i,2)] = 
                (m1.d[Mat4Index(i,0)]*m2.d[Mat4Index(0,2)]) + 
                (m1.d[Mat4Index(i,1)]*m2.d[Mat4Index(1,2)]) +
                (m1.d[Mat4Index(i,2)]*m2.d[Mat4Index(2,2)]) +
                (m1.d[Mat4Index(i,3)]*m2.d[Mat4Index(3,2)]);

            result.d[Mat4Index(i,3)] = 
                (m1.d[Mat4Index(i,0)]*m2.d[Mat4Index(0,3)]) + 
                (m1.d[Mat4Index(i,1)]*m2.d[Mat4Index(1,3)]) +
                (m1.d[Mat4Index(i,2)]*m2.d[Mat4Index(2,3)]) +
                (m1.d[Mat4Index(i,3)]*m2.d[Mat4Index(3,3)]);
        }
        
        return result;
    }

    Vec4 Mat4MultVec4(Mat4 m1, Vec4 v1){
        Vec4 result;
        result.x = (m1.d[Mat4Index(0,0)]*v1.x) + (m1.d[Mat4Index(0,1)]*v1.y) + (m1.d[Mat4Index(0,2)]*v1.z) + (m1.d[Mat4Index(0,3)]*v1.w);
        result.y = (m1.d[Mat4Index(1,0)]*v1.x) + (m1.d[Mat4Index(1,1)]*v1.y) + (m1.d[Mat4Index(1,2)]*v1.z) + (m1.d[Mat4Index(1,3)]*v1.w);
        result.z = (m1.d[Mat4Index(2,0)]*v1.x) + (m1.d[Mat4Index(2,1)]*v1.y) + (m1.d[Mat4Index(2,2)]*v1.z) + (m1.d[Mat4Index(2,3)]*v1.w);
        result.w = (m1.d[Mat4Index(3,0)]*v1.x) + (m1.d[Mat4Index(3,1)]*v1.y) + (m1.d[Mat4Index(3,2)]*v1.z) + (m1.d[Mat4Index(3,3)]*v1.w);

        return result;
    }

    Mat4 Mat4Identity(){
        return
        {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };
    }

    float DegreesToRadians(float degrees){
        return (degrees/180)*PI;
    }

    float RadiansToDegrees(float radians){
        return (radians/PI)*180;
    }
}
