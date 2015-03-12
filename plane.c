/*
 * This file will mode a plane and will allow for the creation of planes.
 *
 * Author: Sean Facello
 * Verison: 2013/3/29
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "structs.h"
#include "object.h"
#include "material.h"
#include "veclib.h"
#include "plane.h"

#define VEC3 3
#define MISS -1
#define BUFF_SIZE 256
#define ZERO .00000000001
/* 
 * This function will initialize the plane and link it to an obj_t struct.
 *
 * parameters:
 * *in: a reference to the input file containing information about the scene.
 * objtype: the number the raytracer uses to fibure out the type of the object.
 */
obj_t* plane_init(FILE *in, int objtype) {
    obj_t *obj = NULL;
    plane_t *new = NULL;
    int pcount = 0;
    obj = object_init(in, objtype);
    new = (plane_t *)malloc(sizeof(plane_t));
    char buff[BUFF_SIZE];
    pcount = fscanf(in, "%lf %lf %lf", &(*new).normal[0], &(*new).normal[1], 
                    &(*new).normal[2]);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != VEC3){
        fprintf(stderr, "miss read normal");
        exit(1);
    }
    pcount = fscanf(in, "%lf %lf %lf", &(*new).point[0], &(*new).point[1], 
            &(*new).point[2]);
    fgets(buff, BUFF_SIZE, in);
    unitvec((*new).normal, (*new).normal);
    if(pcount != VEC3){
        fprintf(stderr, "miss read point");
        exit(1);
    }
    (*new).priv = NULL;
    (*obj).priv = new;
    return obj;
}

/*
 * This function will print the state of a given plane to a choosen destination
 *
 * parameters:
 * *out: the destination of the output
 * *obj: the obj of type plane you wish to print
 */
int plane_dump(FILE *out, obj_t *obj) {
    fprintf(out,"Dumping object of type Plane\n");
    plane_t *plane = NULL;
    material_dump(out, obj);
    plane = (plane_t *)(*obj).priv;
    fprintf(out, "Plane data\n");
    fprintf(out, "normal -   % 5.3lf   % 5.3lf   % 5.3lf\n", (*plane).normal[0]
            , (*plane).normal[1], (*plane).normal[2]);
    fprintf(out, "point  -   % 5.3lf   % 5.3lf   % 5.3lf\n", (*plane).point[0], 
            (*plane).point[1], (*plane).point[2]);
    fprintf(out, "\n");
    return 1;
}

/*
 * This funciton will determine if a given ray hits a given plane, and where.
 *
 * parameters:
 * base: the origin of the array
 * dir: a vector representation of the direction of the ray
 * obj: the object holding the plane that needs to be compared to 
 */
double hits_plane(double *base, double *dir, obj_t *obj){
    double unitdir[3];
    unitvec(dir, unitdir);
    plane_t *plane = (plane_t *)(*obj).priv;
    double dot1 = dot3(plane->normal, unitdir);
    if(dot1 != 0) {
        double th1 = (((dot3((*plane).normal, (*plane).point)) - 
                        (dot3((*plane).normal, base))) / dot1);
        if(th1 >= ZERO) {
            double h[3];
            scale3(th1, unitdir, unitdir); 
            sum(base, unitdir, h); 
            if(h[2] < ZERO) {
                (*obj).normal[0] = *(*plane).normal;
                (*obj).normal[1] = *((*plane).normal + 1);
                (*obj).normal[2] = *((*plane).normal + 2);
                (*obj).hitloc[0] = *h;
                (*obj).hitloc[1] = *(h + 1);
                (*obj).hitloc[2] = *(h + 2);
                return th1;
            } else {
                return MISS;
            }
        } else {
            return MISS;
        }
    } 
    return MISS;

}
        
