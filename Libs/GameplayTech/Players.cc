#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Code/UserManager.h"

#include "./Players.h"

namespace Players{

    Player NewPlayer(){
        Player new_player;
        new_player.lifes = 3;
        new_player.score = 0;

        return new_player;
    }
}
