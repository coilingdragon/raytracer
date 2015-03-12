/*
 * This file is the first file for the ray tracing project, as such it is a 
 * collection of mathmatical functions for vectors
 *
 * Author: Sean Facello
 * Version: 2013/3/5
 */

#include <stdio.h>
#include <math.h>
#include "veclib.h"

/*
 * This function will find the dot product of the two vectors given, and return 
 * the resulting double.
 *
 * Parameters:
 * v1: the first vector input, referenced by the double*
 * v2: the second vector input, referenced by the double*
 */
double dot3 (double* v1, double* v2) {
    return ((v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]));
}

/*
 * This function will find the vector of v1 after the scaler has been applied, 
 * then store the result in v3. 
 *
 * Parameters:
 * scaler: the scaling factor
 * v1: the vector input
 * v2: the vector the output will be stored in
 */
void scale3 (double scaler, double* v1, double* v2) {
    v2[0] = v1[0] * scaler;
    v2[1] = v1[1] * scaler;
    v2[2] = v1[2] * scaler;
}

/*
 * This function will determine the length of the provided vector and return the 
 * value.
 *
 * Parameters:
 * v1: the vector that the user needs to find out the length of.
 */
double length3 (double* v1) {
    return sqrt(pow(v1[0], 2) + pow(v1[1], 2) + pow(v1[2], 2));
}

/*
 * This function will find the difference between two vectors and store the 
 * result in the variable v3.
 *
 * Parameters:
 * v1: the first vector input
 * v2: the second vector input
 * v3: the location to store the result
 */
void diff3 (double* v1, double* v2, double* v3) {
    v3[0] = v2[0] - v1[0];
    v3[1] = v2[1] - v1[1];
    v3[2] = v2[2] - v1[2];
}

/*
 * This funtion will find the sum of two vectors and store the resulting vector 
 * in the variable v3
 *
 * Parameters:
 * v1: the first vector input
 * v2: the second vector input
 * v3: the location to store the result
 */
void sum (double* v1, double* v2, double* v3) {
    v3[0] = v2[0] + v1[0];
    v3[1] = v2[1] + v1[1];
    v3[2] = v2[2] + v1[2];
}

/*
 * This function will determine the unit vector for v1 and store it in v2
 *
 * Parameters:
 * v1: the input vector
 * v2: will be the unit vector of v1 after the function ends
 */
void unitvec (double* v1, double* v2) {
    double length = length3(v1);
    v2[0] = v1[0] / length;
    v2[1] = v1[1] / length;
    v2[2] = v1[2] / length;
}

/*
 * This function will print the provided vector and the provided label.
 *
 * Parameters:
 * label: will name the vector that the function is printing
 * v1: the vector to be printed
 */
void vecprn3 (char* label, double* v1) {
    fprintf(stderr, "%s (%lf, %lf, %lf)\n", label, v1[0], v1[1], v1[2]);
}

/*
 * This function calculates the reflection of a vector and sets a vector to the
 * result.
 *
 * parameters:
 * unitin - unit vector in the incoming direction 
 * unitnorm - the outward surface normal
 * unitout - unit vector in the outgoing direction
 */
void reflect3(double *unitin, double *unitnorm, double *unitout) {
    unitvec(unitin, unitin);
    unitvec(unitnorm, unitnorm);
    double neg_unitin[3];
    scale3(-1, unitin, neg_unitin);
    double udotn = dot3(neg_unitin, unitnorm);
    udotn = udotn * 2;
    double scaled_norm[3];
    scale3(udotn, unitnorm, scaled_norm);
    diff3(neg_unitin, scaled_norm, unitout);
    unitvec(unitout, unitout);
}

/*
 * This function will determine the angle of refraction for a given point on an
 * object.
 *
 * parameters:
 * unitin - unit vector incoming
 * unitnorm - the normal of hitpoint
 * unitout - the output
 * n1 - outside refraction quality
 * n2 - inside refraction quatlity
 */
void refract3(double *unitin, double *unitnorm, double *unitout, double n1,
                double n2) {
    double n = n1 / n2;
    unitvec(unitin, unitin);
    unitvec(unitnorm, unitnorm);
    double neg_unitin[3];
    scale3(-1, unitin, neg_unitin);
    double c1 = -1 * dot3(unitnorm, unitin);
    double c2 = sqrt(1 - ((n * n) * (1 - (c1 * c1))));
    if(c2 < 0){
        //fprintf(stderr, "c2 less than 0\n");
        return;
    }
    double temp;
    //if(c1 >= 0){
        temp = (n * c1) - c2;
    //} else {
    //    temp = (n * c1) + c2;
    //}
    double temp1[3];
    double temp2[3];
    scale3(n, unitin, temp1);
    scale3(temp, unitnorm, temp2);
    sum(temp1, temp2, unitout);
    unitvec(unitout, unitout);
}


/*
 * This function will take the cross product of two vectors and set the 
 * remaining vector to the result.
 *
 * parameters:
 * v1: a 3d vector
 * v2: a 3d vector
 * v3: the resulting vector
 */
void cross3(double *v1, double *v2, double *v3) {
    v3[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    v3[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    v3[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
}

/*
 * This function will find the projection of a given vector
 * vec: the vector 
 * norm: the normal of a given surface
 * proj: the projected vector
 */
void proj3(double *vec, double *norm, double *proj) {
    unitvec(vec, vec);
    unitvec(norm, norm);
    double temp[3];
    scale3(dot3(norm, vec), norm, temp);
    diff3(temp, vec, proj);
    unitvec(proj, proj);
}


/*int main(void){
    vec3 v1;
    v1.points[0] = 1;
    v1.points[1] = 1;
    v1.points[2] = 1;
    vecprn3("a", v1.points);
    printf("%lf\n", length3(v1.points));
    vec3 v2;
    unitvec(v1.points, v1.points);
    vecprn3("b", v1.points);
    scale3(2, v1.points, v2.points);
    vecprn3("b", v2.points);
    vec3 v3;
    sum(v1.points, v2.points, v3.points);
    vecprn3("c", v3.points);
    printf("%lf\n", dot3(v1.points, v2.points));
    diff3(v1.points, v2.points, v3.points);
    vecprn3("c", v3.points);
    sum(v1.points, v1.points, v1.points);
    vecprn3("a", v1.points);
    diff3(v1.points, v1.points, v1.points);
    vecprn3("a", v1.points);
    return 1;
}*/
