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

    void GenerateAsteroidRound(){
        TList::ListInfo asteroid_aux_info = {NULL};
        int max_asteroids = 0;

        TList::ClearList(&asteroid_ingame);

        if(GameManager::game_status.level == GameManager::Level::GAMEPLAY){
            switch (GameManager::game_status.actual_game->round){
                case 1:
                    max_asteroids = 4;
                break;
                
                case 2:
                    max_asteroids = 6;
                break;
                
                case 3:
                    max_asteroids = 8;
                break;
            
                default:
                    max_asteroids = 11;
                break;
            }
        }else{
            max_asteroids = 11;
        }

        for (int i = 0; i < max_asteroids; i++){
            asteroid_aux_info.asteroid_info = 
                Asteroids::NewAsteroid(
                    (Asteroids::AsteroidType)Utils::GenerateRandomNumber(Asteroids::AsteroidType::TOTAL_ASTEROIDS), 
                    GameManager::game_status.level == GameManager::Level::GAMEPLAY ? 3 : Utils::GenerateRandomNumber(3)+1
                );
                
            PolyLibJMATH::UpdatePoly(&(asteroid_aux_info.asteroid_info.figure));
            TList::InsertList(&asteroid_ingame, TList::ListType::ASTEROID, asteroid_aux_info);
        }
}


    //Whole Gameplay initializer
    void Init(){
        // printf("INIT GAMEPLAY\n");
        GenerateAsteroidRound();
    }

    void GenerateOnAsteroidDestroy(Asteroids::Asteroid asteroid){
        TList::ListInfo asteroid_aux_info = {NULL};

        if(asteroid.size_level > 1){
            for(int i = 0; i < 2; i++){
                asteroid_aux_info.asteroid_info = 
                    Asteroids::NewAsteroid(
                        (Asteroids::AsteroidType)Utils::GenerateRandomNumber(Asteroids::AsteroidType::TOTAL_ASTEROIDS), 
                        asteroid.size_level-1
                    );
                    
                asteroid_aux_info.asteroid_info.figure.transform.translation = asteroid.figure.transform.translation;
                PolyLibJMATH::UpdatePoly(&(asteroid_aux_info.asteroid_info.figure));
                TList::InsertList(&asteroid_ingame, TList::ListType::ASTEROID, asteroid_aux_info);
            }
        }
    }

    //Gameplay UPDATE
    void AdvanceRound(){
        GameManager::game_status.actual_game->round++;
        GenerateAsteroidRound();
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

    void UpdateGameAsteroids(){
        Asteroids::Asteroid *asteroid_aux;

        if(TList::IsEmptyList(&asteroid_ingame)){
            AdvanceRound();
        }else{
            for(TList::ListNode *p = asteroid_ingame; p!=nullptr; p = p->next){
                asteroid_aux = &(p->info.asteroid_info);
                //TO_DO REPLACE WITH CollisionPolyPlayerShots
                if(GameManager::game_status.level == GameManager::Level::GAMEPLAY && 
                    Collisions::CollisionPolyOnRClick(asteroid_aux->figure)){
                    //TO_DO NOT HARDCODED TO PLAYER ONE
                    AddAsteroidPoints(asteroid_aux->size_level, &(GameManager::game_status.actual_game->p1));

                    GenerateOnAsteroidDestroy(*asteroid_aux);
                    TList::DeleteElement(&asteroid_ingame, p->info);
                }else{
                    PolyLibJMATH::MovePoly(&(p->info.asteroid_info.figure), p->info.asteroid_info.speed_v);
                    PolyLibJMATH::UpdatePoly(&(p->info.asteroid_info.figure));
                }
            }
        }
    }

    //TO_DO PLAYER CONTROL WIP 
    void UpdatePlayerShots(Players::Player* player){
        for(int i = 0; i < Players::max_player_shots; i++){
            if(((player->ship.shots)+i)->is_active){
                PolyLibJMATH::MovePoly(&(((player->ship.shots)+i)->bullet), (((player->ship.shots)+i)->speed_v));
                PolyLibJMATH::UpdatePoly(&(((player->ship.shots)+i)->bullet));
            }
        }
    }

    void UpdatePlayer(Players::Player* player){
        PolyLibJMATH::MovePoly(&(player->ship.figure), player->ship.speed_v);
        PolyLibJMATH::UpdatePoly(&(player->ship.figure));
        UpdatePlayerShots(player);
    }

    void UpdatePlayers(){
        UpdatePlayer(&(GameManager::game_status.actual_game->p1));
        if(GameManager::game_status.actual_game->gamemode != PlayedGames::Gamemode::SP){
            UpdatePlayer(&(GameManager::game_status.actual_game->p2));
        }
    }

    //Whole Gameplay update method
    void Update(){  
        UpdatePlayers();
        UpdateGameAsteroids();
    }


    //Gameplay LOAD

    //Loads the Gameplay
    void Load(PlayedGames::Gamemode gm, UserManager::User* p2){
        TList::ListInfo aux_game_info = {NULL};
        GameManager::game_status.level = GameManager::Level::GAMEPLAY;

        //CREATE NEW GAME
        aux_game_info.game_info = PlayedGames::LoadBaseGameManagerGame(gm);
        aux_game_info.game_info.p2_user = p2;

        TList::InsertList(((TList::ListNode**)(&(PlayedGames::game_list))), TList::ListType::PLAYED_GAME, aux_game_info);
        TList::SaveList(((TList::ListNode**)(&(PlayedGames::game_list))), PlayedGames::game_list_dat, PlayedGames::game_list_dat_path);

        //LOADS NEW GAME AS THE ACTUAL GAME
        GameManager::game_status.actual_game = &(TList::FindInList((TList::ListNode*)PlayedGames::game_list, aux_game_info)->info.game_info);

        //UPDATES VALUES BEFORE ITS REFLECTED ON SCREEN
        GenerateAsteroidRound();
        Update();
    }

    //Gameplay DRAW
    void DrawGameAsteroids(){
        for(TList::ListNode *p = asteroid_ingame; p!=nullptr; p = p->next){
            PolyLibJMATH::DrawPoly(p->info.asteroid_info.figure,false);
        }
    }

    void DrawLifes(PlayedGames::PlayedGame actual_game){

    }

    void DrawP1UI(PlayedGames::PlayedGame actual_game){
        UILib::DrawText(
            {20.0f,Utils::kBaseFontSize*2.0f},
            {
                {255,255,255,255},
                actual_game.p1_user->alias,
                Utils::kBaseFontSize*2.0f
            }
        );
        UILib::DrawIntToText(
            {20.0f + (Utils::kBaseFontSize*2.0f)*3,Utils::kBaseFontSize*2.0f},
            {
                {255,255,255,255},
                actual_game.p1_user->alias,
                Utils::kBaseFontSize*2.0f
            },
            actual_game.p1.score,6,true
        );
    }

    void DrawP2UI(PlayedGames::PlayedGame actual_game){
        UILib::DrawText(
            {Utils::kWindowWidth - ((Utils::kBaseFontSize*2.0f)*2.5f),Utils::kBaseFontSize*2.0f},
            {
                {255,255,255,255},
                actual_game.p2_user->alias,
                Utils::kBaseFontSize*2.0f
            }
        );
        UILib::DrawIntToText(
            {Utils::kWindowWidth - ((Utils::kBaseFontSize*2.0f)*7.5f),Utils::kBaseFontSize*2.0f},
            {
                {255,255,255,255},
                actual_game.p2_user->alias,
                Utils::kBaseFontSize*2.0f
            },
            actual_game.p2.score,6,true
        );
    }

    void DrawPlayerShots(Players::Player player){
        for(int i = 0; i < Players::max_player_shots; i++){
            if(((player.ship.shots)+i)->is_active){
                PolyLibJMATH::DrawPoly(((player.ship.shots)+i)->bullet,true);
            }
        }
    }

    void DrawPlayers(PlayedGames::PlayedGame actual_game){
        PolyLibJMATH::DrawPoly(actual_game.p1.ship.figure,false);
        DrawPlayerShots(actual_game.p1);

        if(actual_game.gamemode != PlayedGames::Gamemode::SP){
            PolyLibJMATH::DrawPoly(actual_game.p2.ship.figure,false);
            DrawPlayerShots(actual_game.p2);
        }
    }

    void DrawGameUI(PlayedGames::PlayedGame actual_game){
        DrawP1UI(actual_game);
        if(actual_game.gamemode != PlayedGames::Gamemode::SP){
            DrawP2UI(actual_game);
        }
    }

    //Whole Gameplay draw method
    void Draw(){
        DrawGameAsteroids();
        DrawPlayers(*(GameManager::game_status.actual_game));
        DrawGameUI(*(GameManager::game_status.actual_game));
    }

    void EmptyMemory(){
        
    }
}