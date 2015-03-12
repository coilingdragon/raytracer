/*
 * This file will model a linked list using obj_t structs.
 *
 * Author: Sean Facello
 * Version: 2013/3/28
 */

#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
#include "list.h"
#include "object.h"
#include "objid.h"

/*
 * This function will create a new list_t and set the pointers to NULL
 */
list_t* list_init(void) {
    list_t *list = NULL;
    list = (list_t *)malloc(sizeof(list_t));
    (*list).first = NULL;
    (*list).last = NULL;
    return list;
}

/*
 * This function will add a new obj to the end of a given list
 *
 * parameters:
 * list: the list that the object needs to be added to 
 * obj: the object to be added to the list
 */
void list_add(list_t *list, obj_t *obj) {
    if((*list).first != NULL) {
        (*((*list).last)).next = obj;
        (*list).last = obj;
    } else {
        (*list).first = obj;
        (*list).last = obj;
    }
}

/*
 * This function will return the nth item of the list, given that it is a 
 * number greater than 0 and is less than the length of the list, if either of 
 * these cases occurs, the funcion will return NULL.
 * 
 * parameters:
 * list: the list you are trying to access.
 * num: the element you are trying to retrive.
 */
obj_t* list_get(list_t *list, int num) {
    obj_t *obj = NULL;
    if(num < 0) {
        return obj;
    } else {
        obj = (*list).first;
        int i = 0;
        while(i < num && obj != NULL){
            obj = (*obj).next;
            i++;
        }
        i++;
        if(i < num) {
            obj = NULL;
            return obj;
        }
    }
    return obj;
}

/*
 * This funcition will free all of the memory associated with the given list.
 *
 * parameteres:
 * list: the list that needs to be cleared
 */
void free_list(list_t list){
    obj_t *obj = NULL;
    obj_t *obj2 = NULL;
    if(list.first != NULL) {
        obj2 = list.first;
        obj = (*obj2).next;
        while(obj != NULL){
            if((*obj2).objtype == LIGHT){
                light_t *lht = (light_t *)(*obj2).priv;
                if((*lht).priv != NULL){
                    free((*lht).priv);
                }
            }
            if((*obj2).objtype == TILED_PLANE){
                plane_t *pln = (plane_t *)(*obj2).priv;
                tplane_t *tpln = (tplane_t *)(*pln).priv;
                free((*tpln).background);
                free((*pln).priv);
            }
            if((*obj2).objtype == FINITE_PLANE){
                plane_t *pln = (plane_t *)(*obj2).priv;
                free((*pln).priv);
            } 
            free((*obj2).priv);
            free((*obj2).material);
            free(obj2);
            obj2 = obj;
            obj = (*obj2).next;
        }
        if((*obj2).objtype == SPOTLIGHT){
            light_t *lht = (light_t *)(*obj2).priv;
            if((*lht).priv != NULL){
                free((*lht).priv);
            }
        }
        if((*obj2).objtype == TILED_PLANE){
            plane_t *pln = (plane_t *)(*obj2).priv;
            tplane_t *tpln = (tplane_t *)(*pln).priv;
            free((*tpln).background);
            free((*pln).priv);
        }
        if((*obj2).objtype == FINITE_PLANE){
            plane_t *pln = (plane_t *)(*obj2).priv;
            free((*pln).priv);
        }
        free((*obj2).priv);
        free((*obj2).material);
        free(obj2);
    }
}
    
