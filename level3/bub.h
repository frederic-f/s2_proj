#ifndef S2_PROJ_BUB_H
#define S2_PROJ_BUB_H

struct Bub_t {
    bool isOnLauncher = true ;
    bool isMoving = false ;
    bool isLaunching = false ;

    double  x ;     // real x coordinate
    double  y ;     // real y coordinate

    double dir_x ;  // step of x motion
    double dir_y ;  // step of y motion

    SDL_rect position ;
};
typedef struct Bub_t bub_t ;


void loadSprite(bub_t * bub) ;

void setColorKey () ;

void init () ;

#endif //S2_PROJ_BUB_H
