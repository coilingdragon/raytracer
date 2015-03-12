/*
 * This file contains two functions that relate to directly raytracing and 
 * finding out what a ray will collide with, and at what angle it will deflect
 * off of.
 *
 * Author: Sean Facello
 * Version: 2013/3/29
*/

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "model.h"
#include "veclib.h"
#include "objid.h"
#include "sphere.h"
#include "plane.h"
#include "fplane.h"
#include "spotlight.h"
#include "raytrace.h"
#include "material.h"

#define outragous_dist 100000
#define MAX_DIST 50000
#define MAX_HITS 50

static int hits = 1;

/*
 * This function will trace each ray as it is fired from each pixel on the 
 * viewing plane.
 *
 * parameters:
 * model: pointer to the model struct for this image
 * base[]: the location of the viewer
 * dir[]: the unit vector in the drection the viewer is pointing
 * intensity[]: the intensity location 
 * total_dist: the distance the ray has traveled
 * last_hit: the obj that reflected the ray, null if not
 */
void ray_trace(model_t *model, double base[vec3], double dir[vec3], 
                double intensity[vec3], double total_dist, obj_t *last_hit) {
    obj_t *closest = NULL;
    double mindist = outragous_dist;
    double specref[3];
    if(total_dist > MAX_DIST || hits > MAX_HITS) {
        hits = 1;
        return;
    }
    closest = find_closest_obj((*model).scene, base, dir, NULL, &mindist);
    if(closest == NULL) {
        return;
    }
    #ifdef DBG_HIT
    fprintf(stderr, " HIT %4d: %5.1lf (%5.1lf, %5.1lf, %5.1lf) - ",
                      closest->objid, mindist,
                      closest->hitloc[0], closest->hitloc[1],
                      closest->hitloc[2]);
    #endif
    total_dist = total_dist + mindist;
    (*closest).getamb(closest, intensity);
    diffuse_illumination(model, closest, intensity); 
    if(trans == 1){
        if(refrac == 1 && (*(*closest).material).refraction != 0 && 
                (*(*closest).material).transparency != 0){
            double new_dir[3];
            if(last_hit != NULL){
                refract3(dir, (*closest).normal, new_dir, 
                            (*(*last_hit).material).refraction, 
                            (*(*closest).material).refraction);
            } else {
                refract3(dir, (*closest).normal, new_dir, 1, 
                            (*(*closest).material).refraction);
            }
            trans_process(model, new_dir, closest, total_dist, intensity);
        } else if((*(*closest).material).transparency != 0){
            trans_process(model, dir, closest, total_dist, intensity);
        }
    }
    scale3((1 / total_dist), intensity, intensity);
    #ifdef DBG_AMB
    fprintf(stderr, "AMB (%5.1lf, %5.1lf, %5.1lf) - ",
                     intensity[0], intensity[1], intensity[2]);
    #endif
    (*closest).getspec(closest, specref);
    if(dot3(specref, specref) > 0) {
        double specint[3];
        double ref_dir[3];
        reflect3(dir, (*closest).normal, ref_dir);
        hits += 1;
        ray_trace(model, (*closest).hitloc, ref_dir, specint, 
                    total_dist, closest);
        if(trans == 1){
            if((*(*closest).material).transparency != 0){
                scale3((*(*closest).material).transparency, 
                        specref, specref);
                scale3(1 - (*(*closest).material).transparency, specint, specint);
            }
        }
        specref[0] = specint[0] * specref[0];
        specref[1] = specint[1] * specref[1];
        specref[2] = specint[2] * specref[2];
    }
    sum(intensity, specref, intensity);
    hits = 1;
}

/*
 * This function will determine which object is closest that a ray hit. This 
 * function will return NULL if no object is hit
 * 
 * parameters:
 * scene: the list of ojbects in the image
 * base: the location of the viewer being :q
 * used for this raytrace
 * dir: the unit vector in the direction the viwer is pointing
 * source: used for reflections, not implemented currently, will be null
 * mindist: a double to record how far the object that is hit is from the 
 */
obj_t* find_closest_obj(list_t *scene, double base[vec3], double dir[vec3], 
                        double source[vec3], double *mindist) {
    obj_t *closest = NULL;
    obj_t *point = (*scene).first;
    double temp = 0;
    while(point != NULL) {
        temp = (*point).hits(base, dir, point);
        if(temp < *mindist && temp > .000000000001) {
            (*mindist) = temp;
            closest = point;
        }
        point = (*point).next;
    }
    #ifdef DBG_FIND
    if(closest == NULL) {
        fprintf(stderr, "FND NULL: %5.1lf - ", *mindist);
    } else {
        fprintf(stderr, "FND %4d: %5.1lf - ", closest->objid, *mindist);
    }
    #endif
    return closest;
}

