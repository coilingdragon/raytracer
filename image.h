/*
 * This file is the header file for the image.c file.
 *
 * Author: Sean Facello
 * Version: 2013/3/29
 */
#include <stdio.h>

#ifndef IMAGE_H
#define IMAGE_H
void map_pix_to_world(proj_t*, int, int, double*);
void make_pixel(model_t*, int, int, unsigned char*);
void make_image(model_t*);
#endif
