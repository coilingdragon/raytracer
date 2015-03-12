/*
 * This structure models a light, and provides functions for the creation of a
 * light object.
 *
 * Author: Sean Facello
 * Version: 2013/4/14
 */

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "object.h"

#define BUFF_SIZE 256
#define VEC_THREE 3

/*
 * This funciton will create a light object, and return the created obj_t 
 *
 * parameters:
 * *in: the file to be read from 
 * objtype: the number that references a light object.
 */
obj_t* light_init(FILE *in, int objtype) {
    obj_t *obj = NULL;
    char buff[BUFF_SIZE];
    light_t *new = NULL;
    int pcount = 0;
    obj = object_init(in, objtype);
    new = (light_t *)malloc(sizeof(light_t));
    pcount = fscanf(in, "%lf %lf %lf", &(*obj).emissivity[0], 
                    &(*obj).emissivity[1], &(*obj).emissivity[2]);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != VEC_THREE) {
        fprintf(stderr, "failed to read in emissivity\n");
        exit(1);
    }
    pcount = fscanf(in, "%lf %lf %lf", &(*new).point[0], &(*new).point[1], 
                    &(*new).point[2]);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != VEC_THREE){
        fprintf(stderr, "failed to read in point\n");
        exit(1);
    }
    (*new).priv = NULL;
    (*obj).priv = new;
    return obj;
}

/*
 * This funciton will dump the relevant information regarding the given light 
 * object.
 *
 * parameters:
 * *out: the destination of the information
 * *obj: the light object to be printed
 */
int light_dump(FILE *out, obj_t *obj) {
    fprintf(out, "Dumping object of type light\n");
    light_t *light = NULL;
    light = (light_t *)(*obj).priv;
    fprintf(out, "Light data\n");
    fprintf(out, "emissivity -   % 5.3lf   % 5.3lf   % 5.3lf\n", 
            (*obj).emissivity[0], (*obj).emissivity[1], (*obj).emissivity[2]);
    fprintf(out, "point      -   % 5.3lf   % 5.3lf   % 5.3lf\n", 
            (*light).point[0], (*light).point[1], (*light).point[2]);
    fprintf(out, "\n");
    return 1;
}


