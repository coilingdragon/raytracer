/*
 * This file defines the function for defining an object, and specifies the 
 * type of the object.
 *
 * Author: Sean Facello
 * Version: 2013/3/12
 */

#include <stdio.h>
#include <stdlib.h>
#include "objid.h"
#include "structs.h"
#include "object.h"
#include "material.h"

#define VEC3 3

/*
 * This function will also set the type of material the object is by calling 
 * the material_init function. This function is to be called by each individual 
 * shape type.
 *
 * parameters:
 * *in: the place the function is to read from.
 * objtype: an identfier for the object type in the inside of the object
 */
obj_t* object_init(FILE *in, int objtype) {
    obj_t *obj = NULL;
    obj = (obj_t *)malloc(sizeof(obj_t));
    (*obj).objtype = objtype;
    if((*obj).objtype > LAST_LIGHT) {
        (*obj).material = material_init(in);
    } else {
        (*obj).material = NULL;
    }
    int i = 0;
    while(i < VEC3) {
        (*obj).hitloc[i] = 0;
        (*obj).normal[i] = 0;
        i++;
    }
    return obj;
}
    


