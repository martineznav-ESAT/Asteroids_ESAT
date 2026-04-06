#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "../Libs/CustomLibs/Utils.h"
#include "../Libs/CustomLibs/UILib.h"
#include "../Libs/GameplayTech/GameplayTech.h"
#include "../Libs/CustomLibs/TList.h"

#include "./GameManager.h"
#include "./MainMenu.h"


namespace Gameplay{
    TList::ListNode *asteroid_ingame = TList::CreateList();

    //Whole Gameplay initializer
    void Init(){
        
    }

    //Gameplay LOAD
    void GenerateAsteroidRound(){
        TList::ListInfo aux_asteroid_info = {NULL};

        TList::ClearList(&asteroid_ingame);

        //TO_DO
        aux_asteroid_info.asteroid_info = Asteroids::NewAsteroid(Asteroids::AsteroidType::A1,3);
        TList::InsertList(&asteroid_ingame, TList::ListType::ASTEROID, aux_asteroid_info);
        asteroid_ingame->info.asteroid_info.speed_v = {
            (float)Utils::GenerateRandomNumber(7)-3,
            (float)Utils::GenerateRandomNumber(7)-3,
            0.0f
        };
        
        aux_asteroid_info.asteroid_info = Asteroids::NewAsteroid(Asteroids::AsteroidType::A1,2);
        TList::InsertList(&asteroid_ingame, TList::ListType::ASTEROID, aux_asteroid_info);
        asteroid_ingame->info.asteroid_info.speed_v = {
            (float)Utils::GenerateRandomNumber(7)-3,
            (float)Utils::GenerateRandomNumber(7)-3,
            0.0f
        };

        aux_asteroid_info.asteroid_info = Asteroids::NewAsteroid(Asteroids::AsteroidType::A1,1);
        TList::InsertList(&asteroid_ingame, TList::ListType::ASTEROID, aux_asteroid_info);
        asteroid_ingame->info.asteroid_info.speed_v = {
            (float)Utils::GenerateRandomNumber(7)-3,
            (float)Utils::GenerateRandomNumber(7)-3,
            0.0f
        };
    }

    //Loads the Gameplay
    void Load(PlayedGames::Gamemode gm, UserManager::User* p2){

        GameManager::game_status.actual_game = PlayedGames::LoadBaseGameManagerGame(gm);
        GameManager::game_status.actual_game.p2_user = p2;

        GenerateAsteroidRound();

        // TList::PrintList(asteroid_ingame);

        // switch (gm){
        //     case PlayedGames::Gamemode::SP:

        //     break;
        //     case PlayedGames::Gamemode::MP_ALT :
            
        //     break;
        //     case PlayedGames::Gamemode::MP_VS :
            
        //     break;
        //     case PlayedGames::Gamemode::MP_COOP :
            
        //     break;
        // }
        GameManager::game_status.level = GameManager::Level::GAMEPLAY;
    }

    //Gameplay UPDATE
    void UpdateIngameAsteroids(){
        for(TList::ListNode *p = asteroid_ingame; p!=nullptr; p = p->next){
            //MOVE
            p->info.asteroid_info.figure.transform.translation = JMATH::Vec2Sum(
                p->info.asteroid_info.figure.transform.translation,
                {p->info.asteroid_info.speed_v.x, p->info.asteroid_info.speed_v.y}
            );

            PolyLibJMATH::UpdatePoly(&(p->info.asteroid_info.figure));
        }
    }

    //Whole Gameplay update method
    void Update(){  
        UpdateIngameAsteroids();
    }

    //Gameplay DRAW

    void DrawIngameAsteroids(){
        for(TList::ListNode *p = asteroid_ingame; p!=nullptr; p = p->next){
            PolyLibJMATH::DrawPoly(p->info.asteroid_info.figure,false);
        }
    }

    //Whole Gameplay draw method
    void Draw(){
        DrawIngameAsteroids();
    }

    void EmptyMemory(){
        
    }
}