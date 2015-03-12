/*
 * This file is the header file for the list.c class
 *
 * Author: Sean Facello
 * Version: 2013/3/29
 */

#ifndef LIST_H
#define LIST_H
list_t* list_init(void);
void list_add(list_t*, obj_t*);
obj_t* list_get(list_t*, int);
void free_list(list_t);
#endif
