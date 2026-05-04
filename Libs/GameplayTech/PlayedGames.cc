#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

    char **gamemode_texts;

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

        //Loads save_time with system time
        time(&new_game.save_time); 

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

    PlayedGame LoadBaseGameManager(Gamemode gm, UserManager::User* p2 = nullptr){
        PlayedGame new_game = NewGame();
        new_game.gamemode = gm;
        new_game.p1_user = GameManager::game_status.logged_user;
        new_game.p2_user = p2;
        
        return new_game;
    }

    void SaveGameUser(UserManager::User *user , FILE *dat_file){
        TList::ListInfo aux_user_info = {NULL};
        // printf("\n\nSaveGameUser\n");
        
        if(user == nullptr){
            // printf("Saving user.username\n");
            fwrite(UserManager::empty_user.username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
            // printf("Saved user.username %s\n", UserManager::empty_user.username);
        }else{
            aux_user_info.user_info = *user;
            if(TList::FindInList((TList::ListNode*)UserManager::user_list, aux_user_info) == nullptr){
                // printf("Saving user.username\n");
                fwrite(UserManager::empty_user.username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
                // printf("Saved user.username %s\n", UserManager::empty_user.username);
            }else{
                // printf("Saving game.user->username\n");
                fwrite(user->username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
                // printf("Saved game.user->username %s\n", user->username);
            }
        }
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
            //printf("Saved game.game_id %d\n",game.game_id);
            
            //printf("Saving game.gamemode\n");
            fwrite(&(game.gamemode), sizeof(Gamemode), 1, dat_file);
            //printf("Saved game.gamemode\n");
            
            SaveGameUser(game.p1_user, dat_file);
            SaveGameUser(game.p2_user, dat_file);
            
            //printf("Saving game.is_player1_turn\n");
            fwrite(&(game.is_player1_turn), sizeof(bool), 1, dat_file);
            //printf("Saved game.is_player1_turn\n");

            SaveGamePlayer(game.p1, dat_file);
            SaveGamePlayer(game.p2, dat_file);
            
            fwrite(&(game.compare_score), sizeof(int), 1, dat_file);
            //printf("Saving game.is_finished\n");
            fwrite(&(game.is_finished), sizeof(bool), 1, dat_file);
            //printf("Saved game.is_finished\n");
            fwrite(&(game.save_time), sizeof(time_t), 1, dat_file);
        }

        // UserManager::EmptyUserMemory(&aux_user_info.user_info);
    }

    void LoadGameUsers(PlayedGame *loaded_game, FILE *dat_file){
        TList::ListInfo aux_info = {NULL};
        TList::ListNode *aux_node = nullptr;
        aux_info.user_info = UserManager::NewUser();


        fread(aux_info.user_info.username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
        aux_node = TList::FindInList((TList::ListNode*)UserManager::user_list, aux_info);
        if(aux_node == nullptr){
            loaded_game->p1_user = &UserManager::empty_user;
        }else{
            loaded_game->p1_user = &(aux_node->info.user_info);
        }

        fread(aux_info.user_info.username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
        aux_node = TList::FindInList((TList::ListNode*)UserManager::user_list, aux_info);
        if(aux_node == nullptr){
            loaded_game->p2_user = &UserManager::empty_user;
        }else{
            loaded_game->p2_user = &(aux_node->info.user_info);
        }

        UserManager::EmptyUserMemory(&aux_info.user_info);
    }

    void LoadGamePlayer(Players::Player *player, FILE *dat_file){
        fread(&(player->lifes), sizeof(int), 1, dat_file);
        fread(&(player->score), sizeof(int), 1, dat_file);
        fread(&(player->round), sizeof(int), 1, dat_file);
    }

    PlayedGame LoadGame(FILE *dat_file){
        PlayedGame loaded_game = NewGame();

        if(dat_file != NULL){
            // printf("START LOADING\n");
            fread(&(loaded_game.game_id), sizeof(int), 1, dat_file);
            // printf("game_id %d\n",loaded_game.game_id);
            fread(&(loaded_game.gamemode), sizeof(Gamemode), 1, dat_file);
            // printf("gamemode %d\n",loaded_game.gamemode);
            

            LoadGameUsers(&loaded_game, dat_file);

            fread(&(loaded_game.is_player1_turn), sizeof(bool), 1, dat_file);

            LoadGamePlayer(&(loaded_game.p1), dat_file);
            LoadGamePlayer(&(loaded_game.p2), dat_file);
            
            fread(&(loaded_game.compare_score), sizeof(int), 1, dat_file);

            fread(&(loaded_game.is_finished), sizeof(bool), 1, dat_file);

            fread(&(loaded_game.save_time), sizeof(time_t), 1, dat_file);
        }
        return loaded_game;
    }

    bool LoadGameList(){
        TList::ListNode** aux_list = (TList::ListNode**) &game_list;
        bool is_loaded = TList::LoadList(aux_list, TList::ListType::PLAYED_GAME, game_list_dat, game_list_dat_path);

        // printf("%p || %p\n",*aux_list,user_list);
        return is_loaded;
    }

    void Init(){
        gamemode_texts = (char**) malloc(sizeof(char*)*4);

        *(gamemode_texts+Gamemode::SP) = "SINGLE PLAYER";
        *(gamemode_texts+Gamemode::MP_ALT) = "MP ALTERNATE";
        *(gamemode_texts+Gamemode::MP_COOP) = "MP VERSUS";
        *(gamemode_texts+Gamemode::MP_VS) = "MP COOPERATIVE";

        LoadGameList();
    }

    void CloseFiles(){
        if(game_list_dat != nullptr){
            fclose(game_list_dat);
        }
    }

    void EmptyGameMemory(PlayedGame *game){
        
    }

    void EmptyMemory(){
        TList::ClearList((TList::ListNode**)&game_list);
        free(gamemode_texts);
    }
}
