/*
 * This file provides functions for initilizing cylinders, cones, and partial
 * cones.
 *
 * Author: Sean Facello
 * Version: 2013/4/26
 */

#include <stdlib.h>
#include <stdio.h>
#include "veclib.h"
#include "matrixlib.h"
#include "fplane.h"
#include "object.h"
#include "material.h"

#define BUFF_SIZE 256
#define VEC_THREE 3
#define VEC_ONE 1

/*
 * This function will initilize an omni object
 *
 * parameters:
 * in: the source of the file
 * objtype: the type of the object
 */
obj_t *omni_init(FILE *in, int objtype){
    obj_t *obj = object_init(in, objtype);
    omni_t *omni = (omni_t *)malloc(sizeof(omni_t));
    int pcount = 0;
    char buff[BUFF_SIZE];
    (*omni).top = material_init(in);
    (*omni).bot = material_init(in);
    pcount = fscanf(in, "%lf %lf %lf", (*omni).toppoint[0], 
                    (*omni).toppoint[1], (*omni).toppoint[2]);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != VEC_THREE){
        fprintf(stderr, "miss read top point\n");
        exit(1);
    }
    pcount = fscanf(in, "%lf", (*omni).topradius);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != VEC_ONE){
        fprintf(stderr, "miss read top radius\n");
        exit(1);
    }
    pcount = fscanf(in, "%lf %lf %lf", (*omni).botpoint[0], 
                    (*omni).botpoint[1], (*omni).botpoint[2]);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != VEC_THREE){
        fprintf(stderr, "miss read bot point\n");
        exit(1);
    }
    pcount = fscanf(in, "%lf", (*omni).botradius);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != VEC_ONE){
        fprintf(stderr, "miss read bot radius\n");
        exit(1);
    }
    if((*omni).topradius > (*omni),botradius){
        (*omni).bigradius = (*omni).topradius;
        (*omni).smlradius = (*omni).botradius;
    } else {
        (*omni).bigradius = (*omni).botradius;
        (*omni).smlradius = (*omni).topradius;
    }
    pcount = fscanf(in, "%lf", (*omni).height);
    fgets(buff, BUFF_SIZE, in);
    if(pcount != VEC_ONE){
        fprintf(stderr, "miss read height\n");
        exit(1);
    }
    diff3((*omni).botpoint, (*omni).toppoint, (*omni).normal);
    unitvec((*omni).normal, (*omni).scalednorm);
    cross3([0, 1, 0], (*omni).normal, (*omni).rotmat[0]);
    scale3(1, (*omni).normal, (*omni).rotmat[1]);
    cross3((*omni).rotmat[1], (*omni).rotmat[0], (*omni).rotmat[2]);
    return obj;
}

/*
 * This function will dump the data regarding a particular omni shape
 *
 * parameters:
 * out: the place the information is to be dumped to
 * obj: the object holding the omni object
 */
int omni_dump(FILE *out, obj_t *obj){
    fprintf(out, "Dumping object of type Omni\n");
    omni_t *omni = (omni_t *)(*obj).priv;
    return 1;
}

/*
 * This function will determine if a ray hits an omni object
 *
 * parameters:
 * base: the origin  of the array
 * dir: a vector representation of the direction of the array
 * obj: the omni object
 */
double hits_omni(double *base, double *dir, obj_t *obj){
    //rotating and scaling all information
    unitvec(dir, dir);
    omni_t *omni = (omni_t *)(*obj).priv;
    double bprime[3];
    diff3((*omni).toppoint, base, bprime);
    double cprime[3];
    diff3((*omni).toppoint, (*omni).toppoint, cprime);
    double newnorm[3];
    xform3((*omni).rotmat, (*omni).normal, newnorm);
    double newb[3];
    xform3((*omni).rotmat, bprime, newb);
    double newd[3];
    xform3((*omni).rotmat, dir, newd);
    double botprime[3];
    diff3((*omni).toppoint, (*omni).botpoint, botprime);
    double newbot[3];
    xform3((*omni).rotmat, botprime, newbot);
    double point[3];
    //lowerleftpoint for finite plane test
    point[0] = newbot[0] + (*omni).bigradius;
    point[1] = newbot[1];
    point[2] = newbot[2];
    if(dot3([0, 0, 1], dir) != 0) {
        double dist = dot3([0, 0, 1], point) - (dot3([0, 0, 1], base) / 
                        dot3([0, 0, 1], newd));
        if(dist >= .000001) {
            double hit1[3];
            scale3(dist, newd, hit1);
            sum(newb, hit1, hit1);
            if(hit[2] < .000001){
                //finite plane tests & testing edge triangles
                if(hit[0] > ((*omni).bigradius * 2) || hit1[0] < 0.0){
                    return -1;
                } else if(hit[1] > (*omni).height || hit1[1] < 0.0){
                    return -1;
                } else if(left-triangle(omni, hit1) == -1){
                    return -1;
                } else if(right-triangle(omni, hit1) == -1){
                    return -1;
                }
                //TODO: find height, determine radius, determine z-modifier
                //find hitpoint, find normal
                //height == hit[1]
                double tempradius = (((((*omni).botradius - (*omni).botradius) / 
                                      (*omni).height) * hit1[1]) + botradius);
                double zmod = sqrt(pow(tempradius, 2), pow(hit[0], 2));
                dist = dist - zmod;
                hit1[2] = hit1[2] + zmod;
                //TODO: rotate and scale hit1
                scale3(1, hit1, (*omni).hitloc);
                //TODO: find normal
                return dist;
            }
        }
        return -1;
    } else {
        //TODO: check endcaps, determine normal
        double dist = cap_check(omni, base, dir);
        if(dist == -1){
            return -1;
        } 
        return dist;
    }
    return -1;
}

/*
 * This function will determine if a ray hits the left-triangle of an omni_t
 *
 * parameters:
 * omni: the omni object
 * hit: the hit point
 */

    double bprime[3];
    diff3((*omni).toppoint, base, bprime);
    double cprime[3];
    diff3((*omni).toppoint, (*omni).toppoint, cprime);
    double newnorm[3];
    xform3((*omni).rotmat, (*omni).normal, newnorm);
    double newb[3];
    xform3((*omni).rotmat, bprime, newb);
    double botprime[3];
    diff3((*omni).toppoint, (*omni).botpoint, botprime);
    double newbot[3];
    xform3((*omni).rotmat, botprime, newbot);

