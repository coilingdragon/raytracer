/*
 * This file contains the header file and function prototypes for the file 
 * pplane.c
 */

#ifndef PPLANE_H
#define PPLANE_H
obj_t* pplane_init(FILE*, int);
void pplane0_amb(obj_t*, double*);
void pplane1_amb(obj_t*, double*);
void pplane2_amb(obj_t*, double*);
void pplane3_amb(obj_t*, double*);
int pplane_dump(FILE*, obj_t*);
#endif
