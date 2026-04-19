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
#include "./HighscoresMenu.h"


namespace Gameplay{
    TList::ListNode *asteroid_ingame = TList::CreateList();
    Ufo::UfoShip ufo;
    PolyLibJMATH::Poly p1_life_figure;
    PolyLibJMATH::Poly p2_life_figure;

    int gameover_title_lt = 5000;
    int gameover_title_ltc = gameover_title_lt;

    void GenerateAsteroidRound(){
        TList::ListInfo asteroid_aux_info = {NULL};
        int max_asteroids = 0;

        TList::ClearList(&asteroid_ingame);

        if(GameManager::game_status.level == GameManager::Level::GAMEPLAY){
            max_asteroids = 4+((GameManager::game_status.actual_game->round-1)*2);
            max_asteroids = max_asteroids > 12 ? 12 : max_asteroids;
        }else{
            max_asteroids = 12;
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
        ufo = Ufo::NewUfo();
        PolyLibJMATH::InitPoly(
            &p1_life_figure,
            5,
            Players::ship_coords,
            {15.0f,15.0f},
            -90.0f,
            {0.0f, 0.0f},
            {255,255,255},
            {0.1f,0.0f}
        );
    }

    //Gameplay UPDATE
    void AdvanceRound(){
        GameManager::game_status.actual_game->round++;
        GenerateAsteroidRound();
    }


    void UpdateGameAsteroids(){
        Asteroids::Asteroid *asteroid_aux;
        bool is_collided;

        if(TList::IsEmptyList(&asteroid_ingame)){
            AdvanceRound();
        }else{
            for(TList::ListNode *p = asteroid_ingame; p!=nullptr; p = p->next){
                //Check All Asteroids collisions
                asteroid_aux = &(p->info.asteroid_info);
                is_collided = false;
                
                if(GameManager::game_status.level == GameManager::Level::GAMEPLAY){
                    

                    //Ufo Collisions
                    is_collided = Collisions::CollisionAsteroidUfoShot(
                        &asteroid_ingame,
                        asteroid_aux,
                        &ufo
                    );
                    if(!is_collided){
                        is_collided = Collisions::CollisionAsteroidUfo(
                            &asteroid_ingame,
                            asteroid_aux,
                            &ufo
                        );
                    }

                    //Player 1 Collisions
                    if(!is_collided){
                        is_collided = Collisions::CollisionAsteroidPlayerShots(
                            &asteroid_ingame,
                            asteroid_aux,
                            &(GameManager::game_status.actual_game->p1)
                        );
                    }

                    if(!is_collided){
                        is_collided = Collisions::CollisionAsteroidPlayer(
                            &asteroid_ingame,
                            asteroid_aux,
                            &(GameManager::game_status.actual_game->p1)
                        );
                    }

                    if(GameManager::game_status.actual_game->gamemode != PlayedGames::Gamemode::SP){
                        //Player 2 Collisions
                        if(!is_collided){
                            is_collided = Collisions::CollisionAsteroidPlayerShots(
                                &asteroid_ingame,
                                asteroid_aux,
                                &(GameManager::game_status.actual_game->p2)
                            );
                        }
                    }
                }

                if(!is_collided){
                    PolyLibJMATH::MovePoly(&(p->info.asteroid_info.figure), p->info.asteroid_info.speed_v);
                    Collisions::BorderExitRellocation(&(p->info.asteroid_info.figure));
                    PolyLibJMATH::UpdatePoly(&(p->info.asteroid_info.figure));
                }
            }
        }
    }
    
    void UpdatePlayers(){
        Players::UpdatePlayer(&(GameManager::game_status.actual_game->p1), true);
        if(GameManager::game_status.actual_game->gamemode != PlayedGames::Gamemode::SP){
            Players::UpdatePlayer(&(GameManager::game_status.actual_game->p2), false);
        }
    }

    void GameOver(){
        TList::ListInfo aux_info = {NULL};
        GameManager::game_status.actual_game->is_finished = true;

        gameover_title_ltc = 0;
        TList::SaveList(((TList::ListNode**)(&(PlayedGames::game_list))), PlayedGames::game_list_dat, PlayedGames::game_list_dat_path);

        aux_info.game_info = *(GameManager::game_status.actual_game);
        if(HighscoresMenu::AddHighScoreGame(aux_info)){
            TList::SaveList(((TList::ListNode**)(&(HighscoresMenu::top_games))), HighscoresMenu::highscores_dat, HighscoresMenu::highscores_dat_path);
        }
    }

    void CheckGameOver(){
        if(GameManager::game_status.actual_game->gamemode == PlayedGames::Gamemode::SP){
            if(GameManager::game_status.actual_game->p1.lifes <= 0){
                GameOver();
            }
        }else{
            if(GameManager::game_status.actual_game->p1.lifes <= 0 && GameManager::game_status.actual_game->p2.lifes <= 0){
                GameOver();
            }
        }
    }

    //Whole Gameplay update method
    void Update(){

        //GameOver Management
        if(GameManager::game_status.level == GameManager::Level::GAMEPLAY){
            if(GameManager::game_status.actual_game->is_finished){
                gameover_title_ltc += 1000/Utils::kFPS;
                if(gameover_title_ltc >= gameover_title_lt){
                    ufo.type = Ufo::UfoType::NONE;
                    MainMenu::Load();
                }
            }else{
                UpdatePlayers();
                CheckGameOver();
            }
        }
        

        UpdateGameAsteroids();

        //DEBUG
        if(esat::IsKeyDown('U')){
            Ufo::SpawnUfo(&ufo);
        }
        Ufo::UpdateUfo(&ufo);
    }


    //Gameplay LOAD
    void LoadGameplayLevel(){
        GameManager::game_status.actual_game->p1.inmunity_ltc = 0;
        GameManager::game_status.actual_game->p2.inmunity_ltc = 0;
        
        GenerateAsteroidRound();
        ufo.type = Ufo::UfoType::NONE;

        //UPDATES VALUES BEFORE ITS REFLECTED ON SCREEN
        Update();
    }

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

        p1_life_figure.color = GameManager::game_status.actual_game->p2.ship.figure.color;

        if(p2 != nullptr){
            p2_life_figure.color = GameManager::game_status.actual_game->p2.ship.figure.color;
        }

        LoadGameplayLevel();
    }

