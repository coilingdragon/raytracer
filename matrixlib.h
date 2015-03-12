/*
 * This file is the header file for matrixlib.c
 *
 * Author: Sean Facello
 * Version: 2013/4/21
 */

#ifndef MATRIXLIB_H
#define MATRIXLIB_H
void mat_mult3(double[][3], double[][3], double[][3]);
void xform3(double[][3], double[3], double[3]);
void xpose3(double in[][3], double out[][3]);
void mat_print3(char*, double[][3]);
#endif
