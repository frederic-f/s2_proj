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


int main(int argc, char* argv[])
{
    bool debug = false ;

    if (debug)
        printf ("**Main started\n") ;


    /* ****************************************************************************************************************
    * SDL initialization
    * ************************************************************************************************************** */

    struct Sys_t * sys_t_ptr = (struct Sys_t *) malloc (sizeof (struct Sys_t)) ;

    sys_init (sys_t_ptr) ;


    /* ****************************************************************************************************************
    * GAME initialization
    * ************************************************************************************************************** */

    struct Game_t * game_t_ptr = (struct Game_t *) malloc (sizeof (struct Game_t)) ;

    game_init (game_t_ptr, sys_t_ptr) ;


    /* ****************************************************************************************************************
    * BUB initialization
    * ************************************************************************************************************** */

    struct Bub_t * bub_t_ptr = (struct Bub_t *) malloc (sizeof (struct Bub_t)) ;

    bub_init (bub_t_ptr, game_t_ptr) ;



    /* Information about the current situation of the launcher sprite */
    /* 0 = pointing left, 22 = upwards, 45 = right*/
    int currentOrientation = 22 ;


    /* creation of a CACHE to hide the launcher sprite */
    SDL_Rect cache ;
    cache.x = 0 ;
    cache.y = 0 ;
    cache.w = SCREEN_WIDTH ;
    cache.h = SCREEN_HEIGHT ;

    /* cache will be black */

    int color = SDL_MapRGB(sys_t_ptr->screen_srf_ptr->format, 0, 0, 0) ;




    /* Rectangle to store the position of the sprite in the window. */
    SDL_Rect launcherPos_rect;

    /* set sprite LAUNCHER position in the bottoom of the window */
    launcherPos_rect.x = (SCREEN_WIDTH - LAUNCHER_WIDTH) / 2; // in the middle of the window in WIDTH
    launcherPos_rect.y = SCREEN_HEIGHT - LAUNCHER_HEIGHT ; // at the bottom in HEIGHT


    int gameover = 0;

    /* main loop: check events and re-draw the window until the end */
    while (!gameover)
    {
        SDL_Event event;

        /* look for an event */
        if (SDL_PollEvent(&event)) {
            HandleEvent(event, &gameover, &currentOrientation, bub_t_ptr);
        }

        /* now handling the changes*/

        /* if an event triggered the launching...*/
        if (bub_t_ptr->isLaunching) {

            bub_launch (bub_t_ptr, game_t_ptr, &currentOrientation) ;
        }

        /* if bub is moving... */
        if (bub_t_ptr->isMoving) {

            /* try to move bub
             * if bub_move == false : 2 reasons possible
             * 1. collision -> we check if game over, otherwise we place bub
             * 2. hit top -> we place bub */
            if ((bub_move (bub_t_ptr, game_t_ptr)) == false) {

                /* if it is game over... */
                if (bub_isBelowLimit (bub_t_ptr)) {

                    /* we reset */
                    // TODO put it syst_init and re-init game and bub
                    game_resetBubsArray (game_t_ptr) ;

                    bub_init (bub_t_ptr, game_t_ptr) ;

                }
                /* we place the bub */
                else {

                    SDL_Rect * bubJustPlaced_rect ;

                    /* place the new bub into non-moving bubs_array */
                    if ((bubJustPlaced_rect = bub_place (bub_t_ptr, game_t_ptr)) == false )
                        fatal("Problem placing the bub") ;

                    if (debug) {
                        printf ("Bub properly placed at line %d col %d\n", bubJustPlaced_rect->y, bubJustPlaced_rect->x) ;
                    }

                    /* get rid of bubs according to connexity rules */
                    if ((game_cleanBoard (game_t_ptr, bubJustPlaced_rect)) == false)
                        fatal ("Problem cleaning board") ;

                    /* return bub to launcher */
                    bub_init (bub_t_ptr, game_t_ptr);
                }
            }
        }




        /* draw cache */
        SDL_FillRect(sys_t_ptr->screen_srf_ptr, &cache, color) ;


	    /* draw board frame*/

        SDL_Rect frame_rect;

        frame_rect.x = 0 ;
        frame_rect.y = 0 ;

	    SDL_BlitSurface(sys_t_ptr->frame_srf_ptr, NULL, sys_t_ptr->screen_srf_ptr, &frame_rect) ;


        // draw the sprite LAUNCHER
        SDL_Rect launcherImg_rect ;

        launcherImg_rect.w = LAUNCHER_WIDTH ;
        launcherImg_rect.h = LAUNCHER_HEIGHT ;
        launcherImg_rect.x = 0 ; // the image is moved in height, not in width
        launcherImg_rect.y = LAUNCHER_HEIGHT * currentOrientation ;

        SDL_BlitSurface(sys_t_ptr->launcher_srf_ptr, &launcherImg_rect, sys_t_ptr->screen_srf_ptr, &launcherPos_rect) ;




        /* draw bubs */

        SDL_Rect bub_rect ;
        bub_rect.w = BUB_SIZE ;
        bub_rect.h = BUB_SIZE ;
        bub_rect.x = 0 ;
        bub_rect.y = 0 ;


        /* draw moving bub */

        SDL_BlitSurface(bub_t_ptr->sprite_ptr, &bub_rect, sys_t_ptr->screen_srf_ptr, &(bub_t_ptr->position)) ;


        /* draw non-moving bubs */

        /* we use a pointer to place the non-moving bubs where they belong
         * the same pointer is used for all non-moving bubs
         * it is updated as we parse through the array of non-moving bubs
         * */
        SDL_Rect dummy_rect, * dumRect_ptr ;
        dumRect_ptr = &dummy_rect;

        int i, j ;

        /* parsing the array of non-moving bubs : i=rows, j=cols */
        for (i = 0 ; i < BUB_NY ; i += 1) {

            /* number of bubs in a row depends on odd/even number of row */
            int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

            for (j = 0 ; j < j_max ; j +=1 ) {

                /* process only the bubs set to 1 */
                if (game_t_ptr->bubs_array[i][j] > 0) {

                    /* update the position of the bub to display */
                    dumRect_ptr = getBubPositionRect(i, j, dumRect_ptr);

                    /* display */
                    SDL_BlitSurface(game_t_ptr->bubs[game_t_ptr->bubs_array[i][j] -1], &bub_rect, sys_t_ptr->screen_srf_ptr, dumRect_ptr);
                }

            }
        }


        /* update the screen */
        SDL_UpdateRect(sys_t_ptr->screen_srf_ptr, 0, 0, 0, 0);

    }

    /* Free memory and quit*/
    sys_cleanUp (sys_t_ptr) ;

    return (0) ;
}
