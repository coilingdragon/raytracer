/*
 * This file is the header file for the spotlight.c file
 *
 * Author: Sean Facello
 * Version: 2013/4/23
 */

#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
obj_t* spotlight_init(FILE*, int);
int spotlight_dump(FILE*, obj_t*);
int check_spot(obj_t*, double*);
#endif
