/*
 * This file models the information that will be used to create the ray-traced 
 * picture, this includes the information about the size of the image, a list 
 * of the lights, and a list of the objects in the scene.
 *
 * Author: Sean Facello
 * Verison: 2013/3/29
 */

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "object.h"
#include "objid.h"
#include "list.h"
#include "sphere.h"
#include "plane.h"
#include "model.h"
#include "material.h"
#include "projection.h"
#include "pplane.h"
#include "psphere.h"
#include "light.h"
#include "spotlight.h"
#include "fplane.h"
#include "tplane.h"
#include "lens.h"

#define BUFF_SIZE 256

/*
 * This function starts the process of reading in from a file and creating the 
 * two lists of lights and objects.
 *
 * parameters:
 * in: the source of the information for the ray-tracer.
 * model: the model struct that needs to be filled in.
 */
int model_init(FILE *in, model_t *model) {
    char buff[BUFF_SIZE];
    int objtype = 0;
    int rc = 0;
    obj_t *obj = NULL;
    int stat = 1;
    int objid = 0;
    fscanf(in, "%d", &objtype);
    if(objtype == 9000){
        trans = 1;
        fgets(buff, BUFF_SIZE, in);
        fscanf(in, "%d", &objtype);
    } else {
        trans = 0;
    }
    if(objtype == 9001){
        refrac = 1;
        fgets(buff, BUFF_SIZE, in);
        fscanf(in, "%d", &objtype);
    } else {
        refrac = 0;
    }
    fgets(buff, BUFF_SIZE, in);
    while(stat == 1) { 
        if(buff != NULL) {
            switch(objtype) {
                case LIGHT:
                    obj = light_init(in, objtype);
                    (*obj).objid = objid;
                    light_dump(stderr, obj);
                    break;
                case SPOTLIGHT:
                    obj = spotlight_init(in, objtype);
                    (*obj).objid = objid;
                    spotlight_dump(stderr, obj);
                    break;
                case PROJECTOR:
                    //null, do not exist yet
                    break;
                case SPHERE:
                    obj = sphere_init(in, objtype);
                    (*obj).objid = objid;
                    (*obj).hits = hits_sphere;
                    (*obj).getamb = getamb;
                    sphere_dump(stderr, obj);
                    break;
                case PLANE:
                    obj = plane_init(in, objtype);
                    (*obj).objid = objid;
                    (*obj).hits = hits_plane;
                    (*obj).getamb = getamb;
                    plane_dump(stderr, obj);
                    break;
                case FINITE_PLANE:
                    obj = fplane_init(in, objtype);
                    (*obj).objid = objid;
                    (*obj).hits = hits_fplane;
                    (*obj).getamb = getamb;
                    fplane_dump(stderr, obj);
                    break;
                case TILED_PLANE:
                    obj = tplane_init(in, objtype);
                    (*obj).objid = objid;
                    (*obj).hits = hits_plane;
                    (*obj).getamb = tp_amb;
                    (*obj).getdiff = tp_diff;
                    (*obj).getspec = tp_spec;
                    tplane_dump(stderr, obj);
                    break;
                case TEX_PLANE:
                case REF_SPHERE:
                    //null do not exist yet
                case P_SPHERE:
                    obj = psphere_init(in, objtype);
                    (*obj).objid = objid;
                    (*obj).hits = hits_sphere;
                    psphere_dump(stderr, obj);
                    break;
                case P_PLANE:
                    obj = pplane_init(in, objtype);
                    (*obj).objid = objid;
                    (*obj).hits = hits_plane;
                    pplane_dump(stderr, obj);
                    break;
                case PARABOLOID:
                case CYLINDER:
                case CONE:
                case HYPERBOLOID:
                case LAST_TYPE:
                    //null, do not exist yet
                    break;
                case LENS:
                    obj = sphere_init(in, objtype);
                    (*obj).getamb = getamb;
                    (*obj).objid = objid;
                    (*obj).hits = hits_lens;
                    lens_dump(stderr, obj);
                    break;
            }
            if(objtype > LAST_LIGHT) {
                list_add((*model).scene, obj);
                if(objtype != TILED_PLANE){
                    (*obj).getdiff = getdiff;
                    (*obj).getspec = getspec;
                }
            } else {
                list_add((*model).lights, obj);
            }
            rc = fscanf(in, "%d", &objtype);
            if(rc != 1) {
                stat = 0;
            }
            fgets(buff, BUFF_SIZE, in);
        } else {
            stat = 0;
        }
        objid++;
    }
    return 0;
}
/*
 * This funciton confirms the finishing of the reading in from a given source.
 *
 *
 * prarameters:
 * out: the destination to print the message
 */
void model_dump(FILE *out) {
    fprintf(out, "The model has been loaded\n");
}
