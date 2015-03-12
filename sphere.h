/*
 * This file is the header for the file sphere.c
 *
 * Author: Sean Facello
 * Verison: 2013/3/29
 */

#ifndef SPHERE_H
#define SPHERE_H

obj_t* sphere_init(FILE* in, int objtype);
int sphere_dump(FILE* out, obj_t* obj);
double hits_sphere(double* base, double* dir, obj_t* obj);
#endif
