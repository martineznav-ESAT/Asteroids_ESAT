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

#include "./Code/GameStatus.h"

void EmptyMemory(){

}

void CloseFiles(){
}


void InitGame(){
}

void UpdateGame(){
    GameStatus::DebugUpdate();
    switch(GameStatus::game_status.level){
        case GameStatus::Level::LOGIN_MENU :
        
        break;

        case GameStatus::Level::REGISTER_MENU :

        break;

        case GameStatus::Level::MAIN_MENU :

        break;

        case GameStatus::Level::HIGHSCORES :

        break;

        case GameStatus::Level::ADMIN_MENU :

        break;

        case GameStatus::Level::PLAY_MENU :

        break;

        case GameStatus::Level::GAME :

        break;
    }
}

void DrawGame(){
    esat::DrawClear(0,0,0);
    switch(GameStatus::game_status.level){
        case GameStatus::Level::LOGIN_MENU :
            esat::DrawClear(100,100,200);
        break;

        case GameStatus::Level::REGISTER_MENU :
            esat::DrawClear(50,50,200);
        break;

        case GameStatus::Level::MAIN_MENU :
            esat::DrawClear(100,100,100);
        break;

        case GameStatus::Level::HIGHSCORES :
            esat::DrawClear(200,100,100);
        break;

        case GameStatus::Level::ADMIN_MENU :
            esat::DrawClear(200,50,200);
        break;

        case GameStatus::Level::PLAY_MENU :
            esat::DrawClear(200,200,200);
        break;

        case GameStatus::Level::GAME :
            esat::DrawClear(0,0,0);
        break;
    }
}

int esat::main(int argc, char **argv) {
    Utils::GenerateRandomSeed();

	esat::WindowInit(Utils::kWindowWidth, Utils::kWindowHeight);
	WindowSetMouseVisibility(true);

    esat::DrawSetTextFont("./Assets/Fonts/asteroids-display.otf");

    InitGame();

    while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {        
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