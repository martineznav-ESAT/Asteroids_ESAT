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
        bool is_player1_turn = true;
        UserManager::User *p1_user;
        UserManager::User *p2_user;
        Players::Player p1;
        Players::Player p2;
        int compare_score = 0;
        bool is_finished;
    };

    extern FILE *game_list_dat;
    extern char *game_list_dat_path;

    extern void *game_list; 

    PlayedGame NewGame();

    PlayedGame NewGameCopy(PlayedGame *game);

    PlayedGame LoadBaseGameManagerGame(Gamemode gm, UserManager::User* p2);

    void SaveGame(PlayedGame game, FILE *dat_file);

    PlayedGame LoadGame(FILE *dat_file);

    bool LoadGameList();

    void CloseFiles();
}

#endif
