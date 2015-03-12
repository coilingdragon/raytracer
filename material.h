/*
 * This file is the header file for the material.c file
 *
 * Author: Sean Facello
 * Version: 2013/3/29
 */

#ifndef MATERIAL_H
#define MATERIAL_H
int material_dump(FILE*, obj_t*);
material_t* material_init(FILE*);
void getamb(obj_t*, double*);
void getdiff(obj_t*, double*);
void getspec(obj_t*, double*);
#endif
