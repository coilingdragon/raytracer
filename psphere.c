/*
 * This file includes funcitons for procedural sphere, that is spheres with a 
 * shader applied to them.
 *
 * Author: Sean Facello
 * Version: 2013/5/3
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structs.h"
#include "sphere.h"
#include "object.h"
#include "veclib.h"

#define BUFF_SIZE 256
#define VEC_ONE 1

#define NUM_SHADERS sizeof(sphere_shaders)/sizeof(void *)

/*
 * This function is for shader 0 of the pspheres
 *
 * parameters:
 * obj: the given object that the shader needs to act on
 * intensity: the calculated intensity of the light on a point
 */
void psphere0_amb(obj_t *obj, double *intensity) {
    //sphere_t* sp = (sphere_t *)(*obj).priv;
    double vec[3];
    scale3(1, (*(*obj).material).ambient, intensity);
    scale3(1, (*obj).hitloc, vec);
    if(sin(vec[1]) < 0){
        intensity[0] = 1;
        intensity[1] = 1;
        intensity[2] = 1;
    } else {
        intensity[0] = 1;
        intensity[1] = 0;
        intensity[2] = 0;
    }
}

/*
 * This funtion is for the shader 2 of psphere
 *
 * parameters:
 * ob: the given objec the shader acts on
 * intensity: the calculated intensity for a point on the object
 */
void psphere1_amb(obj_t *obj, double *intensity) {
    double vec[3];
    scale3(1, (*obj).hitloc, vec);
    intensity[0] = .84 * (vec[1] - vec[0]);
    intensity[1] = .54 * (vec[2] + vec[0] - (2 * vec[1]));
    intensity[2] = .73 * (vec[1] - vec[2]);
}

/*
 * This function is the 3rd shader of psphere
 *
 * parameter:
 * obj: teh given obj_t
 * intensity: the intensity
 */
void psphere2_amb(obj_t *obj, double *intensity) {
    double vec[3];
    scale3(1, (*obj).hitloc, vec);
    intensity[0] = sin((vec[0] + vec[1]));
    intensity[1] = sin((vec[0] + vec[1])) + (M_PI / 3);
    intensity[2] = sin((vec[0] + vec[1])) + (2 * M_PI / 3);
}


/*
 * this array stores the possible shaders for a given psphere
 */
static void (*sphere_shaders[])(obj_t *obj, double *intensity) = 
{
    psphere0_amb,
    psphere1_amb,
    psphere2_amb,
};

/*
 * This funciton simulates inheritance of object oriented languages, and 
 * initalizes a psphere. and returns the created obj
 *
 * parameters:
 * in: the source file being used
 * objtype: an indicator of the object type
 */
obj_t* psphere_init(FILE *in, int objtype) {
    obj_t *new = NULL;
    double dndx;
    int ndx;
    new = (obj_t *)sphere_init(in, objtype);
    char buff[BUFF_SIZE];
    ndx = fscanf(in, "%lf", &dndx);
    fgets(buff, BUFF_SIZE, in);
    if(ndx != VEC_ONE) {
        exit(1);
    }
    ndx = dndx;
    if(ndx > NUM_SHADERS) {
        exit(1);
    }
    (*new).getamb = sphere_shaders[ndx];
    return new;
}

/*
 * This function dumps informaiton about the psphere that is given in the 
 * parameters of the function.
 *
 * parameters:
 * *out: the destination to send the information.
 * *obj: the object to print
 */
int psphere_dump(FILE *out, obj_t *obj) {
    fprintf(out, "Dumping object of type Psphere\n");
    sphere_dump(out, obj);
    return 1;
}
