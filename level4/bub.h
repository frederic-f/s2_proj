#ifndef S2_PROJ_BUB_H
#define S2_PROJ_BUB_H

#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>


#include "constants.h"



struct Bub_t {

    SDL_Surface * sprite_ptr ;

    SDL_Rect position ; // position of the sprite on the window WE USE .x and .y only for the display

    short color ;
    
    double  x ;     // real x coordinate
    double  y ;

    int start_x ; // position on launcher
    int start_y ;

    bool isLaunching ;
    bool isMoving ;

    double step_x ;  // step of x motion
    double step_y ;

};
typedef struct Bub_t bub_t ;


void bub_init (bub_t * bub_t_ptr) ;

void bub_launch (bub_t * bub_t_ptr, int * currOrientation) ;

void bub_getOnLauncher (bub_t * bub_t_ptr) ;

bool bub_move (bub_t * bub_t_ptr, int ** bubs_array, int *** bub_array_centers) ;

bool bub_place (bub_t * bub_t_ptr, int ** bubs_array, int *** bub_array_centers) ;

bool bub_isColliding (bub_t * bub_t_ptr, int ** bubs_array, int *** bub_array_centers, double *target_pos_x, double *target_pos_y) ;

double bub_getDistanceBetweenTwoBubs (double bub1_x, double bub1_y, double bub2_x, double bub2_y) ;

void fatal (char *message) ;

#endif
