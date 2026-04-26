#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Code/UserManager.h"
#include "../../Code/GameManager.h"

#include "../CustomLibs/TList.h"
#include "./PlayedGames.h"

namespace PlayedGames{

    FILE *game_list_dat = nullptr;
    char *game_list_dat_path = "./Assets/Files/Data/game_list.dat";
    
    //Whenever game_list is needed, create an aux variable such as:
    //TList::ListNode** aux_list = (TList::ListNode**) &game_list;
    //To work with the ListNode typing while aiming the same memory direction with a pointer anidation
    void *game_list = nullptr; 

    PlayedGame NewGame(){
        TList::ListNode** aux_list = (TList::ListNode**) &game_list;

        PlayedGame new_game;
        new_game.game_id = TList::ListLength(*aux_list);
        new_game.gamemode = Gamemode::SP;
        new_game.p1_user = nullptr;
        new_game.p2_user = nullptr;
        new_game.is_player1_turn = true;
        new_game.p1 = Players::NewPlayer();
        new_game.p2 = Players::NewPlayer();
        new_game.compare_score = 0;
        new_game.is_finished = false;

        return new_game;
    }

    PlayedGame NewGameCopy(PlayedGame *game){
        TList::ListNode** aux_list = (TList::ListNode**) &game_list;

        PlayedGame new_game;
        new_game.game_id = TList::ListLength(*aux_list);
        new_game.gamemode = game->gamemode;
        new_game.p1_user = game->p1_user;
        new_game.p2_user = game->p2_user;
        new_game.is_player1_turn = game->is_player1_turn;
        new_game.p1 = game->p1;
        new_game.p2 = game->p2;
        new_game.compare_score = game->compare_score;
        new_game.is_finished = game->is_finished;

        return new_game;
    }

    PlayedGame LoadBaseGameManagerGame(Gamemode gm, UserManager::User* p2 = nullptr){
        PlayedGame new_game = NewGame();
        new_game.gamemode = gm;
        new_game.p1_user = GameManager::game_status.logged_user;
        new_game.p2_user = p2;
        
        return new_game;
    }

    void SaveGameP2User(PlayedGame game, FILE *dat_file){
        UserManager::User aux_p2_user = UserManager::NewUser();

        if(game.p2_user == nullptr){
            //printf("Saving aux_p2_user.username\n");
            fwrite(aux_p2_user.username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
            //printf("Saved aux_p2_user.username\n");
        }else{
            //printf("Saving game.p2_user->username\n");
            fwrite(game.p2_user->username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
            //printf("Saved game.p2_user->username\n");
        }

        UserManager::FreeUserMemory(&aux_p2_user);
    }

    void SaveGamePlayer(Players::Player player, FILE *dat_file){
        //printf("Saving player.lifes\n");
        fwrite(&(player.lifes), sizeof(int), 1, dat_file);
        //printf("Saved player.lifes\n");

        //printf("Saving player.score\n");
        fwrite(&(player.score), sizeof(int), 1, dat_file);
        //printf("Saved player.score\n");
        
        //printf("Saving player.score\n");
        fwrite(&(player.round), sizeof(int), 1, dat_file);
        //printf("Saved player.score\n");
        
    }

    void SaveGame(PlayedGame game, FILE *dat_file){
        //char* values are writen with +1 length to ensure '\0' character has space to be saved 
        //(Should have been written the same way previously)
        //Saved separately because User values are memory pointers, therefore the only needed value is de username (id value)
        //Player values only register lifes and score since their ship will get generated on game load
        if(dat_file != NULL){
            
            //printf("Saving game.game_id\n");
            fwrite(&(game.game_id), sizeof(int), 1, dat_file);
            //printf("Saved game.game_id\n");
            
            //printf("Saving game.gamemode\n");
            fwrite(&(game.gamemode), sizeof(Gamemode), 1, dat_file);
            //printf("Saved game.gamemode\n");
            
            
            //printf("Saving game.p1_user->username\n");
            fwrite(game.p1_user->username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
            //printf("Saved game.p1_user->username\n");

            SaveGameP2User(game, dat_file);
            
            //printf("Saving game.is_player1_turn\n");
            fwrite(&(game.is_player1_turn), sizeof(bool), 1, dat_file);
            //printf("Saved game.is_player1_turn\n");

            SaveGamePlayer(game.p1, dat_file);
            SaveGamePlayer(game.p2, dat_file);
            
            fwrite(&(game.compare_score), sizeof(int), 1, dat_file);
            //printf("Saving game.is_finished\n");
            fwrite(&(game.is_finished), sizeof(bool), 1, dat_file);
            //printf("Saved game.is_finished\n");
        }
    }

    void LoadGamePlayer(Players::Player *player, FILE *dat_file){
        fread(&(player->lifes), sizeof(int), 1, dat_file);
        fread(&(player->score), sizeof(int), 1, dat_file);
        fread(&(player->round), sizeof(int), 1, dat_file);
    }

    PlayedGame LoadGame(FILE *dat_file){
        PlayedGame loaded_game = NewGame();
        TList::ListInfo aux_info = {NULL};
        if(dat_file != NULL){
            // printf("START LOADING\n");
            fread(&(loaded_game.game_id), sizeof(int), 1, dat_file);
            // printf("game_id %d\n",loaded_game.game_id);
            fread(&(loaded_game.gamemode), sizeof(Gamemode), 1, dat_file);
            // printf("gamemode %d\n",loaded_game.gamemode);
            
            aux_info.user_info = UserManager::NewUser();

            fread(aux_info.user_info.username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
            loaded_game.p1_user = &(TList::FindInList((TList::ListNode*)UserManager::user_list, aux_info)->info.user_info);

            fread(aux_info.user_info.username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
            loaded_game.p2_user = &(TList::FindInList((TList::ListNode*)UserManager::user_list, aux_info)->info.user_info);

            fread(&(loaded_game.is_player1_turn), sizeof(bool), 1, dat_file);

            LoadGamePlayer(&(loaded_game.p1), dat_file);
            LoadGamePlayer(&(loaded_game.p2), dat_file);
            
            fread(&(loaded_game.compare_score), sizeof(int), 1, dat_file);

            fread(&(loaded_game.is_finished), sizeof(bool), 1, dat_file);
        }
        return loaded_game;
    }

    bool LoadGameList(){
        TList::ListNode** aux_list = (TList::ListNode**) &game_list;
        bool is_loaded = TList::LoadList(aux_list, TList::ListType::PLAYED_GAME, game_list_dat, game_list_dat_path);

        // printf("%p || %p\n",*aux_list,user_list);
        return is_loaded;
    }
}
