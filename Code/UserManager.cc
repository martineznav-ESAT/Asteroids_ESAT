#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "./UserManager.h"

#include "../Libs/CustomLibs/TList.h"
#include "../Libs/CustomLibs/Utils.h"

//Holds the game information needed globally, that may be needed for Level/Screen management 
namespace UserManager{
    const unsigned char kDefaultStrL = 20;
    const unsigned char kAliasStrL = 3;
    const unsigned char kEmailStrL = 40;
    const unsigned char kDOBDayL = 2;
    const unsigned char kDOBMonthL = 2;
    const unsigned char kDOBYearL = 4;
    const unsigned char kCreditsStrL = 2;

    FILE *user_list_dat = nullptr;
    char *user_list_dat_path = "./Assets/Files/Data/user_list.dat";
    
    //Whenever user_list is needed, create an aux variable such as:
    //TList::ListNode** aux_list = (TList::ListNode**) &user_list;
    //To work with the ListNode typing while aiming the same memory direction with a pointer anidation
    //It has been made this way due to the impossibility to include TList.h in UserManager since it makes an "infinite include loop"
    void *user_list = nullptr;
    User empty_user;

    void Init(){
        empty_user = NewUser();
    }
    
    User NewUser(){
        User new_user;
        new_user.username = (char*) malloc(sizeof(char)*(kDefaultStrL+1));
        Utils::StringFillWithChar(new_user.username, kDefaultStrL+1, '\0', -1);
        Utils::StringFillWithChar(new_user.username, kDefaultStrL+1, '-', 3);
        
        new_user.password = (char*) malloc(sizeof(char)*(kDefaultStrL+1));
        *(new_user.password) = '\0';
        
        new_user.alias = (char*) malloc(sizeof(char)*(kAliasStrL+1));
        *(new_user.alias) = '\0';
        
        new_user.email = (char*) malloc(sizeof(char)*(kEmailStrL+1));
        *(new_user.email) = '\0';
        
        new_user.name = (char*) malloc(sizeof(char)*(kDefaultStrL+1));
        *(new_user.name) = '\0';
        
        new_user.surname = (char*) malloc(sizeof(char)*(kDefaultStrL+1));
        *(new_user.surname) = '\0';

        new_user.day_dob = 1;
        new_user.month_dob = 1;
        new_user.year_dob = 1979;
        
        new_user.country = (char*) malloc(sizeof(char)*(kDefaultStrL+1));
        *(new_user.country) = '\0';
        
        new_user.province = (char*) malloc(sizeof(char)*(kDefaultStrL+1));
        *(new_user.province) = '\0';

        new_user.credits = 10;
        new_user.is_admin = false;

        return new_user;
    }

    User NewUser(User old_user){
        User new_user;
        new_user.username = (char*) malloc(sizeof(char)*(kDefaultStrL+1));
        strcpy(new_user.username, old_user.username);
        
        new_user.password = (char*) malloc(sizeof(char)*(kDefaultStrL+1));
        strcpy(new_user.password, old_user.password);
        
        new_user.alias = (char*) malloc(sizeof(char)*(kAliasStrL+1));
        strcpy(new_user.alias, old_user.alias);
        
        new_user.email = (char*) malloc(sizeof(char)*(kEmailStrL+1));
        strcpy(new_user.email, old_user.email);
        
        new_user.name = (char*) malloc(sizeof(char)*(kDefaultStrL+1));
        strcpy(new_user.name, old_user.name);
        
        new_user.surname = (char*) malloc(sizeof(char)*(kDefaultStrL+1));
        strcpy(new_user.surname, old_user.surname);

        new_user.day_dob = old_user.day_dob;
        new_user.month_dob = old_user.month_dob;
        new_user.year_dob = old_user.year_dob;
        
        new_user.country = (char*) malloc(sizeof(char)*(kDefaultStrL+1));
        strcpy(new_user.country, old_user.country);
        
        new_user.province = (char*) malloc(sizeof(char)*(kDefaultStrL+1));
        strcpy(new_user.province, old_user.province);

        new_user.credits = old_user.credits ;
        new_user.is_admin = old_user.is_admin;

        return new_user;
    }

    void FreeUserMemory(User *user){
        free(user->username);
        free(user->password);
        free(user->alias);
        free(user->email);
        free(user->name);
        free(user->surname);
        free(user->country);
        free(user->province);
    }

