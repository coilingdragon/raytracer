/*
 * This file holds specific functions relating to finite planes
 *
 * Author: Sean Facello
 * Version: 2013/4/21
 */

#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
#include "veclib.h"
#include "matrixlib.h"
#include "plane.h"

#define BUFF_SIZE 256
#define VEC3 3
#define VEC2 2
/*
 * This function will initialize a finite plane
 *
 * parameters:
 * in: the source file of the image
 * objtype: the identifier for the type of object
 */
obj_t* fplane_init(FILE *in, int objtype) {
    obj_t *obj = plane_init(in, objtype);
    plane_t *pln = (plane_t *)(*obj).priv;
    fplane_t *fpln = (fplane_t *)malloc(sizeof(fplane_t));
    (*pln).priv = fpln;
    char buff[BUFF_SIZE];
    int pcount = 0;
    pcount = fscanf(in, "%lf %lf %lf", &(*fpln).xdir[0], &(*fpln).xdir[1], 
                    &(*fpln).xdir[2]);
    fgets(buff, BUFF_SIZE, in); 
    if(pcount != VEC3){
        exit(1);
    }
    pcount = fscanf(in, "%lf %lf", &(*fpln).size[0], &(*fpln).size[1]);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != VEC2){
        exit(1);
    }
    proj3((*fpln).xdir, (*pln).normal, (*fpln).xdir);
    unitvec((*fpln).xdir, (*fpln).rotmat[0]);
    unitvec((*pln).normal, (*fpln).rotmat[2]);
    cross3((*fpln).rotmat[2], (*fpln).rotmat[0], (*fpln).rotmat[1]);
    return obj;
}

/*
 * This function will determine if a ray hits a given finite plane
 *
 * parameters: 
 * base: the origin of an array
 * dir: a vector representation of the direction of a ray
 * obj: the object holding a finite plane
 */
double hits_fplane(double *base, double *dir, obj_t *obj){
    unitvec(dir, dir);
    plane_t *pln = (plane_t *)(*obj).priv;
    fplane_t *fpln = (fplane_t *)(*pln).priv;
    double hit = hits_plane(base, dir, obj);
    if(hit < 0) {
        return hit;
    }
    double newhit[3];
    diff3((*pln).point, (*obj).hitloc, newhit);
    xform3((*fpln).rotmat, newhit, newhit);
    if(newhit[0] > (*fpln).size[0] || newhit[0] < 0.0) {
        return -1;
    } else if(newhit[1] > (*fpln).size[1] || newhit[1] < 0.0) {
        return -1;
    } 
    return hit;
}

/*
 * This function will dump information about a given finite plane
 *
 * parameters:
 * *out: the destination of the output
 * *obj: the given finite plane
 */
int fplane_dump(FILE *out, obj_t *obj){
    fprintf(out, "Dumping object of type Finite Plane\n");
    plane_dump(out, obj);
    plane_t *pln = (plane_t *)(*obj).priv;
    fplane_t *fpln = (fplane_t *)(*pln).priv;
    fprintf(out, "Finite Plane data\n");
    fprintf(out, "xdir  -   % 5.3lf % 5.3lf % 5.3lf\n", (*fpln).xdir[0], 
            (*fpln).xdir[1], (*fpln).xdir[2]);
    fprintf(out, "size -   % 5.3lf % 5.3lf\n", (*fpln).size[0], 
            (*fpln).size[1]);
    fprintf(out, "\n");
    return 1;
}
