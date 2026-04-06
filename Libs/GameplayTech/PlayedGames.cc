#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Code/UserManager.h"
#include "../../Code/GameManager.h"

#include "../CustomLibs/TList.h"
#include "./PlayedGames.h"

namespace PlayedGames{

    const FILE *game_list_dat = nullptr;
    const char *game_list_dat_path = "./Assets/Files/Data/game_list.dat";
    
    //Whenever game_list is needed, create an aux variable such as:
    //TList::ListNode** aux_list = (TList::ListNode**) &game_list;
    //To work with the ListNode typing while aiming the same memory direction with a pointer anidation
    void *game_list = nullptr; 

    PlayedGame NewGame(){
        TList::ListNode** aux_list = (TList::ListNode**) &game_list;

        PlayedGame new_game;
        new_game.game_id = TList::ListLength(*aux_list);
        new_game.gamemode = Gamemode::SP;
        new_game.is_finished = false;

        return new_game;
    }

    PlayedGame LoadBaseGameManagerGame(Gamemode gm){
        PlayedGame new_game = NewGame();
        new_game.p1_user = GameManager::game_status.logged_user;
        new_game.p2_user = nullptr;

        new_game.p1 = Players::NewPlayer();
        new_game.p2 = Players::NewPlayer();

        return new_game;
    }

    void SaveGame(PlayedGame game, FILE *file){
        //TO_DO
        printf("Save Game In File WIP\n");
    }

    PlayedGame LoadGame(FILE *dat_file){
        PlayedGame loaded_game;
        //TO_DO
        printf("Load Game From File WIP\n");
        return loaded_game;
    }
}
