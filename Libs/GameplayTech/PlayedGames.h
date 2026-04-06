#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Code/UserManager.h"

#include "./Players.h"

#ifndef PlayedGames_H
#define PlayedGames_H

namespace PlayedGames{
    enum Gamemode{
        SP,
        MP_ALT,
        MP_COOP,
        MP_VS
    };

    struct PlayedGame{
        int game_id = 0;
        Gamemode gamemode;
        UserManager::User *p1_user;
        UserManager::User *p2_user;
        Players::Player p1;
        Players::Player p2;
        bool is_finished;
    };

    const extern FILE *game_list_dat;
    const extern char *game_list_dat_path;

    PlayedGame NewGame();
    PlayedGame LoadBaseGameManagerGame(Gamemode gm);

    void SaveGame(PlayedGame game, FILE *file);

    PlayedGame LoadGame(FILE *dat_file);
}

#endif
