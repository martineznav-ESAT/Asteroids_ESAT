#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "./TList.h"
#include "../../Code/UserManager.h"
#include "../GameplayTech/GameplayTech.h"

namespace TList{

    ListNode* CreateList(){
        return nullptr;
    }

    bool IsEmptyList(ListNode **list){
        return (*list == nullptr);
    }

    //Searches for a node by value
    ListNode* FindInList(ListNode *list, ListInfo info){
        ListNode *aux = nullptr;

        switch (list->type){
            case ListType::INT:
                for(aux = list; aux != nullptr && aux->info.int_info != info.int_info; aux = aux->next);
            break;
        
            case ListType::CHAR:
                for(aux = list; aux != nullptr && aux->info.char_info != info.char_info; aux = aux->next);
            break;

            case ListType::USER:
                // printf("\n-----Searching %s in\n",info.user_info.username);
                // PrintList(list);
                for(aux = list; aux != nullptr && strcmp(aux->info.user_info.username, info.user_info.username) != 0; aux = aux->next);
            break;

            case ListType::PLAYED_GAME:
                // printf("\n-----REGISTERED GAMES\n");
                // PrintList(list);
                for(aux = list; aux != nullptr && aux->info.game_info.game_id != info.game_info.game_id; aux = aux->next);
            break;

            case ListType::ASTEROID:
                // printf("\n-----REGISTERED GAMES\n");
                // PrintList(list);
                for(aux = list; aux != nullptr && aux->info.asteroid_info.id != info.asteroid_info.id; aux = aux->next);
            break;
        }


        return aux;
    }

    //Adds a value at the beginning of the list
    bool InsertList(ListNode **list, ListType type, ListInfo info){
        bool is_inserted = true;
        if(!IsEmptyList(list) && FindInList(*list, info)){
            is_inserted = false;
        }else{
            ListNode *aux;
            // printf("InsertList\n");
            aux = (ListNode*) malloc(sizeof(ListNode));
            aux->info = info;
            aux->type = type;
            aux->next = *list;
            aux->prev = nullptr;

            if(*list != nullptr){
                (*list)->prev = aux;
            }

            *list = aux;
        }

        return is_inserted;
    }    


    //Prints the values of one ListNode
    void PrintNodeInfo(ListNode *list){
        switch (list->type){
            case ListType::INT:
                printf("%d | ",list->info.int_info);
            break;
        
            case ListType::CHAR:
                printf("%c | ",list->info.char_info);
            break;

            case ListType::USER:
                printf("|| User %s - Passwd %s ",list->info.user_info.username, list->info.user_info.password);
                if(list->info.user_info.is_admin){
                    printf("A ");
                }
                printf("||\n");
            break;

            case ListType::PLAYED_GAME:
                printf("|| Game %d - Gamemode %d - P1 %s",list->info.game_info.game_id, list->info.game_info.gamemode, list->info.game_info.p1_user->username);
                if(strcmp(list->info.game_info.p2_user->username,"\0") != 0){
                    printf("P2 %s ",list->info.game_info.p2_user->username);
                }
                if(list->info.game_info.is_finished){
                    printf("F ");
                }{
                    printf("NF ");
                }
                printf("||\n");
            break;

            case ListType::ASTEROID:
                printf("|| Asteroid %d - Size %d - Speed:",list->info.asteroid_info.id, list->info.asteroid_info.size_level);
                JMATH::Vec3Print(list->info.asteroid_info.speed_v);
                printf("||\n");
            break;
        }
    }

    //Prints the values of the whole list
    void PrintList(ListNode *list){
        for(ListNode *p = list; p!=nullptr; p = p->next){
            PrintNodeInfo(p);
        }
    }

    ListNode* GetLastListNode(ListNode *list){
        ListNode *aux = nullptr;
        for(aux = list; aux->next != nullptr; aux = aux->next);
        return aux;
    }

    ListNode* GetIndexListNode(ListNode *list, int index){
        ListNode *aux = nullptr;
        int i = 0;
        for(aux = list; aux != nullptr && i != index; aux = aux->next, i++);
        return aux;
    }

    //Prints the values of the list in reverse order
    void ReverseShowList(ListNode *list){
        ListNode *aux = GetLastListNode(list);

        //Second traversal backwards printing info
        while(aux != nullptr){
            PrintNodeInfo(aux);
            aux = aux->prev;
        }
    }

