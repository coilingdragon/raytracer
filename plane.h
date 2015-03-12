/*
 * This file is the header file for the plane.c file
 *
 * Author: Sean Facello
 * Version: 2013/3/29
 */

#ifndef PLANE_H
#define PLANE_H
obj_t* plane_init(FILE*, int);
int plane_dump(FILE*, obj_t*);
double hits_plane(double*, double*, obj_t*);
#endif
