/*
 * This file holds the header file for the projection.c file
 *
 * Author: Sean Facello
 * Verison: 2013/3/29
 */

#ifndef PROJECTION_H
#define PROJECTION_H
proj_t* proj_init(FILE* in);
int projection_dump(FILE*, proj_t*);
#endif
