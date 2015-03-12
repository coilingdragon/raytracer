/*
 * This file includes funcitons for procedural planes, that is planes with a 
 * shader applied to them.
 *
 * Author: Sean Facello
 * Version: 2013/4/11
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structs.h"
#include "plane.h"
#include "object.h"
#include "veclib.h"

#define BUFF_SIZE 256
#define VEC_ONE 1

#define NUM_SHADERS sizeof(plane_shaders)/sizeof(void *)

/*
 * This function is for shader 0 of the pplanes
 *
 * parameters:
 * obj: the given object that the shader needs to act on
 * intensity: the calculated intensity of the light on a point
 */
void pplane0_amb(obj_t *obj, double *intensity) {
    plane_t* pln = (plane_t *)(*obj).priv;
    double vec[3];
    scale3(1, (*(*obj).material).ambient, intensity);
    diff3((*pln).point, (*obj).hitloc, vec);
    intensity[0] = (length3(vec) / 14) * intensity[0];
    intensity[1] = (length3(vec) / 3) * intensity[1];
    intensity[2] = (length3(vec) / 8) * intensity[2];
}

/*
 * This function is for shader 1 of pplane
 *
 * parameters:
 * obj: the given object that the shader is to act on
 * intensity: the calculated intensity for a point with out the shader
 */
void pplane1_amb(obj_t *obj, double *intensity) {
    double vec[3];
    plane_t *p = (plane_t *)(obj->priv);
    int isum;
    double sum;
    scale3(1, obj->material->ambient, intensity);
    diff3(p->point, obj->hitloc, vec);
    sum = 1000 + vec[0] * vec[1] * vec[1] / 100 + vec[0] * vec[1] / 100;
    isum = sum;
    if (isum & 1)
        intensity[0] = 0; // zap red 
    else
        intensity[2] = 0; // zap blue
}

/*
 * This funtion is for the shader 2 of pplane
 *
 * parameters:
 * ob: the given objec the shader acts on
 * intensity: the calculated intensity for a point on the object
 */
void pplane2_amb(obj_t *obj, double *intensity) {
    double vec[3];
    scale3(1, (*obj).hitloc, vec);
    intensity[0] = .84 * (vec[1] - vec[0]);
    intensity[1] = .54 * (vec[2] + vec[0] - (2 * vec[1]));
    intensity[2] = .73 * (vec[1] - vec[2]);
}

/*
 * This function is the 3rd shader of pplane
 *
 * parameter:
 * obj: teh given obj_t
 * intensity: the intensity
 */
void pplane3_amb(obj_t *obj, double *intensity) {
    double vec[3];
    scale3(1, (*obj).hitloc, vec);
    intensity[0] = sin((vec[0] + vec[1]));
    intensity[1] = sin((vec[0] + vec[1])) + (M_PI / 3);
    intensity[2] = sin((vec[0] + vec[1])) + (2 * M_PI / 3);
}


/*
 * this array stores the possible shaders for a given pplane
 */
static void (*plane_shaders[])(obj_t *obj, double *intensity) = 
{
    pplane0_amb,
    pplane1_amb,
    pplane2_amb,
    pplane3_amb
};

/*
 * This funciton simulates inheritance of object oriented languages, and 
 * initalizes a pplane. and returns the created obj
 *
 * parameters:
 * in: the source file being used
 * objtype: an indicator of the object type
 */
obj_t* pplane_init(FILE *in, int objtype) {
    obj_t *new = NULL;
    double dndx;
    int ndx;
    new = (obj_t *)plane_init(in, objtype);
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
    (*new).getamb = plane_shaders[ndx];
    return new;
}

/*
 * This function dumps informaiton about the pplane that is given in the 
 * parameters of the function.
 *
 * parameters:
 * *out: the destination to send the information.
 * *obj: the object to print
 */
int pplane_dump(FILE *out, obj_t *obj) {
    fprintf(out, "Dumping object of type Pplane\n");
    plane_dump(out, obj);
    return 1;
}
