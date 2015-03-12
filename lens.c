/*
 *
 * This file models a sphere, and will allow for the creation of spheres
 *
 * Author: Sean Facello
 * Version: 2013/3/22
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structs.h"
#include "error.h"
#include "object.h"
#include "material.h"
#include "veclib.h"
#include "sphere.h"
#include "lens.h"

#define BUFF_SIZE 256
#define VEC3 3
#define RADIUS 1
#define MISS -1

/*
 * This function will print the state of a lens, to whatever place you 
 * designate with the paramteres
 *
 * parameters:
 * *out: the destination of the output
 * *obj: the obj of type lens you want printed
 */
int lens_dump(FILE *out, obj_t *obj) {
    fprintf(out, "Dumping object of type Lens\n");
    sphere_t *sphere = NULL;
    material_dump(out, obj);
    sphere = (sphere_t *)(*obj).priv;
    fprintf(out, "Lens data\n");
    fprintf(out, "radius -   % 5.3lf\n", (*sphere).radius);
    fprintf(out, "point  -   % 5.3lf   % 5.3lf   % 5.3lf\n", (*sphere).point[0]
            , (*sphere).point[1], (*sphere).point[2]);
    fprintf(out, "\n");
    return 1;
}

/* 
 * This function will determine if a ray fired hits a given sphere
 *
 * parameters:
 * base: the x, y, z coordinates of the origin of a ray
 * dir: the direction of the ray
 * obj: the sphere that needs to be tested
 */
double hits_lens(double* base, double* dir, obj_t* obj) {
    unitvec(dir, dir);
    sphere_t *sphere;
    sphere = (sphere_t *)(*obj).priv;
    double center[3];
    diff3((*sphere).point, (*sphere).point, center);
    double vprime[3];
    diff3((*sphere).point, base, vprime);
    double b = 2 * (dot3(vprime, dir));
    double bsqr = pow(b, 2);
    double a = dot3(dir, dir);
    double c = ((dot3(vprime, vprime)) - pow((*sphere).radius, 2));
    double diss = bsqr - (4 * a * c);
    if(diss > 0) {
        double th1 = 0;
        th1 = ((-1 * b) + sqrt(diss)) / (2 * a);
        if(th1 > .00000001){
            double h[3];
            scale3(th1, dir, dir);
            sum(base, dir, h);
            (*obj).hitloc[0] = h[0];
            (*obj).hitloc[1] = h[1];
            (*obj).hitloc[2] = h[2];
            double cprime[3];
            diff3((*sphere).point, h,  cprime);
            scale3(-1, cprime, cprime);
            (*obj).normal[0] = cprime[0];
            (*obj).normal[1] = cprime[1];
            (*obj).normal[2] = cprime[2];
            unitvec((*obj).normal, (*obj).normal);
            return th1;
        }
    }
    return MISS;
}
