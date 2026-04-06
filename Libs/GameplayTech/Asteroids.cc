#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "./Asteroids.h"

#include "../CustomLibs/PolyLibJMATH.h"

#include "../../Code/Gameplay.h"

namespace Asteroids{
    JMATH::Vec3 **asteroids_coords = nullptr;

    void Init(){
        asteroids_coords = (JMATH::Vec3**) malloc(sizeof(JMATH::Vec3*)*AsteroidType::TOTAL_ASTEROIDS);

        // Asteroid 1
        *(asteroids_coords+0) = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3)*AsteroidTypeVertices::A1_TV);
        *(*(asteroids_coords+0)+0) = {-0.945f, -0.374f, 1.0f};   // 1
        *(*(asteroids_coords+0)+1) = {-0.940f,  0.148f, 1.0f};   // 2
        *(*(asteroids_coords+0)+2) = {-0.600f,  0.971f, 1.0f};   // 3
        *(*(asteroids_coords+0)+3) = { 0.092f,  0.710f, 1.0f};   // 4
        *(*(asteroids_coords+0)+4) = { 0.432f,  1.000f, 1.0f};   // 5
        *(*(asteroids_coords+0)+5) = { 0.938f,  0.560f, 1.0f};   // 6
        *(*(asteroids_coords+0)+6) = { 0.071f,  0.054f, 1.0f};   // 7
        *(*(asteroids_coords+0)+7) = { 0.945f, -0.162f, 1.0f};   // 8
        *(*(asteroids_coords+0)+8) = { 0.945f, -0.523f, 1.0f};   // 9
        *(*(asteroids_coords+0)+9) = { 0.251f, -1.000f, 1.0f};   // 10
        *(*(asteroids_coords+0)+10) = {-0.514f, -1.000f, 1.0f};  // 11
        *(*(asteroids_coords+0)+11) = {-0.180f, -0.374f, 1.0f};  // 12
        
        *(asteroids_coords+1) = nullptr;
        *(asteroids_coords+2) = nullptr;
        *(asteroids_coords+3) = nullptr;
    }

    Asteroid NewAsteroid(AsteroidType type, int size_level){
        Asteroid new_ast;
        new_ast.id = TList::ListLength(Gameplay::asteroid_ingame) == 0 ? 0 : (Gameplay::asteroid_ingame->info.asteroid_info.id + 1);
        new_ast.type = type;
        new_ast.size_level = size_level;
        new_ast.speed_v = {0.0f, 0.0f, 0.0f};
        PolyLibJMATH::InitPoly(
            &new_ast.figure,
            (int)AsteroidTypeVertices::A1_TV,
            *(asteroids_coords+0),
            {new_ast.size_level*30.0f, new_ast.size_level*30.0f},
            0.0f,
            {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f},
            {255,255,255}
        );
        return new_ast;
    }

    void EmptyMemory(){
        for(int i = 0; i < (int)AsteroidType::TOTAL_ASTEROIDS; i++){
            free(*asteroids_coords+i);
        }
        free(asteroids_coords);
    }
}
