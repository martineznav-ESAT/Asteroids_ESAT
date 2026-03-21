#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef UserManager_H
#define UserManager_H

//Holds the game information needed globally, that may be needed to save and the Level/Screen management 
namespace UserManager{

    extern unsigned char kDefaultStrL;
    extern unsigned char kAliasStrL;
    extern unsigned char kEmailStrL;
    extern unsigned char kCreditsStrL;

    struct User{
        char* username = nullptr;
        char* password = nullptr;
        char* alias = "AAA";
        char* email = nullptr;
        char* name = nullptr;
        char* surname = nullptr;
        // date dob;
        char* country;
        char* province;
        unsigned char credits = 3;
        bool is_admin;
    };

}

#endif