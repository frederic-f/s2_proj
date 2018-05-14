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


int main()
{
    bool debug = false ;

    if (debug)
        printf ("**Main started\n") ;


    /* ****************************************************************************************************************
    * SYS initialization ---------------- (SDL, Sprites, ...)
    * ************************************************************************************************************** */

    struct Sys_t * sys_t_ptr = (struct Sys_t *) malloc (sizeof (struct Sys_t)) ;

    sys_init (sys_t_ptr) ;


    /* ****************************************************************************************************************
    * GAME initialization ---------------- (Bubs_array, connexity, launcher ...)
    * ************************************************************************************************************** */

    struct Game_t * game_t_ptr = (struct Game_t *) malloc (sizeof (struct Game_t)) ;

    game_init (game_t_ptr, sys_t_ptr) ;


    /* ****************************************************************************************************************
    * BUB initialization ---------------- (Movement, placement, ...)
    * ************************************************************************************************************** */

    struct Bub_t * bub_t_ptr = (struct Bub_t *) malloc (sizeof (struct Bub_t)) ;

    //bub_init (bub_t_ptr, game_t_ptr) ;


    /* ****************************************************************************************************************
    * Starts a new game
    * ************************************************************************************************************** */
    game_newGame (sys_t_ptr, game_t_ptr, bub_t_ptr) ;


    /* ****************************************************************************************************************
    * main loop: check events and re-draw the window until the end
    * ************************************************************************************************************** */


    while (!game_t_ptr->quit)
    {

        /* we check if roof has to go down*/
        if ((SDL_GetTicks() - game_t_ptr->roofTimer) > ROOF_DELAY) {

            /* roof goes down one notch */
            game_shiftRoof (sys_t_ptr, game_t_ptr) ;

            /* timer reset */
            game_resetRoofTimer (game_t_ptr) ;

            /* check if roof shift makes bub be below limit */
            game_checkRoofGameOver (sys_t_ptr, game_t_ptr, bub_t_ptr) ;

        }


        /* Look for an event */
        SDL_Event event;

        if (SDL_PollEvent(&event)) {
            sys_handleEvent(event, game_t_ptr, bub_t_ptr);
        }

        /* Handling the changes following the event */

        /* if an event triggered the launch...*/
        if (bub_t_ptr->isLaunching) {

            bub_launch (bub_t_ptr, game_t_ptr) ;
        }

        /* if bub is moving... */
        if (bub_t_ptr->isMoving) {

            /* try to move bub
             * if impossible (ie bub_move == -1) : 2 reasons possible
             * 1. collision -> we check if game over, otherwise we place bub
             * 2. hit top -> we place bub */
            if ((bub_move (bub_t_ptr, game_t_ptr)) == -1) {

                /* if it is game over... */
                if (bub_isBelowLimit (bub_t_ptr)) {

                    /* game over routine (message, etc.) */
                    game_gameOver ();

                    /* we reset */
                    game_newGame (sys_t_ptr, game_t_ptr, bub_t_ptr) ;

                }
                /* we place the bub */
                else {

                    SDL_Rect * bubJustPlaced_rect ;

                    /* place the new bub into non-moving bubs_array */
                    bubJustPlaced_rect = bub_place (bub_t_ptr, game_t_ptr) ;

                    if (debug) {
                        printf ("Bub properly placed at line %d col %d\n", bubJustPlaced_rect->y, bubJustPlaced_rect->x) ;
                    }

                    /* get rid of bubs according to connexity rules */
                    game_cleanBoard (game_t_ptr, bubJustPlaced_rect) ;

                    free (bubJustPlaced_rect) ;

                    /* check if game won */
                    if (game_checkVictory (game_t_ptr)) {

                        /* victory message */
                        printf ("VICTORY :-)\n") ;

                        /* we reset */
                        game_newGame (sys_t_ptr, game_t_ptr, bub_t_ptr) ;
                    }

                    /* return bub to launcher */
                    bub_init (bub_t_ptr, game_t_ptr);
                }
            }
        }


        /* move falling bubs */
        game_moveFallingBub (game_t_ptr) ;


        /* draw all the sprites and update the screen */
        sys_draw (sys_t_ptr, game_t_ptr, bub_t_ptr) ;

        /* here we check for game over (roof down, ...), that is, AFTER screen is updated (so player can see what happened */


    }

    /* free memory and quit*/
    sys_cleanUp (sys_t_ptr, game_t_ptr, bub_t_ptr) ;

    return (0) ;
}
