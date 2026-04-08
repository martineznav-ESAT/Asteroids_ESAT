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

#include "../Libs/GameplayTech/GameplayTech.h"

#include "./GameManager.h"
#include "./MainMenu.h"


namespace Gameplay{
    extern TList::ListNode *asteroid_ingame;

    //Whole Gameplay initializer
    void Init();

    //Loads the Gameplay
    void Load(PlayedGames::Gamemode gm, UserManager::User* p2 = nullptr);

    void UpdateGameAsteroids();

    //Whole Gameplay update method
    void Update();

    void DrawGameAsteroids();

    //Whole Gameplay draw method
    void Draw();

    void EmptyMemory();
}