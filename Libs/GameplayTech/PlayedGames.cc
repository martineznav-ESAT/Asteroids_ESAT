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
        new_game.round = 1;
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

    void SaveGameP2User(PlayedGame game, FILE *dat_file){
        UserManager::User aux_p2_user = UserManager::NewUser();

        if(game.p2_user == nullptr){
            fwrite(aux_p2_user.username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
        }else{
            fwrite(game.p2_user->username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
        }

        UserManager::FreeUserMemory(&aux_p2_user);
    }

    void SaveGamePlayer(Players::Player player, FILE *dat_file){
        fwrite(&(player.lifes), sizeof(int), 1, dat_file);
        fwrite(&(player.score), sizeof(int), 1, dat_file);
    }

    void SaveGame(PlayedGame game, FILE *dat_file){
        //char* values are writen with +1 length to ensure '\0' character has space to be saved 
        //(Should have been written the same way previously)
        //Saved separately because User values are memory pointers, therefore the only needed value is de username (id value)
        //Player values only register lifes and score since their ship will get generated on game load
        if(dat_file != NULL){
            fwrite(&(game.game_id), sizeof(int), 1, dat_file);
            fwrite(&(game.gamemode), sizeof(Gamemode), 1, dat_file);
            fwrite(&(game.round), sizeof(int), 1, dat_file);
            
            fwrite(game.p1_user->username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);

            SaveGameP2User(game, dat_file);
            
            SaveGamePlayer(game.p1, dat_file);
            SaveGamePlayer(game.p2, dat_file);
            
            fwrite(&(game.is_finished), sizeof(bool), 1, dat_file);
        }
    }

    void LoadGamePlayer(Players::Player *player, FILE *dat_file){
        fread(&(player->lifes), sizeof(int), 1, dat_file);
        fread(&(player->score), sizeof(int), 1, dat_file);
    }

    PlayedGame LoadGame(FILE *dat_file){
        PlayedGame loaded_game = NewGame();
        TList::ListInfo aux_info = {NULL};
        if(dat_file != NULL){
            printf("START LOADING\n");
            fread(&(loaded_game.game_id), sizeof(int), 1, dat_file);
            printf("game_id %d\n",loaded_game.game_id);
            fread(&(loaded_game.gamemode), sizeof(Gamemode), 1, dat_file);
            printf("gamemode %d\n",loaded_game.gamemode);
            fread(&(loaded_game.round), sizeof(int), 1, dat_file);
            printf("round %d\n",loaded_game.round);
            
            aux_info.user_info = UserManager::NewUser();

            fread(aux_info.user_info.username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
            loaded_game.p1_user = &(TList::FindInList((TList::ListNode*)UserManager::user_list, aux_info)->info.user_info);

            fread(aux_info.user_info.username, sizeof(char), UserManager::kDefaultStrL + 1, dat_file);
            loaded_game.p2_user = &(TList::FindInList((TList::ListNode*)UserManager::user_list, aux_info)->info.user_info);

            LoadGamePlayer(&(loaded_game.p1), dat_file);
            LoadGamePlayer(&(loaded_game.p2), dat_file);
            
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

    //Returns a TListNode* list with the top 10 PlayedGames with highest scores registered
    void* FindHighScores(){
        TList::ListNode* highscores = TList::CreateList();
        int i = 0;
        //TO_DO Right now only reads the first 10. Need to calculate top values and order them

        for(TList::ListNode *p = (TList::ListNode*) game_list; p!=nullptr && i < 10; p = p->next, i++){
            TList::InsertList(&highscores, TList::ListType::PLAYED_GAME, p->info);
        }
        
        return highscores;
    }
}
