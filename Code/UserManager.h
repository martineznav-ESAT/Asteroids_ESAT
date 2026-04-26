#include <esat/window.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <esat/input.h>
#include <esat/draw.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef UserManager_H
#define UserManager_H

//Holds the game information needed globally, that may be needed to save and the Level/Screen management 
namespace UserManager{

    struct User{
        char* username = nullptr;
        char* password = nullptr;
        char* alias = nullptr;
        char* email = nullptr;
        char* name = nullptr;
        char* surname = nullptr;
        int day_dob;
        int month_dob;
        int year_dob;
        char* country = nullptr;
        char* province = nullptr;
        unsigned char credits;
        bool is_admin = false;
    };

    extern const unsigned char kDefaultStrL;
    extern const unsigned char kAliasStrL;
    extern const unsigned char kEmailStrL;
    extern const unsigned char kCreditsStrL;
    extern const unsigned char kDOBDayL;
    extern const unsigned char kDOBMonthL;
    extern const unsigned char kDOBYearL;

    extern FILE *user_list_dat;
    extern char *user_list_dat_path;
    

    //Whenever user_list is needed, create an aux variable such as:
    //TList::ListNode** aux_list = (TList::ListNode**) &user_list;
    //To work with the ListNode typing while aiming the same memory direction with a pointer anidation
    //It has been made this way due to the impossibility to include BTree.h in UserManager since it makes an "infinite include loop"
    extern void *user_list;
    extern User empty_user;

    void Init();

    User NewUser();

    void FreeUserMemory(User *user);

    void SaveUser(User user, FILE *dat_file);
    User LoadUser(FILE *dat_file);

    bool RegisterNewUser(User new_user);
    bool LoadRegisteredUsers();

    void CloseFiles();

    void EmptyMemory();
}

#endif