/*
 * This file contains two functions that will allow the rays that are created 
 * in the raytracing algorithim to be translated to pixels and it will create 
 * the ppm file that will become the image.
 *
 * Author: Sean Facello
 * Verison: 2013/3/29
 */

#include <stdlib.h>
#include "structs.h"
#include "image.h"
#include "veclib.h"
#include "raytrace.h"
#include "model.h"
#include "projection.h"

#define VEC3 3
#define START_DIST 0.0
#define MAX_COLOR 255
#define MAX_INTENSITY 1.0
#define MIN_INTENSITY 0.0

/*
 * This function converts the pixel coordinates to world cooridinates, and 
 * determines the direction of the resulting vector.
 *
 * parameters:
 * proj: the proj_t representing this image
 * x: the x coordinate of the pixel
 * y: the y coordinate of the pixel
 * world: the vector relating to the given pixel
 */
void map_pix_to_world(proj_t *proj, int x, int y, double *world) {
    *(world + 0) = (double)x / ((*proj).win_size_pixel[0] - 1) * 
                    ((*proj).win_size_world[0]);
    *(world + 0) -= (*proj).win_size_world[0] / 2.0;
    *(world + 2) = 0.0;
    *(world + 1) = (double)y / ((*proj).win_size_pixel[1] - 1) * 
                    ((*proj).win_size_world[1]);
    *(world + 1) -= (*proj).win_size_world[1] / 2.0;
}

/*
 * This function will convert the pixels of the image to be created into rays 
 * that can be raytraced.
 *
 * parameters:
 * model: the model_t struct for the picture we are trying to make
 * x: the x coordiante of the pixel
 * y: the y coordinate of the pixel
 * pixval: the r, g, b of the pixel we are setting
 */
void make_pixel(model_t *model, int x, int y, unsigned char *pixval) {
    double world[VEC3];
    double intensity[VEC3];
    map_pix_to_world((*model).proj, x, y, world);
    #ifdef DBG_WORLD
    fprintf(stderr, "WRL (%5.1lf, %5.1lf) - ", world[0], world[1]);
    #endif
    intensity[0] = START_DIST;
    intensity[1] = START_DIST;
    intensity[2] = START_DIST;
    double dir[3]; 
    diff3((*(*model).proj).view_point, world, dir);
    ray_trace(model, (*(*model).proj).view_point, dir, intensity, 
                START_DIST, NULL);
    int i;
    for(i = 0; i < vec3; i++) {
        if(intensity[i] > MAX_INTENSITY) {
            intensity[i] = MAX_INTENSITY;
        }
        else if(intensity[i] < MIN_INTENSITY) {
            intensity[i] = MIN_INTENSITY;
        }
    }
    for(i = 0; i < vec3; i++) {
        *pixval = (unsigned char)MAX_COLOR * intensity[i];
        pixval++;
    }
}

/*
 * This function will created the ppm file that will be the output of the 
 * program itself.
 *
 * parameters:
 * model: the source of the image to be created
 */
void make_image(model_t *model) {
    unsigned char *pixmap = NULL;
    int x = (*(*model).proj).win_size_pixel[0];
    int y = (*(*model).proj).win_size_pixel[1];
    fprintf(stderr, "%d %d x y \n", x, y);
    pixmap = (unsigned char *)malloc(sizeof(unsigned char) * x * y * 3);
    int i;
    for(i = y; i > 0; i--) {
        int j;
        for(j = 1; j <= x; j++) {
            #ifdef DBG_PIX
            fprintf(stderr, "\nPIX %4d %4d - ", j - 1, i - 1);
            #endif
            make_pixel(model, j - 1, i - 1, (&pixmap[((y - i) * 3 * x) + (j * 3)]));
        }
    }
    printf("P6 %d %d %d\n", x, y, MAX_COLOR);
    i = fwrite(pixmap, sizeof(unsigned char), x * y * 3, stdout);
    if(i != x * y * 3) {
        fprintf(stderr, "error in writing the file\n");
        exit(1);
    }
    free(pixmap);
}
 
