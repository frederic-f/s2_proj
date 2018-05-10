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


/* ****************************************************************************************************************
*   Initialization
* ************************************************************************************************************** */
int sys_init (sys_t * sys_t_ptr) {

    /* initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);

    /* set the title bar */
    SDL_WM_SetCaption("Puzzle Bobble - FST Nancy", "Puzzle Bobble - FST Nancy");

    /* set keyboard repeat */
    SDL_EnableKeyRepeat(10, 30);

    sys_t_ptr->screen_srf_ptr = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

    /* set color that is to be transparent */
    sys_t_ptr->colorkey = SDL_MapRGB(sys_t_ptr->screen_srf_ptr->format, 255, 0, 255);

    /* load all sprites but bubbles*/
    sys_loadSprites (sys_t_ptr) ;


    /* launcher */
    sys_t_ptr->launcher_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    /* set sprite LAUNCHER position in the bottoom of the window */
    sys_t_ptr->launcher_rect_ptr->x = (SCREEN_WIDTH - LAUNCHER_WIDTH) / 2; // in the middle of the window in WIDTH
    sys_t_ptr->launcher_rect_ptr->y = SCREEN_HEIGHT - LAUNCHER_HEIGHT ; // at the bottom in HEIGHT


    /* board frame*/
    sys_t_ptr->frame_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    sys_t_ptr->frame_rect_ptr->x = 0 ;
    sys_t_ptr->frame_rect_ptr->y = 0 ;


    /* creation of a CACHE to hide the launcher sprite */
    sys_t_ptr->cache_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    sys_t_ptr->cache_rect_ptr->x = 0 ;
    sys_t_ptr->cache_rect_ptr->y = 0 ;
    sys_t_ptr->cache_rect_ptr->w = SCREEN_WIDTH ;
    sys_t_ptr->cache_rect_ptr->h = SCREEN_HEIGHT ;


    return (0) ;
}


/* ****************************************************************************************************************
*   Load Sprites controlled by sys
* ************************************************************************************************************** */
int sys_loadSprites (sys_t * sys_t_ptr) {

    SDL_Surface *temp ;


    /* Frame of board */
    temp  = SDL_LoadBMP("img/frame_1p.bmp");
    sys_t_ptr->frame_srf_ptr = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    sys_makeTransparent (sys_t_ptr, sys_t_ptr->frame_srf_ptr) ;


    /* Launcher */
    temp = SDL_LoadBMP("img/frame_launcher.bmp");
    sys_t_ptr->launcher_srf_ptr = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    sys_makeTransparent (sys_t_ptr, sys_t_ptr->launcher_srf_ptr) ;


    return (0) ;
}


/* ****************************************************************************************************************
*   Makes sprite transparent
* ************************************************************************************************************** */
int sys_makeTransparent (sys_t * sys_t_ptr, SDL_Surface * surf_ptr) {

    return SDL_SetColorKey(surf_ptr, SDL_SRCCOLORKEY | SDL_RLEACCEL, sys_t_ptr->colorkey);

}


/* ****************************************************************************************************************
*   Draw all sprites on screen
* ************************************************************************************************************** */
int sys_draw (sys_t * sys_t_ptr, game_t * game_t_ptr, bub_t * bub_t_ptr) {


    /* cache */
    /* cache will be black */
    int color = SDL_MapRGB(sys_t_ptr->screen_srf_ptr->format, 0, 0, 0) ;

    SDL_FillRect(sys_t_ptr->screen_srf_ptr, sys_t_ptr->cache_rect_ptr, color) ;


    /* frame */
    SDL_BlitSurface(sys_t_ptr->frame_srf_ptr, NULL, sys_t_ptr->screen_srf_ptr, sys_t_ptr->frame_rect_ptr) ;


    /* launcher */
    SDL_Rect launcherImg_rect ;

    launcherImg_rect.w = LAUNCHER_WIDTH ;
    launcherImg_rect.h = LAUNCHER_HEIGHT ;
    launcherImg_rect.x = 0 ; // the image is moved in height, not in width
    launcherImg_rect.y = LAUNCHER_HEIGHT * game_t_ptr->launcherOrientation ;

    SDL_BlitSurface(sys_t_ptr->launcher_srf_ptr, &launcherImg_rect, sys_t_ptr->screen_srf_ptr, sys_t_ptr->launcher_rect_ptr) ;


    /* draw bubs */
    SDL_Rect bub_rect ;
    bub_rect.w = BUB_SIZE ;
    bub_rect.h = BUB_SIZE ;
    bub_rect.x = 0 ;
    bub_rect.y = 0 ;

    /* draw moving bub */
    SDL_BlitSurface(bub_t_ptr->sprite_ptr, &bub_rect, sys_t_ptr->screen_srf_ptr, bub_t_ptr->position) ;


    /* draw non-moving bubs */

    /* we use a pointer to place the non-moving bubs
     * the same pointer is used for all non-moving bubs
     * it is updated as we parse through the array of non-moving bubs
     * */
    SDL_Rect * dumRect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    int i, j ;

    /* parsing the array of non-moving bubs : i=rows, j=cols */
    for (i = 0 ; i < BUB_NY ; i += 1) {

        /* number of bubs in a row depends on odd/even number of row */
        int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

        for (j = 0 ; j < j_max ; j +=1 ) {

            /* process only the bubs set to 1 */
            if (game_t_ptr->bubs_array[i][j] > 0) {

                /* update the position of the bub to display */
                dumRect_ptr = sys_getBubPositionRect(i, j, dumRect_ptr);

                /* display */
                SDL_BlitSurface(game_t_ptr->bubs[game_t_ptr->bubs_array[i][j] -1], &bub_rect, sys_t_ptr->screen_srf_ptr, dumRect_ptr);
            }
        }
    }

    free (dumRect_ptr) ;


    /* update the screen */
    SDL_UpdateRect(sys_t_ptr->screen_srf_ptr, 0, 0, 0, 0);


    return (0) ;
}


