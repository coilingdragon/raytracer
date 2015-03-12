/*
 * This file is the header file for the tplane.c file
 *
 * Author: Sean Facello
 * Version: 2013/4/22
 */

#ifndef TPLANE_H
#define TPLANE_H
obj_t* tplane_init(FILE*, int);
int tplane_dump(FILE*, obj_t*);
int tp_select(obj_t*);
void tp_amb(obj_t*, double*);
void tp_diff(obj_t*, double*);
void tp_spec(obj_t*, double*);
#endif