    //Returns the number of elements in the list
    int ListLength(ListNode* list){
        int res = 0;

        for(ListNode *p = list; p!=nullptr; p = p->next){
            res++;
        }

        return res;
    }

    //Extracts a node from the list and returns it detached
    ListNode* ExtractFromList(ListNode **list_element){
        ListNode *aux_act = *list_element;
        ListNode *aux_next = (*list_element)->next;
        ListNode *aux_prev = (*list_element)->prev;

        if(aux_prev != nullptr){
            aux_prev->next = aux_next;
        }
        if(aux_next != nullptr){
            aux_next->prev = aux_prev;
        }

        (*list_element)->next = nullptr;
        (*list_element)->prev = nullptr;

        if(aux_prev == nullptr){
            *list_element = aux_next;
        }
        
        return aux_act;
    }

    //Deletes a node with the given value
    void DeleteElement(ListNode **list, ListInfo info){
        // printf("DeleteFromList\n");
        
        ListNode *aux = FindInList(*list, info);

        //Check if exists
        if(IsEmptyList(&aux)){
            printf(" >>> Not found in list\n");
        }else{
            //If exists, extract it
            if(aux == *list){
                aux = ExtractFromList(list);
            }else{
                aux = ExtractFromList(&aux);
            }

            //Free memory
            free(aux);
        }
    }

    void ClearList(ListNode **list){
        for(ListNode *act = *list; !IsEmptyList(list); act = *list){
            free(ExtractFromList(list));
        }
        *list = CreateList();
    }

    void SaveNode(ListNode *list, FILE *file){
        if(file != NULL && !IsEmptyList(&list)){
            fwrite(&(list->type), sizeof(list->type), 1, file);

            switch (list->type){
                case ListType::INT:
                    fwrite(&(list->info.int_info), sizeof(list->info.int_info), 1, file);
                break;
            
                case ListType::CHAR:
                    fwrite(&(list->info.char_info), sizeof(list->info.char_info), 1, file);
                break;

                case ListType::USER:
                    UserManager::SaveUser(list->info.user_info, file);
                break;

                case ListType::PLAYED_GAME:
                    PlayedGames::SaveGame(list->info.game_info, file);
                break;
            }
        }
    }

    void SaveList(ListNode **list, FILE *dat_file, char* dat_path){
        ListNode *aux = GetLastListNode(*list);
        dat_file = fopen(dat_path, "wb");

        //Saved backwards to mantain consistency when loaded again
        for(ListNode *p = aux; p!=nullptr; p = p->prev){
            SaveNode(p, dat_file);
        }
        fclose(dat_file);
    }

    bool LoadList(ListNode **list_to_load, ListType list_type, FILE *dat_file, char* dat_path){
        bool is_loaded = true;
        *list_to_load = CreateList();
        ListType aux_type;
        //Initialize "empty" info
        ListInfo aux_info = {NULL};

        // Check file before reading
        if((dat_file = fopen(dat_path,"rb")) == NULL){
            printf("ERROR NOT FOUND: \n%s\n",dat_path);
            is_loaded = false;
        }else{
            //If the read type is different than the parameter intended, fails to read file 
            //Following C logic, the while conditional should execute the fread before aux_type comparison. 
            //So the aux_type == list_type comparison should always be properly executed
            while (fread(&aux_type, sizeof(ListType), 1, dat_file) && aux_type == list_type){
                switch (list_type){
                    case ListType::INT:
                        fread(&(aux_info.int_info), sizeof(aux_info.int_info), 1, dat_file);
                    break;
                
                    case ListType::CHAR:
                        fread(&(aux_info.char_info), sizeof(aux_info.char_info), 1, dat_file);
                    break;

                    case ListType::USER:
                        aux_info.user_info = UserManager::LoadUser(dat_file);
                    break;

                    case ListType::PLAYED_GAME:
                        aux_info.game_info = PlayedGames::LoadGame(dat_file);
                    break;
                }
                // printf("LOADED\n");
                InsertList(list_to_load, list_type, aux_info);
                // printf("INSERTED\n");
            }

            if(aux_type != list_type){
                is_loaded = false;
            }else{
                printf("---- REGISTERED USERS LOADED LIST ----\n");
                PrintList(*list_to_load);
            }

            fclose(dat_file);
        }

        return is_loaded;
    }
}
