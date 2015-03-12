/*
 * This file models tiled planes, which are based off of infinite planes and 
 * finite planes
 * 
 * Author: Sean Facello
 * Version: 2013/4/22
 */

#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
#include "veclib.h"
#include "matrixlib.h"
#include "plane.h"
#include "material.h"

#define BUFF_SIZE 256
#define VEC_THREE 3
#define VEC_TWO 2
/*
 * This function will initilize a tiled plane
 *
 * parameters:
 * in: the source file of the image
 * objtype: the identifier for the type of object, in this case tplane
 */
obj_t* tplane_init(FILE *in, int objtype) {
    obj_t *obj = plane_init(in, objtype);
    plane_t *pln = (plane_t *)(*obj).priv;
    tplane_t *tpln = malloc(sizeof(tplane_t));
    (*pln).priv = tpln;
    char buff[BUFF_SIZE];
    int pcount = 0;
    pcount = fscanf(in, "%lf %lf %lf", &(*tpln).xdir[0], &(*tpln).xdir[1],
                    &(*tpln).xdir[2]);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != VEC_THREE){
        exit(1);
    }
    pcount = fscanf(in, "%lf %lf", &(*tpln).size[0], &(*tpln).size[1]);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != VEC_TWO){
        exit(1);
    }
    unitvec((*tpln).xdir, (*tpln).xdir);
    unitvec((*pln).normal, (*pln).normal);
    proj3((*tpln).xdir, (*pln).normal, (*tpln).xdir);
    scale3(1, (*tpln).xdir, (*tpln).rotmat[0]);
    scale3(1, (*pln).normal, (*tpln).rotmat[2]);
    cross3((*tpln).rotmat[2], (*tpln).rotmat[0], (*tpln).rotmat[1]);
    (*tpln).background = material_init(in);
    return obj;
}

/*
 * This function will dump information regarding a given tiled plane
 *
 * parameters:
 * out: the destination for the information
 * obj: the given tiled plane object
 */
int tplane_dump(FILE *out, obj_t *obj){
    fprintf(out, "Dumping object of type Tiled Plane\n");
    plane_dump(out, obj);
    plane_t *pln = (plane_t *)(*obj).priv;
    tplane_t *tpln = (tplane_t *)(*pln).priv;
    fprintf(out, "Tiled Plane data\n");
    fprintf(out, "xdir  - % 5.3lf % 5.3lf % 5.3lf\n", (*tpln).xdir[0], 
            (*tpln).xdir[1], (*tpln).xdir[2]);
    fprintf(out, "size  - % 5.3lf % 5.3lf\n\n", (*tpln).size[0], (*tpln).size[1]);
    fprintf(out, "Background data -\n");
    fprintf(out, "Ambient  -   %5.3lf  %5.3lf  %5.3lf\n", 
            (*(*tpln).background).ambient[0], (*(*tpln).background).ambient[1], 
            (*(*tpln).background).ambient[2]);
    fprintf(out, "Diffuse  -   %5.3lf  %5.3lf  %5.3lf\n", 
            (*(*tpln).background).diffuse[0], (*(*tpln).background).diffuse[1], 
            (*(*tpln).background).diffuse[2]);
    fprintf(out, "Specular -   %5.3lf  %5.3lf  %5.3lf\n", 
            (*(*tpln).background).specular[0], (*(*tpln).background).specular[1], 
            (*(*tpln).background).specular[2]);
    fprintf(out, "\n");
    return 1;
}

/*
 * This function determines if a point is on a foreground or background tile
 *
 * parameters:
 * obj: the tiled plane
 *
 * returns: 1 for foreground
 *          0 for background
 */
int tp_select(obj_t *obj){
    plane_t *pln = (plane_t *)(*obj).priv;
    tplane_t *tpln = (tplane_t *)(*pln).priv;
    double newhit[3];
    diff3((*pln).point, (*obj).hitloc, newhit);
    xform3((*tpln).rotmat, newhit, newhit);
    int relx = (int) 10000 + (newhit[0] / (*tpln).size[0]);
    int rely = (int) 10000 + (newhit[1] / (*tpln).size[1]);
    int num = (relx + rely) % 2;
    if(num == 0) {
        return 0;
    }
    return 1;
}

/*
 * This function will get the ambient of the appropriate amibent for the 
 * hit point
 *
 * parameters:
 * obj: the object holding the tplane
 * intensity: the current intesnity
 */
void tp_amb(obj_t *obj, double *intensity){
    int indicate = tp_select(obj);
    if(indicate == 1){
        sum((*(*obj).material).ambient, intensity, intensity);
    } else {
        plane_t *pln = (plane_t *)(*obj).priv;
        tplane_t *tp = (tplane_t *)(*pln).priv;
        sum((*(*tp).background).ambient, intensity, intensity);
    }
}

/*
 * This function will get the diffuse for the appropriate plane according to 
 * the hit point
 *
 * parameters:
 * obj: the object holding the tplane
 * intensity: the  current intensity
 */
void tp_diff(obj_t *obj, double *intensity){
    int indicate = tp_select(obj);
    if(indicate == 1){
        scale3(1, (*(*obj).material).diffuse, intensity);
    } else {
        plane_t *pln = (plane_t *)(*obj).priv;
        tplane_t *tp = (tplane_t *)(*pln).priv;
        scale3(1, (*(*tp).background).diffuse, intensity);
    }
}

/*
 * This function will get the specular for the appropriate plane according to 
 * parameters:
 * obj: the object holding the tplane
 * spec: an array to hold the specular
 */
void tp_spec(obj_t *obj, double *spec){
    int indicate = tp_select(obj);
    if(indicate == 1){
        scale3(1, (*(*obj).material).specular, spec);
    } else {
        plane_t *pln = (plane_t *)(*obj).priv;
        tplane_t *tp = (tplane_t *)(*pln).priv;
        scale3(1, (*(*tp).background).specular, spec);
    }
}

