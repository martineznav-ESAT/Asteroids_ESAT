#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "./TList.h"

namespace TList{
    
    TList* ListCreate(){
        return nullptr;
    }

    bool IsEmptyList(TList **list){
        return (*list == nullptr);
    }

    //Añade un valor al inicio de la list
    void ListAdd(TList **list, int v){
        TList *aux;
        // printf("ListAdd\n");
        aux = (TList*) malloc(sizeof(TList));
        aux->info = v;
        aux->next = *list;
        aux->prev = nullptr;

        if(*list != nullptr){
            (*list)->prev = aux;
        }

        *list = aux;
    }    

    //Devuelve en consola los valores de la list
    void ListPrint(TList *list){
        for(TList *p = list; p!=nullptr; p = p->next){
            printf("%02d ",p->info);
        }
    }

    //Devuelve en consola los valores de la list invertidos de orden
    void ListPrintReverted(TList *list){
        TList *aux = nullptr;
        //Recorro en primera instancia para asignar a aux el valor del nodo de la cola
        for(aux = list; aux->next != nullptr; aux = aux->next);

        //Recorro en segunda instancia a la inversa desde aux mostrando por consola la info
        while(aux != nullptr){
            printf("%02d ",aux->info);
            aux = aux->prev;
        }
    }

    //Devuelve el número de datos que contiene la list
    int ListLength(TList* list){
        int res = 0;

        for(TList *p = list; p!=nullptr; p = p->next){
            res++;
        }

        return res;
    }

    //Devuelve nodo buscando por el valor a encontrar
    TList* ListFind(TList *list, int info){
        TList *aux = nullptr;

        // printf("\n-----Buscando %d\n",info);

        for(aux = list; aux != nullptr && aux->info != info ; aux = aux->next);

        if(info == 25 || info == 4){
            if(aux == nullptr){
                printf("NO ENCONTRADO -----\n");
            }else{
                printf("ENCONTRADO %p -----\n",aux);
            }
        }
        

        return aux;
    }

    //Extrae de la list el nodo pasado por parametro y lo devuelve desvinculado
    TList* ListExtract(TList **elemento_list){
        TList *aux_act = *elemento_list;
        TList *aux_next = (*elemento_list)->next;
        TList *aux_prev = (*elemento_list)->prev;

        if(aux_prev != nullptr){
            aux_prev->next = aux_next;
        }
        if(aux_next != nullptr){
            aux_next->prev = aux_prev;
        }

        (*elemento_list)->next = nullptr;
        (*elemento_list)->prev = nullptr;

        if(aux_prev == nullptr){
            *elemento_list = aux_next;
        }
        
        return aux_act;
    }

    // //Elimina el nodo con el valor info introducido
    void ListRemove(TList **list, int info){
        // printf("EliminaEnLista\n");
        
        TList *aux = ListFind(*list, info);

        //Comprueba si existe
        if(IsEmptyList(&aux)){
            printf(" >>> No encontrado en list\n");
        }else{
            //Si existe lo extrae de la list;
            if(aux == *list){
                aux = ListExtract(list);
            }else{
                aux = ListExtract(&aux);
            }

            //Y Libera la memoria de la extraida que se quiere eliminar, eliminandola por completo
            free(aux);
        }
    }

    void EmptyList(TList **list){
        for(TList *act = *list; !IsEmptyList(list); act = *list){
            ListExtract(list);
        }
    }
}