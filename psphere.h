/*
 * This file is the header file for procedural spheres
 *
 * Author: Sean Facello
 * Version: 2013/5/3
 */

#ifndef PSPHERE_H
#define PSPHERE_H
void psphere0_amb(obj_t*, double*);
void psphere1_amb(obj_t*, double*);
void psphere2_amb(obj_t*, double*);
obj_t* psphere_init(FILE*, int);
int psphere_dump(FILE*, obj_t*);
#endif

