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
    TList::ListNode *asteroid_particles = TList::CreateList();
    Ufo::UfoShip ufo;
    PolyLibJMATH::Poly p1_life_figure;
    PolyLibJMATH::Poly p2_life_figure;

    int gameover_title_lt = 5000;
    int gameover_title_ltc = gameover_title_lt;

    void GenerateAsteroidRound(){
        TList::ListInfo asteroid_aux_info = {NULL};
        TList::ListInfo particle_aux_info = {NULL};
        int max_asteroids = 0;

        TList::ClearList(&asteroid_ingame);
        TList::ClearList(&asteroid_particles);

        if(GameManager::game_status.level == GameManager::Level::GAMEPLAY){
            switch (GameManager::game_status.actual_game->gamemode){
                case PlayedGames::Gamemode::SP:
                case PlayedGames::Gamemode::MP_COOP:
                case PlayedGames::Gamemode::MP_VS:
                    max_asteroids = 4+((GameManager::game_status.actual_game->p1.round)*2);
                break;

                case PlayedGames::Gamemode::MP_ALT:
                    if(GameManager::game_status.actual_game->is_player1_turn){
                        max_asteroids = 4+((GameManager::game_status.actual_game->p1.round)*2);
                    }else{
                        max_asteroids = 4+((GameManager::game_status.actual_game->p2.round)*2);
                    }
                break;

                break;
            }
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
            for(int i = 0; i < 8; i++){
                particle_aux_info.particle_info = (asteroid_aux_info.asteroid_info.destroy_particles+i);
                TList::InsertList(&asteroid_particles, TList::ListType::PARTICLE, particle_aux_info);
            }
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
        PolyLibJMATH::InitPoly(
            &p2_life_figure,
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

    void LoadGameplayLevel(){
        GameManager::game_status.actual_game->p1.inmunity_ltc = 0;
        GameManager::game_status.actual_game->p2.inmunity_ltc = 0;
        
        GenerateAsteroidRound();
        ufo.type = Ufo::UfoType::NONE;
        ufo.spawn_ltc = 0;
    }

    void AdvanceRound(){
        switch (GameManager::game_status.actual_game->gamemode){
            case PlayedGames::Gamemode::SP:
                GameManager::game_status.actual_game->p1.round++;
            break;

            case PlayedGames::Gamemode::MP_ALT:
                if(GameManager::game_status.actual_game->is_player1_turn){
                    GameManager::game_status.actual_game->p1.round++;
                }else{
                    GameManager::game_status.actual_game->p2.round++;
                }
            break;

            case PlayedGames::Gamemode::MP_COOP:
            case PlayedGames::Gamemode::MP_VS:
                GameManager::game_status.actual_game->p1.round++;
                GameManager::game_status.actual_game->p2.round++;
            break;
        }
        LoadGameplayLevel();
    }

    bool AsteroidPlayerCollisions(Asteroids::Asteroid *asteroid, Players::Player *p){
        bool is_collided = false;
        is_collided = Collisions::CollisionAsteroidPlayerShots(
            &asteroid_ingame,
            &asteroid_particles,
            asteroid,
            p
        );

        if(!is_collided){
            is_collided = Collisions::CollisionAsteroidPlayer(
                &asteroid_ingame,
                &asteroid_particles,
                asteroid,
                p
            );
        }

        return is_collided;
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
                        &asteroid_particles,
                        asteroid_aux,
                        &ufo
                    );
                    if(!is_collided){
                        is_collided = Collisions::CollisionAsteroidUfo(
                            &asteroid_ingame,
                            &asteroid_particles,
                            asteroid_aux,
                            &ufo
                        );
                    }

                    
                    //Players Collisions
                    switch (GameManager::game_status.actual_game->gamemode){
                        case PlayedGames::Gamemode::SP:
                            //Player 1 Collisions
                            if(!is_collided){
                                is_collided = AsteroidPlayerCollisions(asteroid_aux, &(GameManager::game_status.actual_game->p1));
                            }
                        break;

                        case PlayedGames::Gamemode::MP_ALT:
                            if(GameManager::game_status.actual_game->is_player1_turn){
                                //Player 1 Collisions
                                if(!is_collided){
                                    is_collided = AsteroidPlayerCollisions(asteroid_aux, &(GameManager::game_status.actual_game->p1));
                                } 
                            }else{
                                //Player 2 Collisions
                                if(!is_collided){
                                    is_collided = AsteroidPlayerCollisions(asteroid_aux, &(GameManager::game_status.actual_game->p2));
                                }
                            }
                            
                        break;

                        case PlayedGames::Gamemode::MP_COOP:
                        case PlayedGames::Gamemode::MP_VS:
                            //Player 1 Collisions
                            if(!is_collided){
                                is_collided = AsteroidPlayerCollisions(asteroid_aux, &(GameManager::game_status.actual_game->p1));
                            }
                            //Player 2 Collisions
                            if(!is_collided){
                                is_collided = AsteroidPlayerCollisions(asteroid_aux, &(GameManager::game_status.actual_game->p2));
                            }
                        break;
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
    

    void UpdateGameAsteroidsParticles(){
        for(TList::ListNode *p = asteroid_particles; p!=nullptr; p = p->next){
            Particles::UpdateParticle(p->info.particle_info);
        }
    }
    

    void UpdatePlayers(){
        switch (GameManager::game_status.actual_game->gamemode){
            case PlayedGames::Gamemode::SP:
                Players::UpdatePlayer(&(GameManager::game_status.actual_game->p1), true);
            break;

            case PlayedGames::Gamemode::MP_ALT:
                if(GameManager::game_status.actual_game->is_player1_turn){
                    Players::UpdatePlayer(&(GameManager::game_status.actual_game->p1), true);
                }else{
                    Players::UpdatePlayer(&(GameManager::game_status.actual_game->p2), false);
                }
                
            break;

            case PlayedGames::Gamemode::MP_COOP:
            case PlayedGames::Gamemode::MP_VS:
                    Players::UpdatePlayer(&(GameManager::game_status.actual_game->p1), true);
                    Players::UpdatePlayer(&(GameManager::game_status.actual_game->p2), false);
            break;
        }
    }

    void GameOver(){
        TList::ListInfo aux_info = {NULL};
        GameManager::game_status.actual_game->is_finished = true;

        gameover_title_ltc = 0;
        TList::SaveList(((TList::ListNode**)(&(PlayedGames::game_list))), PlayedGames::game_list_dat, PlayedGames::game_list_dat_path);

        aux_info.game_info = *(GameManager::game_status.actual_game);
        if(HighscoresMenu::AddHighScoreGame(aux_info)){
            // printf("SaveList HIGHSCORES ON GAME OVER\n");
            TList::SaveList(((TList::ListNode**)(&(HighscoresMenu::top_games))), HighscoresMenu::highscores_dat, HighscoresMenu::highscores_dat_path);
            // printf("HIGHSCORES LIST PROPERLY SAVED ON GAME OVER\n");
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

    void UpdateGameOverScreen(){
        gameover_title_ltc += 1000/Utils::kFPS;
        if(gameover_title_ltc >= gameover_title_lt){
            Ufo::DestroyUfo(&ufo);
            MainMenu::Load();
        }
    }

    //Whole Gameplay update method
    void Update(){

        //GameOver Management
        if(GameManager::game_status.level == GameManager::Level::GAMEPLAY){
            UpdatePlayers();
    
            if(GameManager::game_status.actual_game->is_finished){
                UpdateGameOverScreen();
            }else{
                CheckGameOver();
            }
        }
        
        Ufo::UpdateUfo(&ufo);
        UpdateGameAsteroids();
        UpdateGameAsteroidsParticles();
    }


    //Gameplay LOAD
    

    //Loads the Gameplay
    void Load(PlayedGames::Gamemode gm, UserManager::User* p2 = nullptr){
        TList::ListInfo aux_game_info = {NULL};
        PlayedGames::PlayedGame *aux_actual_game = nullptr;
        GameManager::game_status.level = GameManager::Level::GAMEPLAY;

        //CREATE NEW GAME
        aux_game_info.game_info = PlayedGames::LoadBaseGameManagerGame(gm, p2);

        TList::InsertList(((TList::ListNode**)(&(PlayedGames::game_list))), TList::ListType::PLAYED_GAME, aux_game_info);
        TList::SaveList(((TList::ListNode**)(&(PlayedGames::game_list))), PlayedGames::game_list_dat, PlayedGames::game_list_dat_path);

        //LOADS NEW GAME AS THE ACTUAL GAME
        aux_actual_game = &(TList::FindInList((TList::ListNode*)PlayedGames::game_list, aux_game_info)->info.game_info);
        GameManager::game_status.actual_game = aux_actual_game;

        //Player Colors
        if(aux_actual_game->gamemode == PlayedGames::Gamemode::SP){
            p1_life_figure.color = aux_actual_game->p1.ship.figure.color;
        }else{
            aux_actual_game->p1.ship.figure.color = {150,150,255};
            aux_actual_game->p2.ship.figure.color = {255,0,0};

            for (int i = 0; i < Players::max_player_shots; i++){
                (aux_actual_game->p1.ship.shots+i)->bullet.color = aux_actual_game->p1.ship.figure.color;
                (aux_actual_game->p2.ship.shots+i)->bullet.color = aux_actual_game->p2.ship.figure.color;
                if(i < 4){
                    (aux_actual_game->p1.ship.death_particles+i)->figure.color = aux_actual_game->p1.ship.figure.color;
                    (aux_actual_game->p2.ship.death_particles+i)->figure.color = aux_actual_game->p2.ship.figure.color;
                }
            }

            p1_life_figure.color = aux_actual_game->p1.ship.figure.color;
            p2_life_figure.color = aux_actual_game->p2.ship.figure.color;
        }

        LoadGameplayLevel();
    }

    //Gameplay DRAW
    void DrawGameAsteroids(){
        for(TList::ListNode *p = asteroid_ingame; p!=nullptr; p = p->next){
            PolyLibJMATH::DrawPoly(p->info.asteroid_info.figure,false);
        }
    }

    void DrawGameAsteroidsParticles(){
        for(TList::ListNode *p = asteroid_particles; p!=nullptr; p = p->next){
            Particles::DrawParticle(p->info.particle_info);
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

            case PlayedGames::Gamemode::MP_ALT:
                if (actual_game.is_player1_turn){
                    Players::DrawPlayer(actual_game.p1);
                }else{
                    Players::DrawPlayer(actual_game.p2);
                }
                
            break;
        
            case PlayedGames::Gamemode::MP_VS:
            case PlayedGames::Gamemode::MP_COOP:
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

            case PlayedGames::Gamemode::MP_ALT:
                if (actual_game.is_player1_turn){
                    DrawP1UI(actual_game);
                    DrawP1Lifes(actual_game);
                }else{
                    DrawP2UI(actual_game);
                    DrawP2Lifes(actual_game);
                }
                
            break;
        
            case PlayedGames::Gamemode::MP_VS:
            case PlayedGames::Gamemode::MP_COOP:
                DrawP1UI(actual_game);
                DrawP1Lifes(actual_game);

                DrawP2UI(actual_game);
                DrawP2Lifes(actual_game);
            break;
        }
    }

    void DrawGameOverScreen(){
        UILib::Text game_over_txt = {
            {255,255,255,255},
            "GAME OVER",
            Utils::kBaseFontSize*3.0f
        };
        
        UILib::DrawText(
            {
                Utils::kWindowWidth*0.5f - (game_over_txt.font_size*2.5f), 
                Utils::kWindowHeight*0.5f - (game_over_txt.font_size*0.5f)
            }, 
            game_over_txt
        );
    }

    //Whole Gameplay draw method
    void Draw(){
        DrawGameAsteroids();
        DrawGameAsteroidsParticles();
        Ufo::DrawUfo(ufo);
        if(GameManager::game_status.level == GameManager::Level::GAMEPLAY){
            DrawPlayers(*(GameManager::game_status.actual_game));
            DrawGameUI(*(GameManager::game_status.actual_game));
            if(gameover_title_ltc < gameover_title_lt){
                if((gameover_title_ltc % 1000) < 750){
                    DrawGameOverScreen();
                }
            }
        }
    }

    void EmptyMemory(){
        Ufo::EmptyUfoMemory(&ufo);
        TList::ClearList(&asteroid_ingame);
        TList::ClearList(&asteroid_particles);
        PolyLibJMATH::EmptyPolyMemory(&p1_life_figure);
        PolyLibJMATH::EmptyPolyMemory(&p2_life_figure);
    }
}