/* ****************************************************************************************************************
*   Free memory and Exit SDL
* ************************************************************************************************************** */
int sys_cleanUp (sys_t * sys_t_ptr, game_t * game_t_ptr, bub_t * bub_t_ptr)  {

    int i, j ;

    /* game_t_ptr pointers */

    /* bubs_array */
    for (i = 0 ; i < BUB_NY ; i += 1) {

        free (game_t_ptr->bubs_array[i]) ;
    }

    free (game_t_ptr->bubs_array) ;

    /* bub_array_centers */
    for (i = 0 ; i < BUB_NY ; i += 1) {

        int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

        for (j = 0 ; j < j_max ; j +=1 ) {

            free (game_t_ptr->bub_array_centers[i][j]) ;
        }

        free (game_t_ptr->bub_array_centers[i]) ;
    }

    free (game_t_ptr->bub_array_centers) ;

    /* bub_connected_component */
    for (i = 0 ; i < BUB_NY ; i += 1) {

        free (game_t_ptr->bub_connected_component[i]) ;
    }

    free (game_t_ptr->bub_connected_component) ;

    /* bub_fifo */
    for (i = 0 ; i < (BUB_NY * BUB_NX) ; i += 1) {

        free (game_t_ptr->bub_fifo[i]) ;
    }

    free (game_t_ptr->bub_fifo) ;

    free (game_t_ptr) ;



    /* bub_t pointers */

    free (bub_t_ptr->position) ;
    free (bub_t_ptr->sprite_ptr) ;
    free (bub_t_ptr) ;


    /* SDL pointers */

    SDL_FreeSurface (sys_t_ptr->launcher_srf_ptr) ;

    SDL_FreeSurface (sys_t_ptr->frame_srf_ptr) ;

    SDL_FreeSurface (sys_t_ptr->screen_srf_ptr) ;


    /* sys_t_ptr pointers */

    free (sys_t_ptr->cache_rect_ptr) ;
    free (sys_t_ptr->frame_rect_ptr) ;
    free (sys_t_ptr->launcher_rect_ptr) ;
    free (sys_t_ptr) ;


    /* Quit framework */
    SDL_Quit ();

    return (0) ;
}


/* ****************************************************************************************************************
* Function that receives [i=lig][j=col] of a cell from the bubs_array
* returns a _ptr to SDL_Rect object with coords /x and y OF TOP LEFT CORNER/
* so that main program can position the bub
* WARNING : coords are for top left corner
* ************************************************************************************************************** */

SDL_Rect * sys_getBubPositionRect(int i, int j, SDL_Rect * dumRect_ptr) {

    /* distance between each bub */
    int d_x = (BOARD_RIGHT - BOARD_LEFT) / 8;

    /* there are 8 bubs on even rows
     * there are 7 bubs on odd rows
     * for odd rows (2d option of ternary op) we add a shift to the right */
    dumRect_ptr->x = (i % 2 == 0) ? BOARD_LEFT + j*d_x : BOARD_LEFT + j*d_x + BUB_SIZE / 2;

    dumRect_ptr->y = BOARD_TOP + (35 * i) ; //35 because 40 * sqrt(3)/2 = 35 and with that bubs are close to each other

    return dumRect_ptr ;
}


/* ****************************************************************************************************************
*   Returns a random number between 0 and (max - 1)
* ************************************************************************************************************** */
short getRandomNumber(int max) {

    clock_t start_t ;

    /* Get number of CPU ticks since program started */
    start_t = clock();

    /* Initializes random number generator */
    srand (start_t) ;

    /* Generates numbers from 0 to max */
    return rand() % max ;
}


/* ****************************************************************************************************************
*   Detects SDL events
* ************************************************************************************************************** */
void sys_handleEvent (SDL_Event event, game_t * game_t_ptr, bub_t * bub_t_ptr)
{
    switch (event.type) {
        /* close button  clicked */
        case SDL_QUIT:
            game_t_ptr->quit = 1;
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
                case SDLK_SPACE:
                    if(!bub_t_ptr->isLaunching) {
                        bub_t_ptr->isLaunching = true ;
                    }
                    break ;
                default:
                    break;
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                case SDLK_q:
                    game_t_ptr->quit = 1;
                    break;
                case SDLK_LEFT:
                    /* launcher rotates to the left, unless already at extreme left */
                    if (game_t_ptr->launcherOrientation > 0) {
                        game_t_ptr->launcherOrientation -= 1 ;
                    }
                    break;
                case SDLK_RIGHT:
                    // launcher rotates to the right, unless already at far right
                    if (game_t_ptr->launcherOrientation < 44) {
                        game_t_ptr->launcherOrientation += 1 ;
                    }
                    break;

                default:
                    break;
            }
            break;
    }
}