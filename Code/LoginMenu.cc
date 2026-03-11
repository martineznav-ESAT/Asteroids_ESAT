#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "../Libs/CustomLibs/Utils.h"
#include "../Libs/CustomLibs/UILib.h"
#include "./GameStatus.h"

namespace LoginMenu{
    UILib::Button *buttons = nullptr;

    //Actions
    void LoginAction(){
        
    }

    void RegisterAction(){
        
    }
    
    void InitButtons(){
        int total_buttons = 4;
        // JMATH::Vec2 centered_bottom_coords = {};
        buttons = (UILib::Button*) malloc(sizeof(UILib::Button)*total_buttons);

        

        // *(buttons+0) = {

        // }
    }

    void Init(){
        InitButtons();
    }

    void Load(){

    }

    void ButtonUpdate(){

    }

    void Update(){
        ButtonUpdate();
    }

    void DrawButtons(){

    }

    void Draw(){
        DrawButtons();
    }
}