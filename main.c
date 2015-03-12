/*
 * This is the file that contains the main function of this program.
 *
 * Author: Sean Facello
 * Verison: 2013/3/29
 */

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "error.h"
#include "list.h"
#include "model.h"
#include "projection.h"
#include "image.h"

/*
 * This is the main function of the program, and as such will run the program 
 * and will manage the program.
 * 
 * paramaters:
 * argc: the number of command line arguments
 * argv: the array containing the command line arguments as strings
 */
int main(int argc, char **argv){
    model_t *model = (model_t *)malloc(sizeof(model_t));
    int rc = 0;
    (*model).proj = proj_init(stdin);
    (*(*model).proj).win_size_pixel[0] = atoi(argv[1]);
    (*(*model).proj).win_size_pixel[1] = atoi(argv[2]);
    projection_dump(stderr, (*model).proj);
    (*model).scene = list_init();
    (*model).lights = list_init();
    rc = model_init(stdin, model);
    model_dump(stderr);
    if(rc == 0) {
        make_image(model);
    }
    free_list((*(*model).lights));
    free_list((*(*model).scene));
    free((*model).lights);
    free((*model).scene);
    free((*model).proj);
    free(model);
    return(1);
}
