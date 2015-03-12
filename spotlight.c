/*
 * This file includes functions that help model a spotlight, which is a 
 * specilaized form of light.
 *
 * Author: Sean Facello
 * Version: 2013/4/23
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "structs.h"
#include "light.h"
#include "veclib.h"

#define VEC3 3
#define VEC1 1
#define BUFF_SIZE 256
#define DEGREE_TRANS 180
/*
 * This funciton will initialize a spotlight using the helper function of 
 * light_t.
 *
 * parameters:
 * in: the source file for the image
 * objtype: a reference to the type of object
 */
obj_t* spotlight_init(FILE *in, int objtype){
    obj_t *obj = light_init(in, objtype);
    light_t *light = (light_t *)(*obj).priv;
    char buff[BUFF_SIZE];
    spotlight_t *spot = malloc(sizeof(spotlight_t));
    (*light).priv = spot;
    int pcount = 0;
    pcount = fscanf(in, "%lf %lf %lf", &(*spot).direction[0], 
                    &(*spot).direction[1], &(*spot).direction[2]);
    if(pcount != VEC3){
        exit(1);
    }
    fgets(buff, BUFF_SIZE, in);
    diff3((*spot).direction, (*light).point, (*spot).direction);
    unitvec((*spot).direction, (*spot).direction);
    pcount = fscanf(in, "%lf", &(*spot).theta);
    if(pcount != VEC1){
        exit(1);
    }
    double rads = (*spot).theta * (M_PI / DEGREE_TRANS);
    (*spot).costheta = cos(rads);
    return obj;
}

/*
 * This function will dump the data abut a given spotlight
 *
 * parameters:
 * out: the destination of the infromation regarding the light
 * obj: the object that holds teh spotlight
 */
int spotlight_dump(FILE *out, obj_t *obj){
    fprintf(out, "Dumping object of type spotlight\n");
    light_t *light = (light_t *)(*obj).priv;
    light_dump(out, obj);
    spotlight_t *spot = (spotlight_t *)(*light).priv;
    fprintf(out, "direction - % 5.3lf  % 5.3lf  % 5.3lf\n", 
            (*spot).direction[0], (*spot).direction[1], (*spot).direction[2]);
    fprintf(out, "theta     - % 5.3lf\n\n", (*spot).theta);
    return 1;
}

/*
 * This is a specific test to determine if a location is illuminated by a 
 * given spotlight
 *
 * parameters:
 * obj: the spotlight object
 * vec: the vector from a hitpoint to the point representing the light
 *
 * returns:
 * 1: if the spotlight illuminates the point 
 * 0: if the spotlight does not illuminate the point
 */
int check_spot(obj_t *obj, double *vec){
    double stat;
    unitvec(vec, vec);
    light_t *light = (light_t *)(*obj).priv;
    spotlight_t *spot = (spotlight_t *)(*light).priv;
    stat = dot3((*spot).direction, vec);
    if(stat > (*spot).costheta){
        return 1;
    }
    return 0;
}
