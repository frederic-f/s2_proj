#include <SDL.h>
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"


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
    * Fonts
    * ************************************************************************************************************** */

    TTF_Init();

    if(TTF_Init() == -1)

    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());

        exit(EXIT_FAILURE);

    }


    /* ****************************************************************************************************************
    * SYS initialization ---------------- (SDL, Sprites, ...)
    * ************************************************************************************************************** */

    struct Sys_t * sys_t_ptr = (struct Sys_t *) malloc (sizeof (struct Sys_t)) ;

    sys_init (sys_t_ptr) ;


    /* ****************************************************************************************************************
   * Music
   * ************************************************************************************************************** */

    // load support for the OGG and MOD sample/music formats

 /*   int flags=MIX_INIT_OGG;

    int initted=Mix_Init(flags);

    if((initted&flags) != flags) {
        printf("Mix_Init: Failed to init required ogg and mod support!\n");
        printf("Mix_Init: %s\n", Mix_GetError());
        // handle error
    }
*/


    sys_t_ptr->musique = (Mix_Chunk *) malloc (sizeof (Mix_Chunk)) ;
    sys_t_ptr->launch1 = (Mix_Chunk *) malloc (sizeof (Mix_Chunk)) ;
    sys_t_ptr->bubsExplode = (Mix_Chunk *) malloc (sizeof (Mix_Chunk)) ;

    //Mix_Chunk *musique, *launch1, *bubsExplode;
    Mix_AllocateChannels(32);
    
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 1024) ;
    
    sys_t_ptr->musique = Mix_LoadWAV("music/mario-world-intro.wav");
    sys_t_ptr->launch1 = Mix_LoadWAV("music/launch1.wav");
    sys_t_ptr->bubsExplode = Mix_LoadWAV("music/bubsExploding.wav");

    if(!sys_t_ptr->musique) {
        printf("Mix_LoadMUS(\"music.mp3\"): %s\n", Mix_GetError());
    }




    /* ****************************************************************************************************************
    * GAME initialization ---------------- (Bubs_array, connexity, launcher ...)
    * ************************************************************************************************************** */

    struct Game_t * game_t_ptr = (struct Game_t *) malloc (sizeof (struct Game_t)) ;

    game_init (game_t_ptr, sys_t_ptr) ;


    /* ****************************************************************************************************************
    * BUB initialization ---------------- (Movement, placement, ...)
    * ************************************************************************************************************** */

    struct Bub_t * bub_t_ptr = (struct Bub_t *) malloc (sizeof (struct Bub_t)) ;


    /* ****************************************************************************************************************
    * Starts a new game
    * ************************************************************************************************************** */
    game_newGame (sys_t_ptr, game_t_ptr, bub_t_ptr) ;


    /* ****************************************************************************************************************
    * main loop: check events and re-draw the window until the end
    * ************************************************************************************************************** */


    while (!game_t_ptr->quit)
    {


        /* Look for an event */
        SDL_Event event;

        if (SDL_PollEvent(&event)) {
            sys_handleEvent(event, sys_t_ptr, game_t_ptr, bub_t_ptr);
        }


        switch (sys_t_ptr->state) {


            case 0 : /* welcome screen */



                break ;


            case 1 :



                break ;



            case 2 : /* playing */

                /* play music */
                /* Play music on first available channel*/
                if (!sys_t_ptr->isPlayingMusic) {
                    if(Mix_PlayChannel(-1, sys_t_ptr->musique, 0)==-1) {
                        printf("Mix_PlayChannel: %s\n",Mix_GetError()) ;
                    }
                    else {
                        sys_t_ptr->isPlayingMusic = true ;
                    }
                }




                /* we check if roof has to go down*/
                if ((SDL_GetTicks() - game_t_ptr->roofTimer) > ROOF_DELAY) {

                    /* roof goes down one notch */
                    game_shiftRoof (sys_t_ptr, game_t_ptr) ;

                    /* timer reset */
                    game_resetRoofTimer (game_t_ptr) ;

                    /* check if roof shift makes bub be below limit */
                    game_checkRoofGameOver (sys_t_ptr, game_t_ptr, bub_t_ptr) ;

                }




                /* Handling the changes following the event */

                /* if an event triggered the launch...*/
                if (bub_t_ptr->isLaunching) {

                    if(Mix_PlayChannel(-1, sys_t_ptr->launch1, 0)==-1) {
                        printf("Mix_PlayChannel: %s\n",Mix_GetError());
                    }


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
                            game_gameOver (game_t_ptr);

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
                            game_cleanBoard (sys_t_ptr, game_t_ptr, bubJustPlaced_rect) ;

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





                break ; /* end case 2 : */
        } /* switch */


        /* draw all the sprites and update the screen */
        sys_draw (sys_t_ptr, game_t_ptr, bub_t_ptr) ;

    }

    /* free memory and quit*/
    sys_cleanUp (sys_t_ptr, game_t_ptr, bub_t_ptr) ;


    /* close TTF */
    TTF_Quit();

    return (0) ;
}
