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

#include "./GameManager.h"
#include "./MainMenu.h"


namespace Gameplay{

    //Whole Gameplay initializer
    void Init(){
        
    }

    //Gameplay LOAD

    //Loads the Gameplay
    void Load(PlayedGames::Gamemode gm){
        switch (gm){
            case PlayedGames::Gamemode::SP:

            break;
            case PlayedGames::Gamemode::MP_ALT :
            
            break;
            case PlayedGames::Gamemode::MP_VS :
            
            break;
            case PlayedGames::Gamemode::MP_COOP :
            
            break;
        }
        GameManager::game_status.level = GameManager::Level::GAMEPLAY;
    }

    //Gameplay UPDATE

    //Whole Gameplay update method
    void Update(){

        
    }

    //Gameplay DRAW
    void DrawMenuItems(){
        
    }

    //Whole Gameplay draw method
    void Draw(){
        
    }

    void EmptyMemory(){
        
    }
}