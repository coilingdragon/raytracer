/*
 * This file will model a material for an object in the ray tracer.
 *
 * Author: Sean Facello
 * Version: 2013/3/20
 */

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "error.h"
#include "material.h"
#include "veclib.h"
#include "model.h"

#define COLOR3 3
#define BUFF_SIZE 256

/*
 * This function will dump information about the material an object is made of.
 * This function is called from the *next object's dump function.
 *
 * parameters:
 * *out: the place that the function will print to 
 * *obj: a reference to the obj_t that needs to be printed.
 */
int material_dump(FILE *out, obj_t *obj) {  
    fprintf(out, "Material data -\n");
    fprintf(out, "Ambient  -   %5.3lf  %5.3lf  %5.3lf\n", 
            (*(*obj).material).ambient[0], (*(*obj).material).ambient[1], 
            (*(*obj).material).ambient[2]);
    fprintf(out, "Diffuse  -   %5.3lf  %5.3lf  %5.3lf\n", 
            (*(*obj).material).diffuse[0], (*(*obj).material).diffuse[1], 
            (*(*obj).material).diffuse[2]);
    fprintf(out, "Specular -   %5.3lf  %5.3lf  %5.3lf\n", 
            (*(*obj).material).specular[0], (*(*obj).material).specular[1], 
            (*(*obj).material).specular[2]);
    fprintf(out, "Trans    -   %5.3lf\n", (*(*obj).material).transparency);
    fprintf(out, "\n");
    return 1;
}

/*
 * This function will initialize the material part of an object. 
 * parameters:
 * *in the place that the funtion is to read from 
 */
material_t* material_init(FILE *in){
    material_t *mat = NULL;
    mat = (material_t *)malloc(sizeof(material_t));
    char buff[BUFF_SIZE];
    int pcount = 0;
    pcount = fscanf(in, "%lf %lf %lf", &(*mat).ambient[0], &(*mat).ambient[1], 
                    &(*mat).ambient[2]);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != COLOR3){
        exit(1);
    }
    pcount = fscanf(in, "%lf %lf %lf", &(*mat).diffuse[0], &(*mat).diffuse[1], 
                    &(*mat).diffuse[2]);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != COLOR3){
        exit(3);
    }
    pcount = fscanf(in, "%lf %lf %lf", &(*mat).specular[0], &(*mat).specular[1], 
                    &(*mat).specular[2]);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != COLOR3){
        exit(1);
    }
    if(trans == 1){
        pcount = fscanf(in, "%lf", &(*mat).transparency);
        if(pcount != 1){
            fprintf(stderr, "no transparency\n");
            exit(1);
        }
        fgets(buff, BUFF_SIZE, in);
    }
    if(refrac == 1){
        pcount = fscanf(in, "%lf", &(*mat).refraction);
        if(pcount != 1){
            fprintf(stderr, "no refraction\n");
            exit(1);
        }
        fgets(buff, BUFF_SIZE, in);
    }
    return mat;
}

/*
 * This function will set intensity to the diffuse of the object
 *
 * parameters:
 * *obj: the object holding the material with the diffuse value
 * *intensity: the current intensity vector for the hit location
 */
void getamb(obj_t *obj, double *intensity){
    sum((*(*obj).material).ambient, intensity, intensity);
}

/*
 * This funciton will get the diffuse values for the given object
 *
 * parameters: 
 * *obj: the object holding the diffuse 
 * *intensity: the intensity of the light vector
 */
void getdiff(obj_t *obj, double *intensity){
    scale3(1, (*(*obj).material).diffuse, intensity);
}

/*
 * This fuction will get the specular values for the given object
 *
 * parameters:
 * *obj: the object holding the specular
 * *spec: the array to hold the specular values
 */
void getspec(obj_t *obj, double *spec){
    scale3(1, (*(*obj).material).specular, spec);
}
