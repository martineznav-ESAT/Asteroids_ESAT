#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "./Libs/CustomLibs/TList.h"
#include "./Libs/CustomLibs/Utils.h"

#include "./Code/GameManager.h"
#include "./Code/LoginMenu.h"
#include "./Code/RegisterMenu.h"
#include "./Code/MainMenu.h"
#include "./Code/PlayMenu.h"
#include "./Code/HighscoresMenu.h"
#include "./Code/AdminMenu.h"
#include "./Code/Gameplay.h"

void InitGame(){
    Players::Init();
    Asteroids::Init();
    Ufo::Init();
    
    LoginMenu::Init();
    RegisterMenu::Init();
    MainMenu::Init();
    PlayMenu::Init();
    AdminMenu::Init();

    Gameplay::Init();

    GameManager::LoadInitLevel();
    PlayedGames::LoadGameList();
    HighscoresMenu::Init();
}

void UpdateGame(){
    GameManager::DebugUpdate();
    switch(GameManager::game_status.level){
        case GameManager::Level::LOGIN_MENU :
            LoginMenu::Update();
        break;

        case GameManager::Level::REGISTER_MENU :
            RegisterMenu::Update();
        break;

        case GameManager::Level::MAIN_MENU :
            Gameplay::Update();
            MainMenu::Update();
        break;

        case GameManager::Level::PLAY_MENU :
            PlayMenu::Update();
        break;

        case GameManager::Level::HIGHSCORES_MENU :
            HighscoresMenu::Update();
        break;

        case GameManager::Level::ADMIN_MENU :
            AdminMenu::Update();
        break;

        case GameManager::Level::GAMEPLAY :
            Gameplay::Update();
        break;
    }
}

void DrawGame(){
    esat::DrawClear(0,0,0);
    switch(GameManager::game_status.level){
        case GameManager::Level::LOGIN_MENU :
            LoginMenu::Draw();
        break;

        case GameManager::Level::REGISTER_MENU :
            RegisterMenu::Draw();
        break;

        case GameManager::Level::MAIN_MENU :
            Gameplay::Draw(); //Asteroids moving background
            MainMenu::Draw();
        break;

        case GameManager::Level::PLAY_MENU :
            PlayMenu::Draw();
        break;

        case GameManager::Level::HIGHSCORES_MENU :
            HighscoresMenu::Draw();
        break;

        case GameManager::Level::ADMIN_MENU :
            AdminMenu::Draw();
        break;

        case GameManager::Level::GAMEPLAY :
            Gameplay::Draw();
        break;
    }
}

bool CloseGameCondition(){
    return (
        esat::WindowIsOpened() && 
        !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape) && 
        GameManager::game_status.level != GameManager::Level::QUIT
    );
}

void EmptyMemory(){
    LoginMenu::EmptyMemory();
    RegisterMenu::EmptyMemory();
    MainMenu::EmptyMemory();
    PlayMenu::EmptyMemory();
    HighscoresMenu::EmptyMemory();
    AdminMenu::EmptyMemory();
    Gameplay::EmptyMemory();
    Asteroids::EmptyMemory();
}

void CloseFiles(){
    UserManager::CloseFiles();
}

int esat::main(int argc, char **argv) {
    Utils::GenerateRandomSeed();

	esat::WindowInit(Utils::kWindowWidth, Utils::kWindowHeight);
	WindowSetMouseVisibility(true);

    esat::DrawSetTextFont("./Assets/Fonts/Hyperspace.ttf");

    InitGame();

    while(CloseGameCondition()) {        
        Utils::last_time = esat::Time();

        esat::DrawBegin();
        esat::DrawClear(0,0,0);
        
        //DEBUGGING CONTROLS
        if(esat::IsSpecialKeyDown(esat::kSpecialKey_Alt)){
            printf("TOGGLE COLLIDERS\n");
            Collisions::show_colliders = !(Collisions::show_colliders);
        }

        UpdateGame();
        DrawGame();

        esat::DrawEnd();  	
        esat::WindowFrame();

        Utils::ControlFps();
    }
        
    TList::SaveList(((TList::ListNode**)(&(PlayedGames::game_list))), PlayedGames::game_list_dat, PlayedGames::game_list_dat_path);
    CloseFiles();
    EmptyMemory();

    esat::WindowDestroy();

    return 0;  
}