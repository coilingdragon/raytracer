/*
 * This file contains functions relating to operations that deal with matrices.
 *
 * Author: Sean Facello
 * Version: 2013/4/21
 */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "veclib.h"
#include "matrixlib.h"

#define VEC_THREE 3
void identity_matrix3(double mtx[][3]){
    mtx[0][0] = 1;
    mtx[1][1] = 1;
    mtx[2][2] = 1;
}

/*
 * This funciton will multiply 2 3x3 matrices and set the third matrix to the 
 * resulting matrix.
 *
 * parameters:
 * inleft: the left side matrix
 * inright: the right side matrix
 * out: the mateix holding the result 
 */
void mat_mult3(double inleft[][3], double inright[][3], double out[][3]){
    int i;
    int j;
    double holder[3][3];
    for(i = 0; i < VEC_THREE; i++){
        double temp[] = {inright[0][i], inright[1][i], inright[2][i]};
        for(j = 0; j < VEC_THREE; j++){
            holder[i][j] = dot3(temp, inleft[j]);
        }
    }
    for(i = 0; i < VEC_THREE; i++){
        for(j = 0; j < VEC_THREE; j++){
            out[i][j] = holder[i][j];
        }
    }
}

/*
 * This funciton will multiply a 3x3 matrix by a vector.
 *
 * parameters:
 * inleft: a 3x3 matrix 
 * inright: a vector
 * out: a vector result
 */
void xform3(double inleft[][3], double inright[3], double out[3]){
    int i;
    double outtemp[3];
    for(i = 0; i < VEC_THREE; i++){
        outtemp[i] = dot3(inright, inleft[i]);
    }
    for(i = 0; i < VEC_THREE; i++){
        out[i] = outtemp[i];
    }
}

/*
 * This function will transpose a given 3x3 matrix and set the second parameter 
 * to the result.
 *
 * par`ameters:
 * in: the 3x3 matrix
 * out: the result
 */
void xpose3(double in[][3], double out[][3]){
    int i;
    int j;
    for(i = 0; i < VEC_THREE; i++){
        for(j = 0; j < VEC_THREE; j++){
            out[j][i] = in[i][j];
        }
    }
}

/*
 * This function will print a matrix
 *
 * parameters:
 * des: the description of the matrix
 * mat: the matrix
 */
void mat_print3(char *des, double mat[][3]) {
    fprintf(stderr, "%s\n", des);
    fprintf(stderr, "%lf %lf %lf\n", mat[0][0], mat[0][1], mat[0][2]);
    fprintf(stderr, "%lf %lf %lf\n", mat[1][0], mat[1][1], mat[1][2]);
    fprintf(stderr, "%lf %lf %lf\n", mat[2][0], mat[2][1], mat[2][2]);
}

/*int main(){
    double m1[3][3];
    double m2[3][3];
    double m3[3][3];
    double v1[3];
    double v2[3];
    double v3[3];
    double xaxis[3] = {1.0, 0.0, 0.0};
    double yaxis[3] = {0.0, 1.0, 0.0};
    double zaxis[3] = {0.0, 0.0, 1.0};
    identity_matrix3(m1); // create an identity matrix
    m1[0][1] = 2; m1[0][2] = 3;
    m1[1][0] = 15; m1[1][2] = 16;
    m1[2][0] = 22; m1[2][1] = 23;
    mat_print3("Original", m1);
    xpose3(m1, m2); 
    mat_print3("Transpose", m2);
    mat_print3("original", m1);
    mat_mult3(m1, m2, m3);
    mat_print3("Product", m3);
    v1[0] = 1;
    v1[1] = 2;
    v1[2] = 3;
    xform3(m2, v1, v2); 
    vecprn3("Transformed vector", v2);
    cross3(xaxis, yaxis, v2);
    vecprn3("X x Y is:", v2);
    cross3(yaxis, zaxis, v2); 
    vecprn3("Y x Z is:", v2);
    cross3(zaxis, xaxis, v2);
    vecprn3("Z x X is:", v2);
    cross3(xaxis, zaxis, v2);
    vecprn3("X x Z is:", v2);
    cross3(xaxis, xaxis, v2);
    vecprn3("X x X is:", v2);
    proj3(v1, zaxis, v2);
    vecprn3("projection of v1 on xaxis is:", v2);
    return 1;
}*/
