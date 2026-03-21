#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "./UserManager.h"


//Holds the game information needed globally, that may be needed for Level/Screen management 
namespace UserManager{
    
    //Extra length space for '\0' character to have space
    unsigned char kDefaultStrL = 21;
    unsigned char kAliasStrL = 4;
    unsigned char kEmailStrL = 51;
    unsigned char kCreditsStrL = 3;

}