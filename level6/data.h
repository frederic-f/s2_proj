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

#define LAUNCHER_DIV        48 /*PI / LAUNCHER_DIV is the angular distance between each of the 45 launcher orientations*/
#define PI                  3.14159265359

#define VELOCITY            1  /*bubble velocity*/
#define EXPLOSION_VELOCITY  30

#define BUB_SIZE            40 /*horizontal and vertical size*/
#define BUB_EX_SIZE         54 /* size of exploding bub */
#define BUB_START_Y         457

#define BUB_NX              8       /* max number of bubs in hrztl direction */
#define BUB_NY              11      /* vrtcl */

#define NUM_COLOR	        8


#include <SDL.h>
#include <stdbool.h>


struct Bub_t {

    SDL_Surface * sprite_ptr ;

    SDL_Rect * position ; /* position of the sprite on the window WE USE .x and .y only for the display */

    SDL_Rect * spriteFrame ; /* position of the sprite on the window WE USE .x and .y only for the display */

    short color ;

    double  x ;     /* real coordinates */
    double  y ;

    int start_x ; /* position on launcher */
    int start_y ;

    bool isLaunching ;
    bool isMoving ;
    bool isExploding ;

    double step_x ;  /* step of x motion */
    double step_y ;  /* step of y motion */

};
typedef struct Bub_t bub_t ;



struct Sys_t {

    SDL_Surface * screen_srf_ptr ;

    SDL_Surface * launcher_srf_ptr ;

    SDL_Surface * frame_srf_ptr ;

    SDL_Rect * cache_rect_ptr ; /* to place the black cache to hide the launcher sprite */

    SDL_Rect * frame_rect_ptr ; /* to place the board frame */

    SDL_Rect * launcher_rect_ptr ; /* to place the launcher */

    int colorkey ; /* transparency color */

};
typedef struct Sys_t sys_t ;/* step of x motion */



struct Game_t {

    int launcherOrientation ; /* value : 0-45 /  22 (vertical) is start value */

    SDL_Surface * bubs[NUM_COLOR] ;

    SDL_Surface * bubsEx[NUM_COLOR] ; /* exploding sprite : 30 images*/

    int * * bubs_array ; /* non-moving bubs /presence/ are kept track of in a pointer-style 2-dimension array */

    int * * * bub_array_centers ; /* all possible spaces for a bub /centers coordinates/ are kept track of in a pointer-syle 3-dimension array */

    int * * bub_connected_component ; /* array to keep track of connected components */

    int * * bub_fifo ; /* queue to keep track of bubs whose connexity is to check */

    int fifoHead, fifoTail ; /* indexes for bub_fifo */

    int bub_ny ; /* used when roof goes down */

    int quit ;

    struct Bub_t * * bub_fallingBubs ; /* to keep track of bub falling */

    int bub_numFallingBubs ;

};
typedef struct Game_t game_t ;



#endif //S2_PROJ_CONSTANTS_H
