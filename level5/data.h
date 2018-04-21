#ifndef S2_PROJ_CONSTANTS_H
#define S2_PROJ_CONSTANTS_H

#define SCREEN_WIDTH        720
#define SCREEN_HEIGHT       540

#define LAUNCHER_WIDTH      187
#define LAUNCHER_HEIGHT     157
#define LAUNCHER_CENTER     94

#define BOARD_LEFT          200
#define BOARD_RIGHT         520
#define BOARD_TOP           31

/* In the sprite, we have 45 images of a 187x157 picture */
#define LAUNCHER_DIV        48 // PI / LAUNCHER_DIV is the angular distance between each of the 45 launcher orientations
#define PI 3.14159265359

#define VELOCITY            1.2  // bubble velocity
#define BUB_SIZE            40 // horizontal and vertical size
#define BUB_START_Y         457

#define BUB_NX              8       // max number of bubs in hrztl direction
#define BUB_NY              11      // vrtcl

#define NUM_COLOR	        8


#include <SDL.h>
#include <stdbool.h>


struct Game_t {

    SDL_Surface * bubs[NUM_COLOR] ;

    int * * bubs_array ; /* non-moving bubs /presence/ are kept track of in a pointer-style 2-dimension array */

    int * * * bub_array_centers ; /* all possible spaces for a bub /centers coordinates/ are kept track of in a pointer-syle 3-dimension array */

    int * * bub_connected_component ; /* array to keep track of connected components */

    int * * bub_fifo ; /* file to keep track bubs whose connexity is to check */

    int fifoHead, fifoTail ; /* indexes for bub_fifo */

    int bub_ny ; /* used when roof goes down */

};
typedef struct Game_t game_t ;



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


#endif //S2_PROJ_CONSTANTS_H
