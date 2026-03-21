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

void InitGame(){
    LoginMenu::Init();
    RegisterMenu::Init();
    MainMenu::Init();
    PlayMenu::Init();
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
            MainMenu::Update();
        break;

        case GameManager::Level::PLAY_MENU :
            PlayMenu::Update();
        break;

        case GameManager::Level::HIGHSCORES_MENU :
            HighscoresMenu::Update();
        break;

        case GameManager::Level::ADMIN_MENU :

        break;

        case GameManager::Level::GAME :

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
            MainMenu::Draw();
        break;

        case GameManager::Level::PLAY_MENU :
            PlayMenu::Draw();
        break;

        case GameManager::Level::HIGHSCORES_MENU :
            HighscoresMenu::Draw();
        break;

        case GameManager::Level::ADMIN_MENU :
            esat::DrawClear(200,50,200);
        break;

        case GameManager::Level::GAME :
            esat::DrawClear(255,255,255);
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
}

void CloseFiles(){
    
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
        
        UpdateGame();
        DrawGame();

        esat::DrawEnd();  	
        esat::WindowFrame();

        Utils::ControlFps();
    }

    CloseFiles();
    EmptyMemory();

    esat::WindowDestroy();

    return 0;  
}