    void SaveUser(User user, FILE *dat_file){
        //char* values are writen with +1 length to ensure '\0' character has space to be saved 
        //(Should have been written the same way previously) 
        if(dat_file != NULL){
            // printf("SAVE %s\n",user.username);
            fwrite(user.username, sizeof(char), kDefaultStrL+1, dat_file);
            
            // printf("SAVE %s\n",user.password);
            fwrite(user.password, sizeof(char), kDefaultStrL+1, dat_file);
            
            // printf("SAVE %s\n",user.alias);
            fwrite(user.alias, sizeof(char), kAliasStrL+1, dat_file);
            
            // printf("SAVE %s\n",user.email);
            fwrite(user.email, sizeof(char), kEmailStrL+1, dat_file);
            
            // printf("SAVE %s\n",user.name);
            fwrite(user.name, sizeof(char), kDefaultStrL+1, dat_file);
            
            // printf("SAVE %s\n",user.surname);
            fwrite(user.surname, sizeof(char), kDefaultStrL+1, dat_file);
            
            // printf("SAVE %d\n",user.day_dob);
            fwrite(&(user.day_dob), sizeof(int), 1, dat_file);
            
            // printf("SAVE %d\n",user.month_dob);
            fwrite(&(user.month_dob), sizeof(int), 1, dat_file);
            
            // printf("SAVE %d\n",user.year_dob);
            fwrite(&(user.year_dob), sizeof(int), 1, dat_file);
            
            // printf("SAVE %s\n",user.country);
            fwrite(user.country, sizeof(char), kDefaultStrL+1, dat_file);
            
            // printf("SAVE %s\n",user.province);
            fwrite(user.province, sizeof(char), kDefaultStrL+1, dat_file);
            
            // printf("SAVE %d\n",user.credits);
            fwrite(&(user.credits), sizeof(unsigned char), 1, dat_file);
            
            // if(user.is_admin){
            //     printf("SAVE ADMIN\n");
            // }else{
            //     printf("SAVE NO ADMIN\n");
            // }
            fwrite(&(user.is_admin), sizeof(bool), 1, dat_file);
        }
    }

    User LoadUser(FILE *dat_file){
        //char* values are read with +1 length to ensure '\0' character has space to be saved 
        //(Should have been written the same way previously) 
        User aux_user = NewUser();
        if(dat_file != NULL){
            // printf("START LOAD\n");
            fread(aux_user.username, sizeof(char), kDefaultStrL+1, dat_file);
            // printf("LOAD %s\n",aux_user.username);

            fread(aux_user.password, sizeof(char), kDefaultStrL+1, dat_file);
            // printf("LOAD %s\n",aux_user.password);

            fread(aux_user.alias, sizeof(char), kAliasStrL+1, dat_file);
            // printf("LOAD %s\n",aux_user.alias);

            fread(aux_user.email, sizeof(char), kEmailStrL+1, dat_file);
            // printf("LOAD %s\n",aux_user.email);

            fread(aux_user.name, sizeof(char), kDefaultStrL+1, dat_file);
            // printf("LOAD %s\n",aux_user.name);

            fread(aux_user.surname, sizeof(char), kDefaultStrL+1, dat_file);
            // printf("LOAD %s\n",aux_user.surname);

            fread(&(aux_user.day_dob), sizeof(int), 1, dat_file);
            // printf("LOAD %d\n",aux_user.day_dob);
            fread(&(aux_user.month_dob), sizeof(int), 1, dat_file);
            // printf("LOAD %d\n",aux_user.month_dob);
            fread(&(aux_user.year_dob), sizeof(int), 1, dat_file);
            // printf("LOAD %d\n",aux_user.year_dob);
            
            fread(aux_user.country, sizeof(char), kDefaultStrL+1, dat_file);
            // printf("LOAD %s\n",aux_user.country);

            fread(aux_user.province, sizeof(char), kDefaultStrL+1, dat_file);
            // printf("LOAD %s\n",aux_user.province);

            fread(&(aux_user.credits), sizeof(unsigned char), 1, dat_file);
            // printf("LOAD %d\n",aux_user.credits);

            fread(&(aux_user.is_admin), sizeof(bool), 1, dat_file);
            // if(aux_user.is_admin){
            //     printf("LOAD ADMIN\n");
            // }else{
            //     printf("LOAD NO ADMIN\n");
            // }
        }

        return aux_user;
    }

    bool LoadRegisteredUsers(){
        TList::ListNode** aux_list = (TList::ListNode**) &user_list;
        bool is_loaded = TList::LoadList(aux_list, TList::ListType::USER, user_list_dat, user_list_dat_path);

        // printf("%p || %p\n",*aux_list,user_list);
        return is_loaded;
    }

    bool RegisterNewUser(User new_user){
        TList::ListInfo aux_info = {NULL};
        bool is_registered = true;
        TList::ListNode** aux_list = (TList::ListNode**) &user_list;

        //Creates a new user copying an existing one
        aux_info.user_info = NewUser(new_user);

        if(TList::InsertList(aux_list, TList::ListType::USER, aux_info)){
            printf("---- REGISTERED USERS LIST TO SAVE  ----\n");
            TList::PrintList(*aux_list);
            TList::SaveList(aux_list, user_list_dat, user_list_dat_path);
        }else{
            is_registered = false;
        }

        return is_registered;
    }

    void CloseFiles(){
        if(user_list_dat != nullptr){
            fclose(user_list_dat);
        }
    }

    void EmptyMemory(){
        FreeUserMemory(&empty_user);
    }
}