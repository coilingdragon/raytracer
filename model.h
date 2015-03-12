/*
 * This file contains the header file for model.c
 *
 * Author: Sean Facello
 * Verison: 2013/3/29
 */

#ifndef MODEL_H
#define MODEL_H
int trans;
int refrac;
int model_init(FILE*, model_t*);
void model_dump(FILE*);
#endif
