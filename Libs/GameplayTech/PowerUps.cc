#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "./PowerUps.h"

#include "../CustomLibs/PolyLibJMATH.h"

#include "../../Code/Gameplay.h"

namespace PowerUps{

    void Init(){

    }

    PowerUp NewPowerUp(PU_Type type){
        PowerUp new_powerUp;
        new_powerUp.type = type;
        PolyLibJMATH::InitPoly(
            &new_powerUp.base_figure,
            4,
            {10.0f,10.0f},
            45.0f,
            {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f},
            {255,255,255},
            {0.0f,0.0f}
        );
        switch(new_powerUp.type){
            case FRIENDLY_FIRE:

            break;
        }
        new_powerUp.duration_lt = 8000;
        new_powerUp.duration_ltc = 0;
        return new_powerUp;
    }

    void IsPowerUpActive(PowerUp *powerUp){
    
    }

    void IsPowerUpBlinking(PowerUp *powerUp){
    
    }

    void UpdatePowerUp(PowerUp *powerUp){
        
    }

    void DrawPowerUp(PowerUp *powerUp){
        
    }
}
