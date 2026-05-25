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

    int last_game_id = -1;

    char **gamemode_texts;

    //Creates a new game with default values
    PlayedGame NewGame(){
        TList::ListNode** aux_list = (TList::ListNode**) &game_list;

        PlayedGame new_game;
        new_game.game_id = ++last_game_id;
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

    // Instantiates a new game based on an already existing one
    PlayedGame NewGameCopy(PlayedGame *game){
        TList::ListNode** aux_list = (TList::ListNode**) &game_list;

        PlayedGame new_game;
        new_game.game_id = ++last_game_id;
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

    //Returns a new base game given a gamemode and a posible second user. 
    //The main user will always be the currently logged user
    //Mainly used to create a completely new game
    PlayedGame LoadBaseGameManager(Gamemode gm, UserManager::User* p2 = nullptr){
        PlayedGame new_game = NewGame();
        new_game.gamemode = gm;
        new_game.p1_user = GameManager::game_status.logged_user;
        new_game.p2_user = p2;
        
        return new_game;
    }

    //Writes the needed values of the given game user to the given opened file
    void SaveGameUser(UserManager::User *user , FILE *dat_file){
        TList::ListInfo aux_user_info = {NULL};
        // printf("\n\nSaveGameUser\n");
        
        if(user == nullptr){
            // printf("Saving user.username\n");
            fwrite(UserManager::empty_user.username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
            // printf("Saved user.username %s\n", UserManager::empty_user.username);
        }else{
            aux_user_info.user_info = *user;
            // printf("Saving aux_user_info.user_info\n");
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

    //Writes the needed values of the given game player on the given opened file
    void SaveGamePlayer(Players::Player player, FILE *dat_file){
        //printf("Saving player.lifes\n");
        fwrite(&(player.lifes), sizeof(int), 1, dat_file);
        //printf("Saved player.lifes\n");

        //printf("Saving player.score\n");
        fwrite(&(player.score), sizeof(int), 1, dat_file);
        //printf("Saved player.score\n");

        //printf("Saving player.score\n");
        fwrite(&(player.life_up_score), sizeof(int), 1, dat_file);
        //printf("Saved player.score\n");
        
        //printf("Saving player.score\n");
        fwrite(&(player.round), sizeof(int), 1, dat_file);
        //printf("Saved player.score\n");
        
    }

    //Writes the needed values of the given game on the given opened file
    void SaveGame(PlayedGame game, FILE *dat_file){
        //char* values are writen with +1 length to ensure '\0' character has space to be saved 
        //(Should have been written the same way previously)
        //Saved separately because User values are memory pointers, therefore the only needed value is de username (id value)
        //Player values only register lifes, score, life-up score and round since their ship will get generated on game load
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

    //Reads the game user saved values from the given opened file, 
    //looks for a match in the userlist, 
    //and loads the information on the given game 
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

    //Reads the game player saved values from the given opened file, 
    //and loads the information on the given game player 
    void LoadGamePlayer(Players::Player *player, FILE *dat_file){
        fread(&(player->lifes), sizeof(int), 1, dat_file);
        fread(&(player->score), sizeof(int), 1, dat_file);
        fread(&(player->life_up_score), sizeof(int), 1, dat_file);
        fread(&(player->round), sizeof(int), 1, dat_file);
    }

    //Reads the values of a game in the given opened file and 
    //instantiates a new PlayedGame struct with the loaded values which
    //is returned as a result
    PlayedGame LoadGame(FILE *dat_file){
        PlayedGame loaded_game = NewGame();
        last_game_id--;

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

    //Loads into game_list the totality of the games saved at the game_list_dat_path file
    bool LoadGameList(){
        TList::ListNode** aux_list = (TList::ListNode**) &game_list;
        bool is_loaded = TList::LoadList(aux_list, TList::ListType::PLAYED_GAME, game_list_dat, game_list_dat_path);

        if(is_loaded){
            for(TList::ListNode *aux = *aux_list; aux->next != nullptr; aux = aux->next){
                if(last_game_id <= aux->info.game_info.game_id){
                    last_game_id = aux->info.game_info.game_id;
                }
            }
        }

        // printf("%p || %p\n",*aux_list,user_list);
        return is_loaded;
    }

    //Initialization function for PlayedGames.cc
    void Init(){
        gamemode_texts = (char**) malloc(sizeof(char*)*4);

        *(gamemode_texts+Gamemode::SP) = "SINGLE PLAYER";
        *(gamemode_texts+Gamemode::MP_ALT) = "MP ALTERNATE";
        *(gamemode_texts+Gamemode::MP_VS) = "MP VERSUS";
        *(gamemode_texts+Gamemode::MP_COOP) = "MP COOPERATIVE";

        LoadGameList();
    }

    //Used to ensures all PlayedGames.cc FILE variables are closed
    void CloseFiles(){
        if(game_list_dat != nullptr){
            fclose(game_list_dat);
        }
    }

    //Releases all the dynamic memory used in the given game
    void EmptyGameMemory(PlayedGame *game){
        //Since the dynamic memory used in the PlayedGame structure is not
        //PlayedGame exclusive at the moment since those values are the game Users
        //Being this the case the User memory management is for the UserManager to take care of.
        //Yet this function is preserved for posible future values such as "char* game_name" for instance
        //Preserving the structure mainly for the TList branch that corresponds to 
        //the PlayedGame list type management
    }

    //Releases all the dynamic memoery of the PlayedGames.cc variables 
    void EmptyMemory(){
        TList::ClearList((TList::ListNode**)&game_list);
        free(gamemode_texts);
    }
}
