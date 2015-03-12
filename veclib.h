/*
 * This header file will define the fuctions and a struct for a 3D vector
*/

#ifndef VECLIB_H
#define VECLIB_H


//typedef struct {
//    double points[3];
//} vec3;

double dot3(double* v1, double* v2);
void scale3(double scaler, double* v1, double* v2);
double length3(double* v1);
void diff3(double* v1, double* v2, double* v3);
void sum(double* v1, double* v2, double* v3);
void unitvec(double* v1, double* v2);
void vecprn3(char* label, double* v1);
void reflect3(double*, double*, double*);
void cross3(double*, double*, double*);
void proj3(double*, double*, double*);
void refract3(double*, double*, double*, double, double);
#endif
