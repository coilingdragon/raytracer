/*
 * This file will hold data representing the size of hte world, window size, 
 * and the location of the view point.
 *
 * Author: Sean Facello
 * Verison: 2013/3/29
 */

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "error.h"
#include "projection.h"

#define BUFF_SIZE 256
#define WINDOW 2
#define LOCATION 3
#define Z_LIMIT 10
#define WIN_SIZE 500

/*
 * This function will initalize the given proj_t struct.
 * 
 * praramaters:
 * in: the location the information is being read from
 * proj: the proj_t that needs to be initalized
 */
proj_t* proj_init(FILE* in){
    proj_t *proj = (proj_t *)malloc(sizeof(proj_t));
    int rc = 0;
    char buff[BUFF_SIZE];
    fgets(buff, BUFF_SIZE, in);
    rc = sscanf(buff, "%lf %lf", &((*proj).win_size_world[0]), 
            &((*proj).win_size_world[1]));
    if(rc != WINDOW){
        exit(1);
    }
    (*proj).win_size_world[2] = Z_LIMIT;
    fgets(buff, BUFF_SIZE, in);
    rc = sscanf(buff, "%lf %lf %lf", &(*proj).view_point[0], 
            &(*proj).view_point[1], &(*proj).view_point[2]);
    fgets(buff, BUFF_SIZE, in);
    if(rc != LOCATION){
        exit(1);
    }
    (*proj).win_size_pixel[0] = WIN_SIZE;
    (*proj).win_size_pixel[1] = WIN_SIZE;
    return proj;
}

/*
 * This funciton will dump the data stored in a given proj_t to stderr
 *
 * paramaters:
 * proj: the proj_t that is to be printed
 */
int projection_dump(FILE *out, proj_t *proj){
    fprintf(out, "Window size in pixels \n\t%d\t%d\n\n", 
            (*proj).win_size_pixel[0], (*proj).win_size_pixel[1]);
    fprintf(out, "Window size in world coordinates \n\t%lf\t%lf\n\n",
            (*proj).win_size_world[0], (*proj).win_size_world[1]);
    fprintf(out, "Location of Viewpoint \n\t%lf\t%lf\t%lf\n\n", 
            (*proj).view_point[0], (*proj).view_point[1], 
            (*proj).view_point[2]);
    return 0;
}