    //Gameplay DRAW
    void DrawGameAsteroids(){
        for(TList::ListNode *p = asteroid_ingame; p!=nullptr; p = p->next){
            PolyLibJMATH::DrawPoly(p->info.asteroid_info.figure,false);
        }
    }

    void DrawP1Lifes(PlayedGames::PlayedGame actual_game){
        float base_height = Utils::kBaseFontSize*2.0f + p1_life_figure.transform.scale.x+10.0f;
        float base_width =  p1_life_figure.transform.scale.x+10.0f;

        for(int i = 1; i <= actual_game.p1.lifes-1; i++){
            p1_life_figure.transform.translation = {base_width*i, base_height};
            PolyLibJMATH::UpdatePoly(&p1_life_figure);
            PolyLibJMATH::DrawPoly(p1_life_figure, false);
        }
    }

    void DrawP2Lifes(PlayedGames::PlayedGame actual_game){
        float base_height = Utils::kBaseFontSize*2.0f + p2_life_figure.transform.scale.x+10.0f;
        float base_width = p2_life_figure.transform.scale.x+10.0f;

        for(int i = 1; i <= actual_game.p2.lifes-1; i++){
            p2_life_figure.transform.translation = {Utils::kWindowWidth - (base_width*i), base_height};
            PolyLibJMATH::UpdatePoly(&p2_life_figure);
            PolyLibJMATH::DrawPoly(p2_life_figure, false);
        }
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

    

    void DrawPlayers(PlayedGames::PlayedGame actual_game){

        switch (actual_game.gamemode){
            case PlayedGames::Gamemode::SP:
                Players::DrawPlayer(actual_game.p1);
            break;
        
            default:
                Players::DrawPlayer(actual_game.p1);
                Players::DrawPlayer(actual_game.p2);
            break;
        }
    }

    void DrawGameUI(PlayedGames::PlayedGame actual_game){
        switch (actual_game.gamemode){
            case PlayedGames::Gamemode::SP:
                DrawP1UI(actual_game);
                DrawP1Lifes(actual_game);
            break;
        
            default:
                DrawP1UI(actual_game);
                DrawP1Lifes(actual_game);

                DrawP2UI(actual_game);
                DrawP2Lifes(actual_game);
            break;
        }
    }

    //Whole Gameplay draw method
    void Draw(){
        DrawGameAsteroids();
        Ufo::DrawUfo(ufo);
        if(GameManager::game_status.level == GameManager::Level::GAMEPLAY){
            DrawPlayers(*(GameManager::game_status.actual_game));
            DrawGameUI(*(GameManager::game_status.actual_game));
        }
    }

    void EmptyMemory(){
        Ufo::EmptyUfoMemory(&ufo);
        TList::ClearList(&asteroid_ingame);
        PolyLibJMATH::EmptyPolyMemory(&p1_life_figure);
        PolyLibJMATH::EmptyPolyMemory(&p2_life_figure);
    }
}