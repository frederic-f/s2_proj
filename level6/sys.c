#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include "data.h"
#include "sys.h"
#include "game.h"
#include "bub.h"



int sys_init (sys_t * sys_t_ptr) {

    /* initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);

    /* set the title bar */
    SDL_WM_SetCaption("Puzzle Bobble - made in FST Nancy", "Zee Animation");

    /* set keyboard repeat */
    SDL_EnableKeyRepeat(10, 30);

    sys_t_ptr->screen_srf_ptr = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

    /* set color that is to be transparent */
    sys_t_ptr->colorkey = SDL_MapRGB(sys_t_ptr->screen_srf_ptr->format, 255, 0, 255);

    /* load all sprites but bubbles*/
    sys_loadSprites (sys_t_ptr) ;



    return (0) ;
}


/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int sys_loadSprites (sys_t * sys_t_ptr) {

    SDL_Surface *temp ;


    /* Frame of board */
    temp  = SDL_LoadBMP("frame_1p.bmp");
    sys_t_ptr->frame_srf_ptr = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    sys_makeTransparent (sys_t_ptr, sys_t_ptr->frame_srf_ptr) ;


    /* Launcher */
    temp = SDL_LoadBMP("frame_launcher.bmp");
    sys_t_ptr->launcher_srf_ptr = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    sys_makeTransparent (sys_t_ptr, sys_t_ptr->launcher_srf_ptr) ;

    return (0) ;

}


/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int sys_makeTransparent (sys_t * sys_t_ptr, SDL_Surface * surf_ptr) {

    return SDL_SetColorKey(surf_ptr, SDL_SRCCOLORKEY | SDL_RLEACCEL, sys_t_ptr->colorkey);

}


/* ****************************************************************************************************************
*   Free memory
* ************************************************************************************************************** */
int sys_cleanUp (sys_t * sys_t_ptr) {

    /* Free memory */
    SDL_FreeSurface (sys_t_ptr->launcher_srf_ptr) ;

    SDL_FreeSurface (sys_t_ptr->frame_srf_ptr);

    /* Quit framework */
    SDL_Quit ();

    return (0) ;

}