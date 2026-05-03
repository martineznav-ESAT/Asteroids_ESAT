#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "./PowerUps.h"

#include "../CustomLibs/PolyLibJMATH.h"
#include "../CustomLibs/Utils.h"


namespace PowerUps{
    JMATH::Vec3 **friendly_fire_coords = nullptr;
    int last_pu_id = -1;

    void InitFriendlyFireIcon(){
        friendly_fire_coords = (JMATH::Vec3**) malloc(sizeof(JMATH::Vec3*)*PU_Type::TOTAL_PU_TYPES);

        // Ship Bottom Half figure
        *(friendly_fire_coords+0) = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3)*FFIconsVertices::V_ICON_1);
        *(*(friendly_fire_coords+0)+0) =    {-0.6f, -0.3f, 1.0f};     // 1
        *(*(friendly_fire_coords+0)+1) =    {-1.0f, -0.6f, 1.0f};     // 2
        *(*(friendly_fire_coords+0)+2) =    {0.0f, -0.3f, 1.0f};      // 3
        *(*(friendly_fire_coords+0)+3) =    {-0.2f, -0.2f, 1.0f};     // 4
        *(*(friendly_fire_coords+0)+4) =    {0.2f, 0.0f, 1.0f};      // 5
        *(*(friendly_fire_coords+0)+5) =    {-0.2f, 0.2f, 1.0f};      // 6
        *(*(friendly_fire_coords+0)+6) =    {0.0f, 0.3f, 1.0f};       // 7
        *(*(friendly_fire_coords+0)+7) =    {-1.0f, 0.6f, 1.0f};      // 8
        *(*(friendly_fire_coords+0)+8) =    {-0.6f, 0.3f, 1.0f};      // 9

        
        // Ship Top Half figure
        *(friendly_fire_coords+1) = (JMATH::Vec3*) malloc(sizeof(JMATH::Vec3)*FFIconsVertices::V_ICON_2);
        *(*(friendly_fire_coords+1)+0) =    {0.2f, 0.0f, 1.0f};      // 1
        *(*(friendly_fire_coords+1)+1) =    {-0.2f, -0.2f, 1.0f};     // 2
        *(*(friendly_fire_coords+1)+2) =    {0.0f, -0.3f, 1.0f};      // 3
        *(*(friendly_fire_coords+1)+3) =    {1.0f, 0.0f, 1.0f};       // 4
        *(*(friendly_fire_coords+1)+4) =    {0.0f, 0.3f, 1.0f};       // 5
        *(*(friendly_fire_coords+1)+5) =    {-0.2f, 0.2f, 1.0f};      // 6

    }

    void Init(){
        InitFriendlyFireIcon();
    }

    void BuildFriendlyFireIcons(PowerUp *p_up){
        JMATH::Mat3 tr_aux;
        p_up->icon_figures = (PolyLibJMATH::Poly*) malloc(sizeof(PolyLibJMATH::Poly)*FFIcons::TOTAL_ICONS);

        PolyLibJMATH::InitPoly(
            (p_up->icon_figures+FFIcons::ICON_1),
            FFIconsVertices::V_ICON_1,
            *(friendly_fire_coords+((int)FFIcons::ICON_1)),
            p_up->base_figure.transform.scale,
            p_up->base_figure.transform.rotation,
            p_up->base_figure.transform.translation,
            p_up->base_figure.color,
            {-0.1f,0.1f}
        );

        

        PolyLibJMATH::InitPoly(
            (p_up->icon_figures+FFIcons::ICON_2),
            FFIconsVertices::V_ICON_2,
            *(friendly_fire_coords+((int)FFIcons::ICON_2)),
            p_up->base_figure.transform.scale,
            p_up->base_figure.transform.rotation,
            p_up->base_figure.transform.translation,
            p_up->base_figure.color,
            {0.5f,0.3f}
        );

        //Rotates base local_coords permanently to draw the Bottom Half figure icon properly
        tr_aux = JMATH::Mat3Identity();
        tr_aux = JMATH::Mat3MultMat3(JMATH::Mat3Rotate(JMATH::DegreesToRadians(10)), tr_aux);
        for(int i = 0; i < (p_up->icon_figures+(int)FFIcons::ICON_1)->t_vertices; i++){
            *(((p_up->icon_figures+(int)FFIcons::ICON_1)->local_coords)+i) = JMATH::Mat3MultVec3(tr_aux, {(((p_up->icon_figures+(int)FFIcons::ICON_1)->local_coords)+i)->x, (((p_up->icon_figures+(int)FFIcons::ICON_1)->local_coords)+i)->y, 1.0f});
            // printf("Coord Update local_coords ICON %d: %f - %f\n",i, (*((p_up->icon_figures+(int)FFIcons::ICON_1)->local_coords+i)).x, (*((p_up->icon_figures+(int)FFIcons::ICON_1)->local_coords+i)).y);
        }

        //Rotates base local_coords permanently to draw the Top Half figure icon properly
        tr_aux = JMATH::Mat3Identity();
        tr_aux = JMATH::Mat3MultMat3(JMATH::Mat3Rotate(JMATH::DegreesToRadians(-10)), tr_aux);
        for(int i = 0; i < (p_up->icon_figures+(int)FFIcons::ICON_2)->t_vertices; i++){
            *(((p_up->icon_figures+(int)FFIcons::ICON_2)->local_coords)+i) = JMATH::Mat3MultVec3(tr_aux, {(((p_up->icon_figures+(int)FFIcons::ICON_2)->local_coords)+i)->x, (((p_up->icon_figures+(int)FFIcons::ICON_2)->local_coords)+i)->y, 1.0f});
            // printf("Coord Update local_coords ICON %d: %f - %f\n",i, (*((p_up->icon_figures+(int)FFIcons::ICON_2)->local_coords+i)).x, (*((p_up->icon_figures+(int)FFIcons::ICON_2)->local_coords+i)).y);
        }
    }

    void BuildIcons(PowerUp *p_up){
        switch(p_up->type){
            case FRIENDLY_FIRE:
                BuildFriendlyFireIcons(p_up);
            break;

            default:
                p_up->icon_figures = nullptr;
            break;
        }
    }

    void UpdateIcon(PowerUp *p_up){
        PolyLibJMATH::UpdatePoly(&(p_up->base_figure));
        switch (p_up->type){
            case PU_Type::FRIENDLY_FIRE:
                for (int i = 0; i < FFIcons::TOTAL_ICONS; i++){
                    (p_up->icon_figures+i)->transform.scale = JMATH::Vec2Scale(p_up->base_figure.transform.scale,0.5f);
                    (p_up->icon_figures+i)->transform.rotation = p_up->base_figure.transform.rotation;
                    (p_up->icon_figures+i)->transform.translation = p_up->base_figure.transform.translation;
                    PolyLibJMATH::UpdatePoly((p_up->icon_figures+i));
                }
            break;
        }
    }

    PowerUp NewPowerUp(PU_Type type){
        PowerUp new_powerUp;
        new_powerUp.id = ++last_pu_id;
        new_powerUp.type = type;
        
        PolyLibJMATH::InitPoly(
            &new_powerUp.base_figure,
            4,
            {22.5f,22.5f},
            -135.0f,
            {Utils::kWindowWidth*0.5f, Utils::kWindowHeight*0.5f},
            {25,25,25},
            {0.0f,0.0f}
        );
        
        BuildIcons(&new_powerUp);

        new_powerUp.duration_lt = 8000;
        new_powerUp.duration_ltc = 0;

        UpdateIcon(&new_powerUp);

        // printf("NEW POWER UP CREATED %d\n",new_powerUp.id);
        return new_powerUp;
    }

    PowerUp NewPowerUpAt(PU_Type type, JMATH::Vec2 position){
        PowerUp new_powerUp;
        new_powerUp.id = ++last_pu_id;
        new_powerUp.type = type;
        
        PolyLibJMATH::InitPoly(
            &new_powerUp.base_figure,
            4,
            {35,35},
            -135.0f,
            {position.x, position.y},
            {25,25,25},
            {0.0f,0.0f}
        );
        
        BuildIcons(&new_powerUp);

        new_powerUp.duration_lt = 8000;
        new_powerUp.duration_ltc = 0;

        UpdateIcon(&new_powerUp);

        // printf("NEW POWER UP CREATED %d\n",new_powerUp.id);
        return new_powerUp;
    }

    bool IsPowerUpActive(PowerUp powerUp){
        return powerUp.duration_ltc < powerUp.duration_lt;
    }

    bool IsPowerUpBlinking(PowerUp powerUp){
        return (powerUp.duration_ltc >= powerUp.duration_lt-3000);
    }


    //Scales the given PowerUp for the amount of the frame equivalent calculated value of a percentage during an specified amount of miliseconds.
    //It scales up or down based on the is_growing parameter
    void ScalePowerUpFramePercentageEquivalent(PowerUp *powerUp, float percentage_decimals, int time_ms, bool is_gowing){
        if(is_gowing){
            powerUp->base_figure.transform.scale = JMATH::Vec2Scale(powerUp->base_figure.transform.scale, 1 + (1.0f/((time_ms/1000.0f)*Utils::kFPS))*percentage_decimals);
        }else{
            powerUp->base_figure.transform.scale = JMATH::Vec2Scale(powerUp->base_figure.transform.scale, 1 - (1.0f/((time_ms/1000.0f)*Utils::kFPS))*percentage_decimals);
        }
    }

    void UpdatePowerUp(PowerUp *powerUp){
        if(IsPowerUpActive(*powerUp)){
            //Grows for 0.5s and Shrinks for 0.5s
            if((((int)(powerUp->duration_ltc))%1000) < 500){
                ScalePowerUpFramePercentageEquivalent(powerUp, 0.33f, 500, true);
            }else{
                ScalePowerUpFramePercentageEquivalent(powerUp, 0.33f, 500, false);
            }

            UpdateIcon(powerUp);

            powerUp->duration_ltc += 1000/Utils::kFPS;
            // printf("pu_duration_ltc %f\n", powerUp->duration_ltc);
        }
    }

    void DrawIcon(PowerUp powerUp){
        PolyLibJMATH::DrawPoly(powerUp.base_figure, true, {255,255,255});

        switch (powerUp.type){
            case PU_Type::FRIENDLY_FIRE:
                for (int i = 0; i < FFIcons::TOTAL_ICONS; i++){
                    PolyLibJMATH::DrawPoly(*(powerUp.icon_figures+i), false, {255,255,255});
                }
            break;
        }
    }

    void DrawPowerUp(PowerUp powerUp){
        if(IsPowerUpActive(powerUp)){
            // printf("DRAWING POWER UP %d\n",powerUp.id);
            if(IsPowerUpBlinking(powerUp)){
                // printf("POWER UP %d BLINKING\n",powerUp.id);
                if((((int)powerUp.duration_ltc) % 300) < 150){
                    DrawIcon(powerUp);
                }
            }else{
                DrawIcon(powerUp);
            }
        }
    }

    void EmptyPowerUpMemory(PowerUp *powerUp){
        PolyLibJMATH::EmptyPolyMemory(&(powerUp->base_figure));
        switch(powerUp->type){
            case FRIENDLY_FIRE:
                for (int i = 0; i < FFIcons::TOTAL_ICONS; i++){
                    PolyLibJMATH::EmptyPolyMemory((powerUp->icon_figures+i));
                }
                free(powerUp->icon_figures);
            break;
        }
    }

    void EmptyMemory(){
        //Memory release code structure with multiple Power Up Types in mind
        for (int type = 0; type < (int)PU_Type::TOTAL_PU_TYPES; type++){
            switch ((PU_Type)type){
                case PU_Type::FRIENDLY_FIRE:
                    for (int i = 0; i < FFIcons::TOTAL_ICONS; i+=2){
                        free(*(friendly_fire_coords+i));
                    }
                    free(friendly_fire_coords);
                break;
            }
        }
    }
}
