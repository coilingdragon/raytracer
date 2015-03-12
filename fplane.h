/*
 * This file is the header file for the fplane.c file
 *
 * Author: Sean Facello
 * Version: 2013/4/21
 */

#ifndef FPLANE_H
#define FPLANE_H
obj_t* fplane_init(FILE*, int);
double hits_fplane(double*, double*, obj_t*);
int fplane_dump(FILE*, obj_t*);
#endif
