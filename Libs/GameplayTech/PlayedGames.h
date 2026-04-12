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
        int round = 1;
        UserManager::User *p1_user;
        UserManager::User *p2_user;
        Players::Player p1;
        Players::Player p2;
        bool is_finished;
    };

    extern FILE *game_list_dat;
    extern char *game_list_dat_path;

    extern void *game_list; 

    PlayedGame NewGame();
    PlayedGame LoadBaseGameManagerGame(Gamemode gm);

    void SaveGame(PlayedGame game, FILE *dat_file);

    PlayedGame LoadGame(FILE *dat_file);

    bool LoadGameList();

}

#endif
