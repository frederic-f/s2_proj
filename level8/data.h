#ifndef S2_PROJ_CONSTANTS_H
#define S2_PROJ_CONSTANTS_H

#define SCREEN_WIDTH        720
#define SCREEN_HEIGHT       540

#define LAUNCHER_WIDTH      187
#define LAUNCHER_HEIGHT     157
#define LAUNCHER_CENTER     94

#define GEARS_WIDTH         126
#define GEARS_HEIGHT        116

#define WHEEL_WIDTH         33
#define WHEEL_HEIGHT        47 /* 752 pixel for 16 frames */

#define BOARD_LEFT          200
#define BOARD_RIGHT         520
#define BOARD_TOP           31

#define ROOF_HEIGHT         9
#define ROOF_DELAY          10000 /* ms */

#define CHAIN_WIDTH         23
#define CHAIN_HEIGHT        560

#define LAUNCHER_DIV        48 /*PI / LAUNCHER_DIV is the angular distance between each of the 45 launcher orientations*/
#define PI                  3.14159265359

#define VELOCITY            1  /*bubble velocity*/
#define EXPLOSION_VELOCITY  30 /* how slow the explosion occurs : larger number => slower explosion */
#define EXPLOSION_COEFF     30.0 /* determines trajectory of exploding bub : y = - (1 / COEFF).x + COEFF */

#define BUB_SIZE            40 /*horizontal and vertical size*/
#define BUB_EX_SIZE         54 /* size of exploding bub */
#define BUB_START_Y         438

#define BUB_NX              8       /* max number of bubs in hrztl direction */
#define BUB_NY              11      /* vrtcl */

#define NUM_COLOR	        8
#define ROOFSHIFT           3

#define SYS_STATE_WELCOME   0
#define SYS_STATE_PLAYING   2

#define NB_LEVELS           3

#define BLACK               1
#define BLA                 1
#define BLUE                2
#define BLU                 2
#define GREEN               3
#define GRE                 3
#define ORANGE              4
#define ORA                 4
#define POURPRE             5
#define PUR                 5
#define RED                 6
#define WHITE               7
#define WHI                 7
#define YEL                 8
#define YELLOW              8


#include <SDL.h>
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
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

    double explosion_x ; /* for trajectory of exploding bub */
    double explosion_y ;
};
typedef struct Bub_t bub_t ;
typedef struct Bub_t * bub_t_p ;



struct Sys_t {

    SDL_Surface * screen_srf_ptr ;

    SDL_Surface * launcher_srf_ptr ;

    SDL_Rect * launcher_rect_ptr ; /* to place the launcher */

    SDL_Surface * frame_srf_ptr ;

    SDL_Rect * frame_rect_ptr ; /* to place the board frame */

    SDL_Rect * cache_rect_ptr ; /* to place the black cache to hide the launcher sprite */

    /* roof */
    SDL_Surface * frameTop_srf_ptr ;

    SDL_Rect * frameTop_rect_ptr ; /* to place the roof */

    /* chain */
	SDL_Surface * frameChain_srf_ptr ;

	SDL_Rect * frameChain1_rect_ptr ;
    SDL_Rect * frameChain2_rect_ptr ;


	/* gears */
    SDL_Surface * frameGears_srf_ptr ;

    SDL_Rect * frameGears_rect_ptr ;

    /* little wheel */
	SDL_Surface * frameWheel_srf_ptr ;

    SDL_Rect * frameWheel_rect_ptr ;

    /* next bub */
    SDL_Rect * nextBub_rect_ptr ;

    /* tux */
	SDL_Surface * tux_srf_ptr ;
	
	SDL_Rect * tux_rect_ptr ;

    int colorkey ; /* transparency color */

    /* ******************************************************
    * Fonts and text
    * **************************************************** */

    TTF_Font * scoreFont ;
    TTF_Font * screenFont ; /* for welcome, transition screens */

    SDL_Surface * score ;

    SDL_Rect * scorePosition_rect_ptr ;

    SDL_Color fontColor ;


    SDL_Surface * text_welcomeScreen_1 ;
    SDL_Rect * text_welcomeScreen_1_position_rect_ptr ;


    SDL_Surface * text_welcomeScreen_2 ;
    SDL_Rect * text_welcomeScreen_2_position_rect_ptr ;

    /* ******************************************************
    * Sounds and Music
    * **************************************************** */

    Mix_Chunk * snd_musique, * snd_launch1, * snd_bubsExplode, * snd_gameOver, * snd_levelCompleted ;

    bool isPlayingMusic ;


    /* ******************************************************
    * System state => different screens
     * (0:welcome, 1:start level, 2:playing, 3:level completed, 4:victory, 5:gameocer
    * **************************************************** */

    int state ;

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

    int quit ;

    struct Bub_t * * bub_fallingBubs ; /* to keep track of bub falling */

    int bub_numFallingBubs ;

    int roofShift ; /* Default: 0. How many times the roof got down */

    int roofTimer ; /* SDL_GetTicks timer to keep track of time for roof movement */

    int nextBubColor ;

    int * colorsOnBoard ; /* keep tracks of the colors of bub currently on board */

    int score ;

    int level ;

};
typedef struct Game_t game_t ;



#endif //S2_PROJ_CONSTANTS_H
