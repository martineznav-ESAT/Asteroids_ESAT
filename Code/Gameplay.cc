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
#include "../Libs/CustomLibs/TList.h"
#include "../Libs/CustomLibs/AudioLib.h"

#include "../Libs/GameplayTech/GameplayTech.h"

#include "./GameManager.h"
#include "./MainMenu.h"
#include "./HighscoresMenu.h"


namespace Gameplay{
    TList::ListNode *asteroid_ingame = TList::CreateList();
    TList::ListNode *asteroid_particles = TList::CreateList();
    TList::ListNode *spawned_power_ups = TList::CreateList();
    Ufo::UfoShip ufo;
    PolyLibJMATH::Poly p1_life_figure;
    PolyLibJMATH::Poly p2_life_figure;

    int gameover_title_lt = 5000;
    int gameover_title_ltc = gameover_title_lt;
    int beat_lt = 1000;
    int beat_ltc = beat_lt;
    bool is_low_beat = true;
    bool new_highscore_p1 = false;
    bool new_highscore_p2 = false;

    //Cleans the gameplay lists and generates new starting round asteroids based on the actual player round and gamemode
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

        // TList::PrintList(asteroid_ingame);
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

    //Respawns the given player. Mainly used when the game for the same player is starting
    void LoadPlayerOnLoadGameplayLevelRespawn(Players::Player *p){
        p->dead_ltc = p->dead_lt;
        for(int i = 0; i < Players::max_player_shots; i++){
            ((p->ship.shots)+i)->lt_count = ((p->ship.shots)+i)->life_time;
            ((p->ship.shots)+i)->is_active = false;
        }
        for(int i = 0; i < 4; i++){
            (p->ship.death_particles+i)->lt_count = (p->ship.death_particles+i)->life_time;
            (p->ship.death_particles+i)->is_active = false;
        }

        Players::RespawnPlayer(p);
    }

    //Loads the actual corresponding level of the game.
    //The respawn parameters lets you chose if you want the players to be respawned or not
    void LoadGameplayLevel(bool respawn){
        GameManager::game_status.actual_game->p1.inmunity_ltc = 0;
        GameManager::game_status.actual_game->p2.inmunity_ltc = 0;
        
        GenerateAsteroidRound();
        ufo.type = Ufo::UfoType::NONE;
        ufo.spawn_ltc = 0;
        ufo.shot.lt_count = ufo.shot.lt_count;
        ufo.shot.is_active = false;
        for(int i = 0; i < 8; i++){
            (ufo.death_particles+i)->lt_count = (ufo.death_particles+i)->lt_count;
            (ufo.death_particles+i)->is_active = false;
        }

        if(respawn){
            LoadPlayerOnLoadGameplayLevelRespawn(&(GameManager::game_status.actual_game->p1));
            LoadPlayerOnLoadGameplayLevelRespawn(&(GameManager::game_status.actual_game->p2));
        }


        switch(GameManager::game_status.actual_game->gamemode){
            case PlayedGames::Gamemode::MP_COOP:
                if(GameManager::game_status.actual_game->p1.lifes <=0 ){
                    GameManager::game_status.actual_game->p1.lifes++;
                    GameManager::game_status.actual_game->p1.dead_ltc = GameManager::game_status.actual_game->p1.dead_lt;
                }

                if(GameManager::game_status.actual_game->p2.lifes <=0 ){
                    GameManager::game_status.actual_game->p2.lifes++;
                    GameManager::game_status.actual_game->p2.dead_ltc = GameManager::game_status.actual_game->p2.dead_lt;
                }
            break;
        }
    }

