/*
 * This file holds all of the structs that are needed in the ray tracer.
 *
 * Author: Sean Facello
 * Version: 2013/3/30
 */

#ifndef STRUCT_H
#define STRUCT_H
#define VEC 3
#define VEC_TWO 2
/*
 * This is the struct to model a material of an object
 */
typedef struct material_type {
    double ambient [VEC];
    double diffuse [VEC];
    double specular[VEC];
    double transparency;
    double refraction;
} material_t;

/* This struct will create what is similar to a linked list in an object 
 * oriented language 
 */
typedef struct object_type {
    material_t *material;
    void *priv;
    struct object_type *next;
    int objid;
    int objtype;
    double (*hits)(double *base, double *dir, struct object_type *obj);
    double hitloc[VEC];
    double normal[VEC];
    double emissivity[VEC];
    void (*getamb)(struct object_type *, double *);
    void (*getdiff)(struct object_type *, double *);
    void (*getspec)(struct object_type *, double *);
} obj_t;

/*
 * This struct will model the start of a linked list, which will hold a pointer
 * to the first "node", and the last "node".
 */
typedef struct list_type { 
    obj_t *first;
    obj_t *last;
} list_t;

/*
 * This struct will hold the information about the projection.
 */
typedef struct projection_type {
    int win_size_pixel[VEC_TWO];
    double win_size_world[VEC];
    double view_point[VEC];
} proj_t;

/*
 * This is the struct that will hold the information required by the ray-tracer.
 */
typedef struct model_type {
    proj_t *proj;
    list_t *lights;
    list_t *scene;
} model_t;

/* This is a model of an infinite plane */
typedef struct plane_type{
    double point[VEC];
    double normal[VEC];
    void *priv;
} plane_t;

/* This models a sphere, using the center and radius */
typedef struct sphere_type{
    double point[VEC];
    double radius;
} sphere_t;

/* This models a light source */
typedef struct light_type{
    double point[VEC];
    void *priv;
} light_t;

/* This models a specialized finite plane */
typedef struct fplane_type{
    double xdir[VEC];
    double size[VEC_TWO];
    double rotmat[VEC][VEC];
    double lasthit[VEC_TWO];
} fplane_t;

/* This struct models a tiled plane */
typedef struct tplane_type{
    double xdir[VEC];
    double size[VEC_TWO];
    double rotmat[VEC][VEC];
    material_t *background;
} tplane_t;

/* This struct models a spotlight */
typedef struct spotlight_type{
    double direction[VEC];
    double theta;
    double costheta;
} spotlight_t;

/* This struct models cylinders, cones, and partial cones */
typedef struct omni_type{
    double toppoint[VEC];
    double botpoint[VEC];
    double topradius;
    double botradius;
    double height;
    double normal[VEC];
    double scalednorm[VEC];
    double bigradius;
    double smlradius;
    material_t *top;
    material_t *bot;
    double rotmat[VEC][VEC];
} omni_t;
#endif
