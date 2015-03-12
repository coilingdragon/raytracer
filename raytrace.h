/*
 * This file contains the header file for the raytracer.c file
 *
 * Author: Sean Facello
 * Version: 2013/3/29
 */

#ifndef RAYTRACE_H
#define RAYTRACE_H

#define vec3 3

void ray_trace(model_t*, double*, double*, double*, double, obj_t*);
obj_t* find_closest_obj(list_t*, double*, double*, double*, double*);
void diffuse_illumination(model_t*, obj_t*, double*);
int process_light(list_t*, obj_t*, obj_t*, double*);
void trans_process(model_t*, double*, obj_t*, int, double*);
#endif
