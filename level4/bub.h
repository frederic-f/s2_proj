#ifndef S2_PROJ_BUB_H
#define S2_PROJ_BUB_H

#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>


#define BOARD_LEFT        200
#define BOARD_RIGHT       520
#define BOARD_TOP         31

/* In the sprite, we have 45 images of a 187x157 picture */
#define LAUNCHER_DIV      48 // PI / LAUNCHER_DIV is the angular distance between each of the 45 launcher orientations
#define PI 3.14159265359

#define VELOCITY          1  // bubble velocity
#define BUB_SIZE          40 // horizontal and vertical size
#define BUB_START_Y       457

#define BUB_NX              8       // max number of bubs in hrztl direction
#define BUB_NY              11      // vrtcl

#define NUM_COLOR	  8           



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

void bub_move (bub_t * bub_t_ptr) ;

void bub_place (bub_t * bub_t_ptr,int ** bubs_array) ;


void fatal (char *message) ;

#endif