/*
 * this is used to calculate the amount of light hitting a particular point of 
 * an object
 *
 * parameters:
 * *model: pointer to the model structure
 * *hitobj: object that was hit by a ray
 * intensity: where the intesnity needs to be added
 */
void diffuse_illumination(model_t *model, obj_t *hitobj, double *intensity) {
    list_t *lights = (*model).lights;
    obj_t *temp = (*lights).first;
    while(temp != NULL) {
        process_light((*model).scene, hitobj, temp, intensity);
        temp = (*temp).next;
    }
}

/*
 * this function will process a given light to determine if it affects a given 
 * point on an object
 *
 * parameters:
 * *lst: the list of all objects
 * *hitobj: the object hit by the ray 
 * lobj: the current light source
 * intensity: the r, g, b of the intensity vector
 *
 */
int process_light(list_t *lst, obj_t *hitobj, obj_t *lobj, double *intensity) {
    double ray[3];
    light_t *light = (light_t *)(*lobj).priv;
    diff3((*hitobj).hitloc, (*light).point, ray);
    double dist = length3(ray);
    if((*lobj).objtype == SPOTLIGHT){
        int check = check_spot(lobj, ray);
        if(check == 0){
            return 0;
        }
    }
    obj_t *closest = find_closest_obj(lst, (*hitobj).hitloc, ray, NULL, &dist);
    unitvec(ray, ray);
    if(dot3(ray, (*hitobj).normal) > 0){
        if(closest == NULL){
            #ifdef DBG_DIFFUSE
            fprintf(stderr, "hit object id was %d\n", hitobj->objid);
            vecprn3("hit point was ", hitobj->hitloc);
            vecprn3("normal at hitpoint ", hitobj->normal);
            fprintf(stderr, "light object id was %d\n", lobj->objid);
            vecprn3("light center was ", (*light).point);
            vecprn3("unit vector to light is ", ray);
            fprintf(stderr, "distance to light is %lf\n", dist);
            fprintf(stderr, "cosine is %lf\n", dot3(ray, (*hitobj).normal));
            #endif
            double diffuse[3]; 
            (*hitobj).getdiff(hitobj, diffuse);
            intensity[0] +=  (diffuse[0] * (*lobj).emissivity[0] * 
                                    dot3(ray, (*hitobj).normal)) / dist;
            intensity[1] +=  (diffuse[1] * (*lobj).emissivity[1] * 
                                    dot3(ray, (*hitobj).normal)) / dist;
            intensity[2] +=  (diffuse[2] * (*lobj).emissivity[2] * 
                                    dot3(ray, (*hitobj).normal)) / dist;
            #ifdef DBG_DIFFUSE
                vecprn3("Emissivity of the light ", (*lobj).emissivity);
                vecprn3("Diffuse reflectivity ", diffuse);
                vecprn3("Current ivec ", intensity);
            #endif
            return 1;
        } else {
            if(trans == 1){
                if((*(*closest).material).transparency != 0 && 
                        dist < MAX_DIST){
                    double new_inten[3];
                    int stat = process_light(lst, closest, lobj, new_inten);
                    if(stat != 0){
                        scale3((*(*closest).material).transparency, new_inten, 
                                    new_inten);
                        scale3(1 - (*(*closest).material).transparency, 
                                    intensity, intensity);
                    }
                }
            } else {
                    #ifdef DBG_DIFFUSE
                    /* If occluded by another object */
                    fprintf(stderr, "hit object occluded by %d\n", closest->objid);
                    fprintf(stderr, "distance was %lf\n", dist);
                    fprintf(stderr, "\n");
                    #endif
            }
            return(0);
        }
    } else {
        return 0;
    }
    return 0;
}

/*
 * This function does recursive calls to determine the objects behind a 
 * transparent object.
 *
 * parameters:
 * model: the model of the scene
 * vec: the direction of the ray
 * obj: the previous hit
 * dist: the distance traveled
 * intensity: an intensity for the objects behind obj
 */
void trans_process(model_t *model, double *vec, obj_t *obj, int dist, 
                    double *intensity){
    double new_inten[3];
    ray_trace(model, (*obj).hitloc, vec, new_inten, dist, obj);
    double trans_fac = 1 - (*(*obj).material).transparency;
    scale3((*(*obj).material).transparency, new_inten, new_inten);
    scale3(trans_fac, intensity, intensity);
    sum(intensity, new_inten, intensity);
}
