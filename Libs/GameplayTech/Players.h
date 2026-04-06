#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Code/UserManager.h"

#ifndef Players_H
#define Players_H

namespace Players{
    struct Player{
        // Ship ship;
        int lifes;
        int score;
    };

    Player NewPlayer();
}

#endif