    //Increases the player/s round and loads it
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
        LoadGameplayLevel(false);
    }

    //Returns if there is a collision between the given asteroid and anything related to a player
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

    //Updates the current state of the active asteroids in the game
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
    

    //Updates the current state of the active asteroid death particles in the game
    void UpdateGameAsteroidsParticles(){
        for(TList::ListNode *p = asteroid_particles; p!=nullptr; p = p->next){
            Particles::UpdateParticle(p->info.particle_info);
        }
    }
    

    //Updates the current state of the players in the game
    void UpdatePlayers(){
        switch (GameManager::game_status.actual_game->gamemode){
            case PlayedGames::Gamemode::SP:
                Players::UpdatePlayer(&(GameManager::game_status.actual_game->p1));
            break;

            case PlayedGames::Gamemode::MP_ALT:
                if(GameManager::game_status.actual_game->is_player1_turn){
                    Players::UpdatePlayer(&(GameManager::game_status.actual_game->p1));
                }else{
                    Players::UpdatePlayer(&(GameManager::game_status.actual_game->p2));
                }
                
            break;

            case PlayedGames::Gamemode::MP_COOP:
            case PlayedGames::Gamemode::MP_VS:
                    Players::UpdatePlayer(&(GameManager::game_status.actual_game->p1));
                    Players::UpdatePlayer(&(GameManager::game_status.actual_game->p2));
            break;
        }
    }

    //Executes the logic of the game ending, its management and starts the counter for the GameOver title to appear
    void GameOver(){
        TList::ListInfo aux_info = {NULL};
        GameManager::game_status.actual_game->is_finished = true;

        gameover_title_ltc = 0;

        // printf("GAMEOVER SWITCH START\n");
        switch (GameManager::game_status.actual_game->gamemode){
            case PlayedGames::Gamemode::SP:
                GameManager::game_status.actual_game->compare_score = GameManager::game_status.actual_game->p1.score;

                TList::SaveList(((TList::ListNode**)(&(PlayedGames::game_list))), PlayedGames::game_list_dat, PlayedGames::game_list_dat_path);

                aux_info.game_info = *(GameManager::game_status.actual_game);
                if((new_highscore_p1 = HighscoresMenu::AddHighScoreGame(aux_info))){
                    // printf("SaveList HIGHSCORES ON GAME OVER\n");
                    UserManager::AddCredits(aux_info.game_info.p1_user, 5);
                    TList::SaveList(((TList::ListNode**)(&(HighscoresMenu::top_games))), HighscoresMenu::highscores_dat, HighscoresMenu::highscores_dat_path);
                    // printf("HIGHSCORES LIST PROPERLY SAVED ON GAME OVER\n");
                }
            break;

            case PlayedGames::Gamemode::MP_ALT:
            case PlayedGames::Gamemode::MP_VS:
                //Player 1 (Actual Game) Compare Score save 
                GameManager::game_status.actual_game->compare_score = GameManager::game_status.actual_game->p1.score;

                //CREATES A COPY GAME WITH PLAYER 2 SCORE FOR HIGHSCORE COMPARISON REASONS 
                //IF THE SCORE IS DIFFERENT TO PLAYER 1
                if(GameManager::game_status.actual_game->p1.score != GameManager::game_status.actual_game->p2.score){
                    aux_info.game_info = PlayedGames::NewGameCopy(GameManager::game_status.actual_game);
                    aux_info.game_info.compare_score = aux_info.game_info.p2.score;

                    TList::InsertList((TList::ListNode**) &(PlayedGames::game_list), TList::PLAYED_GAME, aux_info);
                }

                TList::SaveList(((TList::ListNode**)(&(PlayedGames::game_list))), PlayedGames::game_list_dat, PlayedGames::game_list_dat_path);

                //IF THE PLAYER 2 SCORE IS DIFFERENT TO PLAYER 1 CHECKS HIGHSCORE
                if(GameManager::game_status.actual_game->p1.score != GameManager::game_status.actual_game->p2.score){
                    //Player 2 Score Highscore Check
                    if((new_highscore_p2 = HighscoresMenu::AddHighScoreGame(aux_info))){
                        UserManager::AddCredits(aux_info.game_info.p2_user, 5);
                        // printf("SaveList HIGHSCORES ON GAME OVER\n");
                        TList::SaveList(((TList::ListNode**)(&(HighscoresMenu::top_games))), HighscoresMenu::highscores_dat, HighscoresMenu::highscores_dat_path);
                        // printf("HIGHSCORES LIST PROPERLY SAVED ON GAME OVER\n");
                    }
                }

                //Player 1 (Actual Game) Score Highscore Check
                aux_info.game_info = *(GameManager::game_status.actual_game);
                if((new_highscore_p1 = HighscoresMenu::AddHighScoreGame(aux_info))){
                    UserManager::AddCredits(aux_info.game_info.p1_user, 5);
                    
                    //Add credits to player 2 too if is the same score
                    if(GameManager::game_status.actual_game->p1.score == GameManager::game_status.actual_game->p2.score){
                        UserManager::AddCredits(aux_info.game_info.p2_user, 5);
                    }
                    // printf("SaveList HIGHSCORES ON GAME OVER\n");
                    TList::SaveList(((TList::ListNode**)(&(HighscoresMenu::top_games))), HighscoresMenu::highscores_dat, HighscoresMenu::highscores_dat_path);
                    // printf("HIGHSCORES LIST PROPERLY SAVED ON GAME OVER\n");
                }
            break;
        
            case PlayedGames::Gamemode::MP_COOP:
                //Player 1+2 (Actual Game) Compare Score save

                GameManager::game_status.actual_game->compare_score = 
                    GameManager::game_status.actual_game->p1.score + 
                    GameManager::game_status.actual_game->p2.score
                ;

                TList::SaveList(((TList::ListNode**)(&(PlayedGames::game_list))), PlayedGames::game_list_dat, PlayedGames::game_list_dat_path);

                aux_info.game_info = *(GameManager::game_status.actual_game);
                if((new_highscore_p1 = HighscoresMenu::AddHighScoreGame(aux_info))){
                    UserManager::AddCredits(aux_info.game_info.p1_user, 5);
                    UserManager::AddCredits(aux_info.game_info.p2_user, 5);
                    // printf("SaveList HIGHSCORES ON GAME OVER\n");
                    TList::SaveList(((TList::ListNode**)(&(HighscoresMenu::top_games))), HighscoresMenu::highscores_dat, HighscoresMenu::highscores_dat_path);
                    // printf("HIGHSCORES LIST PROPERLY SAVED ON GAME OVER\n");
                }
            break;
        }

        //Saves user credit change in case of highscore
        if(new_highscore_p1 || new_highscore_p2){
            TList::SaveList(((TList::ListNode**)(&(UserManager::user_list))), UserManager::user_list_dat, UserManager::user_list_dat_path);
        }
        // printf("GAMEOVER SWITCH\n");
    }

    //Checks if the conditions for a game over are fullfilled and starts the logic if that's the case
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

    //Updates the current state of the game over title
    void UpdateGameOverScreen(){
        gameover_title_ltc += 1000/Utils::kFPS;
        if(gameover_title_ltc >= gameover_title_lt){
            ufo.shot.is_active = false;
            for (int i = 0; i < 8; i++){
                (ufo.death_particles+i)->is_active = false;
            }
            Ufo::UpdateUfo(&ufo);
            ufo.type = Ufo::UfoType::NONE;
            
            
            ufo.spawn_ltc = 10000; 
            TList::ClearList(&asteroid_ingame);
            TList::ClearList(&asteroid_particles);
            TList::ClearList(&spawned_power_ups);

            printf("GAME OVER\n");
            MainMenu::Load();
        }
    }

    //Updates the current state of the spawned powerups
    void UpdatePowerUps(){
        for(TList::ListNode *aux = spawned_power_ups; aux != nullptr; aux = aux->next){
            PowerUps::UpdatePowerUp(&(aux->info.powerUp_info));
        }
    }

    //Updates the current state of the background beat sound
    void UpdateBeat(){
        if(beat_ltc < beat_lt){
            beat_ltc += 1000/Utils::kFPS;
        }else{
            if (is_low_beat)
            {
                AudioLib::PlaySound(AudioLib::BEAT_1);
            }else{
                AudioLib::PlaySound(AudioLib::BEAT_2);
            }

            beat_ltc = 0;
            is_low_beat = !is_low_beat;
        }
    }

    //Whole Gameplay update method
    void Update(){

        //GameOver Management
        if(GameManager::game_status.level == GameManager::Level::GAMEPLAY){
            UpdateBeat();
            UpdatePowerUps();
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
    
    //Loads the Gameplay screen for a new game
    void Load(PlayedGames::Gamemode gm, UserManager::User* p2 = nullptr){
        TList::ListInfo aux_game_info = {NULL};
        PlayedGames::PlayedGame *aux_actual_game = nullptr;
        TList::ClearList(&spawned_power_ups);

        GameManager::game_status.level = GameManager::Level::GAMEPLAY;

        //CREATE NEW GAME
        aux_game_info.game_info = PlayedGames::LoadBaseGameManager(gm, p2);
        
        TList::InsertList(((TList::ListNode**)(&(PlayedGames::game_list))), TList::ListType::PLAYED_GAME, aux_game_info);
        TList::SaveList(((TList::ListNode**)(&(PlayedGames::game_list))), PlayedGames::game_list_dat, PlayedGames::game_list_dat_path);

        //LOADS NEW GAME AS THE ACTUAL GAME
        aux_actual_game = &(TList::FindInList((TList::ListNode*)PlayedGames::game_list, aux_game_info)->info.game_info);
        GameManager::game_status.actual_game = aux_actual_game;

        RemoveCredits(aux_actual_game->p1_user, 1);
        if(p2 != nullptr){
            RemoveCredits(aux_actual_game->p2_user, 1);
        }

        TList::SaveList(((TList::ListNode**)(&(UserManager::user_list))), UserManager::user_list_dat, UserManager::user_list_dat_path);

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

        LoadGameplayLevel(true);
    }

    //Loads the Gameplay screen for a loaded game
    bool Load(PlayedGames::PlayedGame loaded_game){
        bool is_loaded = false;
        TList::ListInfo aux_game_info = {NULL};
        aux_game_info.game_info = loaded_game;
        PlayedGames::PlayedGame *aux_actual_game = nullptr;
        TList::ClearList(&spawned_power_ups);


        //LOADS LOADED GAME AS THE ACTUAL GAME IF FOUND ON GAME_LIST
        aux_actual_game = &(TList::FindInList((TList::ListNode*)PlayedGames::game_list, aux_game_info)->info.game_info);
        
        // If its found in the game list, completes the loading.
        // is_loaded variable returns feedback to the line calling it in case an
        // error message is needed
        if(aux_actual_game){
            //Loads save_time with system time
            // printf("%lld\n",aux_actual_game->save_time);
            time(&(aux_actual_game->save_time));
            // printf("%lld\n",aux_actual_game->save_time);
            TList::SaveList(((TList::ListNode**)(&(PlayedGames::game_list))), PlayedGames::game_list_dat, PlayedGames::game_list_dat_path);

            TList::ClearList(&spawned_power_ups);

            GameManager::game_status.actual_game = aux_actual_game;

            GameManager::game_status.level = GameManager::Level::GAMEPLAY;
            
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

            LoadGameplayLevel(true);
            is_loaded = true;
        }
        

        return is_loaded;
    }

    //Gameplay DRAW

    //Draws the active asteroids on screen
    void DrawGameAsteroids(){
        for(TList::ListNode *p = asteroid_ingame; p!=nullptr; p = p->next){
            PolyLibJMATH::DrawPoly(p->info.asteroid_info.figure,false);
        }
    }

    //Draws the active death particles of the asteroids on screen
    void DrawGameAsteroidsParticles(){
        for(TList::ListNode *p = asteroid_particles; p!=nullptr; p = p->next){
            Particles::DrawParticle(p->info.particle_info);
        }
    }

    //Draws the Player 1 lifes UI
    void DrawP1Lifes(PlayedGames::PlayedGame actual_game){
        float base_height = Utils::kBaseFontSize*2.0f + p1_life_figure.transform.scale.x+10.0f;
        float base_width =  p1_life_figure.transform.scale.x+10.0f;

        for(int i = 1; i <= actual_game.p1.lifes-1; i++){
            p1_life_figure.transform.translation = {base_width*i, base_height};
            PolyLibJMATH::UpdatePoly(&p1_life_figure);
            PolyLibJMATH::DrawPoly(p1_life_figure, false);
        }
    }

    //Draws the Player 2 lifes UI
    void DrawP2Lifes(PlayedGames::PlayedGame actual_game){
        float base_height = Utils::kBaseFontSize*2.0f + p2_life_figure.transform.scale.x+10.0f;
        float base_width = p2_life_figure.transform.scale.x+10.0f;

        for(int i = 1; i <= actual_game.p2.lifes-1; i++){
            p2_life_figure.transform.translation = {Utils::kWindowWidth - (base_width*i), base_height};
            PolyLibJMATH::UpdatePoly(&p2_life_figure);
            PolyLibJMATH::DrawPoly(p2_life_figure, false);
        }
    }

    //Draws the Player 1 UI
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

    //Draws the Player 2 UI
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

    //Draws the sum of both players score at the top center of the screen
    void DrawCoopScore(PlayedGames::PlayedGame actual_game){
        UILib::DrawIntToText(
            {(Utils::kWindowWidth*0.5f) - ((Utils::kBaseFontSize*2.0f)*2.0f),Utils::kBaseFontSize*2.0f},
            {
                {255,255,255,255},
                actual_game.p2_user->alias,
                Utils::kBaseFontSize*2.0f
            },
            actual_game.p1.score+actual_game.p2.score,6,true
        );
    }
    

    //Draws the game players ships and its resources on screen
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

    //Draws the whole Game UI on screen
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
                DrawP1UI(actual_game);
                DrawP1Lifes(actual_game);

                DrawP2UI(actual_game);
                DrawP2Lifes(actual_game);
            break;
            case PlayedGames::Gamemode::MP_COOP:
                DrawP1UI(actual_game);
                DrawP1Lifes(actual_game);

                DrawCoopScore(actual_game);

                DrawP2UI(actual_game);
                DrawP2Lifes(actual_game);
            break;
        }
    }

    //Draws the Game over title on screen
    void DrawGameOverScreen(){
        UILib::Text game_over_txt;
        UILib::Text highscore_p1_txt;
        UILib::Text highscore_p2_txt;
        switch (GameManager::game_status.actual_game->gamemode){
            case PlayedGames::Gamemode::SP:
            case PlayedGames::Gamemode::MP_COOP:
                highscore_p1_txt = {
                    {255,255,255,255},
                    "NEW HIGHSCORE",
                    Utils::kBaseFontSize*3.0f
                };

                game_over_txt = {
                    {255,255,255,255},
                    "GAME OVER",
                    Utils::kBaseFontSize*3.0f
                };
            break;

            case PlayedGames::Gamemode::MP_ALT:
            case PlayedGames::Gamemode::MP_VS:
                highscore_p1_txt = {
                    {150,150,255,255},
                    "NEW HIGHSCORE",
                    Utils::kBaseFontSize*3.0f
                };

                if(GameManager::game_status.actual_game->p1.score == GameManager::game_status.actual_game->p2.score){
                    game_over_txt = {
                        {255,255,255,255},
                        "DRAW",
                        Utils::kBaseFontSize*2.5f
                    };
                }else{
                    if (GameManager::game_status.actual_game->p1.score > GameManager::game_status.actual_game->p2.score){
                        game_over_txt = {
                            {150,150,255,255},
                            "Player 1 Wins",
                            Utils::kBaseFontSize*2.5f
                        };
                    }else{
                        game_over_txt = {
                            {255,0,0,255},
                            "Player 2 Wins",
                            Utils::kBaseFontSize*2.5f
                        };
                    }
                }

                highscore_p2_txt = {
                    {255,0,0,255},
                    "NEW HIGHSCORE",
                    Utils::kBaseFontSize*3.0f
                };
            break;
        }
        
        if(new_highscore_p1){
            UILib::DrawText(
                {
                    Utils::kWindowWidth*0.5f - (highscore_p1_txt.font_size * (strlen(highscore_p1_txt.text)/3)), 
                    Utils::kWindowHeight*0.5f - ((highscore_p1_txt.font_size)*2)
                }, 
                highscore_p1_txt
            );
        }
        UILib::DrawText(
            {
                Utils::kWindowWidth*0.5f - (game_over_txt.font_size * (strlen(game_over_txt.text)/3)), 
                Utils::kWindowHeight*0.5f - (game_over_txt.font_size*0.5f)
            }, 
            game_over_txt
        );
        if(new_highscore_p2){
            UILib::DrawText(
                {
                    Utils::kWindowWidth*0.5f - (highscore_p2_txt.font_size * (strlen(highscore_p2_txt.text)/3)), 
                    Utils::kWindowHeight*0.5f + ((highscore_p2_txt.font_size)*1)
                }, 
                highscore_p2_txt
            );
        }
    }

    //Draws the active power ups on screen
    void DrawPowerUps(){
        for(TList::ListNode *aux = spawned_power_ups; aux != nullptr; aux = aux->next){
            PowerUps::DrawPowerUp(aux->info.powerUp_info);
        }
    }

    //Whole Gameplay draw method
    void Draw(){
        DrawGameAsteroids();
        DrawGameAsteroidsParticles();
        Ufo::DrawUfo(ufo);
        if(GameManager::game_status.level == GameManager::Level::GAMEPLAY){
            DrawPowerUps();
            // printf("DrawPowerUps\n");
            DrawPlayers(*(GameManager::game_status.actual_game));
            // printf("DrawPlayers\n");
            DrawGameUI(*(GameManager::game_status.actual_game));
            //Draw Game over while counter is smaller than the life time
            if(gameover_title_ltc < gameover_title_lt){
                //Blink Condition
                if((gameover_title_ltc % 1000) < 750){
                    DrawGameOverScreen();
                }
            }
        }
    }

    //Releases the dynamic memory used
    void EmptyMemory(){
        Ufo::EmptyUfoMemory(&ufo);
        TList::ClearList(&asteroid_ingame);
        TList::ClearList(&asteroid_particles);
        TList::ClearList(&spawned_power_ups);
        PolyLibJMATH::EmptyPolyMemory(&p1_life_figure);
        PolyLibJMATH::EmptyPolyMemory(&p2_life_figure);
    }
}