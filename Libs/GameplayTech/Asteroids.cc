#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "./Asteroids.h"

#include "../CustomLibs/PolyLibJMATH.h"

#include "../../Code/Gameplay.h"

namespace Asteroids{
    JMATH::Vec3 **asteroids_coords = nullptr;
    int last_asteroid_id = -1;

    int GetAsteroidTypeVertices(AsteroidType type){
        int res = 0;
        switch (type){
            case A1:
                res = (int)AsteroidTypeVertices::A1_TV;
            break;
            case A2:
                res = (int)AsteroidTypeVertices::A2_TV;
            break;
            case A3:
                res = (int)AsteroidTypeVertices::A3_TV;
            break;
            case A4:
                res = (int)AsteroidTypeVertices::A4_TV;
            break;
        }

        return res;
    }

    void Init(){
        asteroids_coords = (JMATH::Vec3**) malloc(sizeof(JMATH::Vec3*)*AsteroidType::TOTAL_ASTEROIDS);

        // Asteroid 1
        *(asteroids_coords+0) = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3)*AsteroidTypeVertices::A1_TV);
        *(*(asteroids_coords+0)+0) = {-0.311f,  0.684f, 1.0f};   // 1
        *(*(asteroids_coords+0)+1) = {-0.383f,  0.851f, 1.0f};   // 2
        *(*(asteroids_coords+0)+2) = {-0.769f,  0.391f, 1.0f};   // 3
        *(*(asteroids_coords+0)+3) = {-0.510f, -0.005f, 1.0f};   // 4
        *(*(asteroids_coords+0)+4) = {-0.802f, -0.444f, 1.0f};   // 5
        *(*(asteroids_coords+0)+5) = {-0.406f, -0.831f, 1.0f};   // 6
        *(*(asteroids_coords+0)+6) = {-0.071f, -0.637f, 1.0f};   // 7
        *(*(asteroids_coords+0)+7) = { 0.377f, -0.868f, 1.0f};   // 8
        *(*(asteroids_coords+0)+8) = { 0.783f, -0.510f, 1.0f};   // 9
        *(*(asteroids_coords+0)+9) = { 0.363f, -0.335f, 1.0f};   // 10
        *(*(asteroids_coords+0)+10) = { 0.802f,  0.009f, 1.0f};  // 11
        *(*(asteroids_coords+0)+11) = { 0.311f,  0.868f, 1.0f};  // 12
        
        *(asteroids_coords+1) = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3)*AsteroidTypeVertices::A2_TV);
        *(*(asteroids_coords+1)+0) = {-0.063f,  0.019f, 1.0f};   // 1
        *(*(asteroids_coords+1)+1) = {-0.476f, -0.956f, 1.0f};   // 2
        *(*(asteroids_coords+1)+2) = {-0.740f, -0.071f, 1.0f};   // 3
        *(*(asteroids_coords+1)+3) = {-0.317f,  0.127f, 1.0f};   // 4
        *(*(asteroids_coords+1)+4) = {-0.699f,  0.363f, 1.0f};   // 5
        *(*(asteroids_coords+1)+5) = {-0.241f,  0.987f, 1.0f};   // 6
        *(*(asteroids_coords+1)+6) = { 0.441f,  1.000f, 1.0f};   // 7
        *(*(asteroids_coords+1)+7) = { 0.739f,  0.343f, 1.0f};   // 8
        *(*(asteroids_coords+1)+8) = { 0.739f, -0.025f, 1.0f};   // 9
        *(*(asteroids_coords+1)+9) = { 0.249f, -0.999f, 1.0f};   // 10
        *(*(asteroids_coords+1)+10) = {-0.108f, -0.986f, 1.0f};  // 11

        
        *(asteroids_coords+2) = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3)*AsteroidTypeVertices::A3_TV);
        *(*(asteroids_coords+2)+0) = {-0.940f, -0.539f, 1.0f};   // 1
        *(*(asteroids_coords+2)+1) = {-0.927f,  0.588f, 1.0f};   // 2
        *(*(asteroids_coords+2)+2) = {-0.509f,  0.994f, 1.0f};   // 3
        *(*(asteroids_coords+2)+3) = { 0.251f,  0.999f, 1.0f};   // 4
        *(*(asteroids_coords+2)+4) = { 0.927f,  0.588f, 1.0f};   // 5
        *(*(asteroids_coords+2)+5) = { 0.704f, -0.039f, 1.0f};   // 6
        *(*(asteroids_coords+2)+6) = { 0.940f, -0.651f, 1.0f};   // 7
        *(*(asteroids_coords+2)+7) = { 0.556f, -0.995f, 1.0f};   // 8
        *(*(asteroids_coords+2)+8) = { 0.007f, -0.586f, 1.0f};   // 9
        *(*(asteroids_coords+2)+9) = {-0.498f, -0.999f, 1.0f};   // 10

        *(asteroids_coords+3) = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3)*AsteroidTypeVertices::A4_TV);
        *(*(asteroids_coords+3)+0) = {-0.945f, -0.374f, 1.0f};   // 1
        *(*(asteroids_coords+3)+1) = {-0.940f,  0.148f, 1.0f};   // 2
        *(*(asteroids_coords+3)+2) = {-0.600f,  0.971f, 1.0f};   // 3
        *(*(asteroids_coords+3)+3) = { 0.092f,  0.710f, 1.0f};   // 4
        *(*(asteroids_coords+3)+4) = { 0.432f,  1.000f, 1.0f};   // 5
        *(*(asteroids_coords+3)+5) = { 0.938f,  0.560f, 1.0f};   // 6
        *(*(asteroids_coords+3)+6) = { 0.071f,  0.054f, 1.0f};   // 7
        *(*(asteroids_coords+3)+7) = { 0.945f, -0.162f, 1.0f};   // 8
        *(*(asteroids_coords+3)+8) = { 0.945f, -0.523f, 1.0f};   // 9
        *(*(asteroids_coords+3)+9) = { 0.251f, -1.000f, 1.0f};   // 10
        *(*(asteroids_coords+3)+10) = {-0.514f, -1.000f, 1.0f};  // 11
        *(*(asteroids_coords+3)+11) = {-0.180f, -0.374f, 1.0f};  // 12
    }

    Asteroid NewAsteroid(AsteroidType type, int size_level){
        Asteroid new_ast;
        new_ast.id = ++last_asteroid_id;
        new_ast.type = type;
        new_ast.size_level = size_level;
        new_ast.speed_v = {
            ((Utils::GenerateRandomFloat(3)+1.0f) * (Utils::GenerateRandomNumber(2) == 0 ? -1 : 1)), //Random number between -3.0f -> -1.0f and 1.0f -> 3.0f 
            ((Utils::GenerateRandomFloat(3)+1.0f) * (Utils::GenerateRandomNumber(2) == 0 ? -1 : 1)), //Random number between -3.0f -> -1.0f and 1.0f -> 3.0f 
            0.0f
        };

        PolyLibJMATH::InitPoly(
            &new_ast.figure,
            GetAsteroidTypeVertices(type),
            *(asteroids_coords+((int)type)),
            {new_ast.size_level*22.5f, new_ast.size_level*22.5f},
            (float)Utils::GenerateRandomNumber(361),
            {(float)Utils::GenerateRandomNumber(Utils::kWindowWidth), (float)Utils::GenerateRandomNumber(Utils::kWindowHeight)},
            {255,255,255}
        );
        return new_ast;
    }

    void AddAsteroidPoints(int size_level, Players::Player* player){
        switch (size_level){
            case 1:
                player->score+=100;
            break;
            case 2:
                player->score+=50;
            break;
            case 3:
                player->score+=20;
            break;
        }
    }

    void GenerateOnAsteroidDestroy(TList::ListNode **asteroid_list, Asteroids::Asteroid *asteroid){
        TList::ListInfo asteroid_aux_info = {NULL};
        // printf("LIST 3 %p\n",*asteroid_list);

        // printf("\n");
        // TList::PrintList(*asteroid_list);
        if(asteroid->size_level > 1){
            for(int i = 0; i < 2; i++){
                asteroid_aux_info.asteroid_info = 
                    Asteroids::NewAsteroid(
                        (Asteroids::AsteroidType)Utils::GenerateRandomNumber(Asteroids::AsteroidType::TOTAL_ASTEROIDS), 
                        asteroid->size_level-1
                    );
                    
                asteroid_aux_info.asteroid_info.figure.transform.translation = asteroid->figure.transform.translation;
                PolyLibJMATH::UpdatePoly(&(asteroid_aux_info.asteroid_info.figure));
                
                TList::InsertList(asteroid_list, TList::ListType::ASTEROID, asteroid_aux_info);
                
            }
        }
        // printf("\n");
        // TList::PrintList(*asteroid_list);
    }

    void DestroyAsteroid(void **asteroid_list, Asteroids::Asteroid *asteroid, Players::Player *player = nullptr){
        TList::ListNode** asteroid_list_aux = (TList::ListNode**) asteroid_list;
        // printf("LIST 2 %p\n",*asteroid_list_aux);
        TList::ListInfo aux_asteroid_info = {NULL};
        aux_asteroid_info.asteroid_info = *asteroid; 
        // printf("ASTEROID TO DELETE ID %d\n",aux_asteroid_info.asteroid_info.id);

        if(player != nullptr){
            AddAsteroidPoints(asteroid->size_level, player);
        }
        GenerateOnAsteroidDestroy(asteroid_list_aux, asteroid);
        TList::DeleteElement(asteroid_list_aux, aux_asteroid_info);
    }
    
    void EmptyMemory(){
        //Free asteroids coords
        for(int i = 0; i < (int)AsteroidType::TOTAL_ASTEROIDS; i++){
            free(*asteroids_coords+i);
        }
        free(asteroids_coords);
    }
}
