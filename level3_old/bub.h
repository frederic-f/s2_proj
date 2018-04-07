#ifndef S2_PROJ_BUB_H
#define S2_PROJ_BUB_H



#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

struct Bub_t {
    bool isOnLauncher ;
    bool isMoving ;
    bool isLaunching ;

    double  x ;     // real x coordinate
    double  y ;     // real y coordinate

    double dir_x ;  // step of x motion
    double dir_y ;  // step of y motion

    SDL_Rect position ;
};
typedef struct Bub_t bub_t ;


void loadSprite(bub_t * bub) ;

void setColorKey () ;

void init () ;

#endif //S2_PROJ_BUB_H
