#include <SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "data.h"
#include "bub.h"
#include "game.h"


int main(int argc, char* argv[])
{

    /* ****************************************************************************************************************
    * SDL initialization
    * ************************************************************************************************************** */

    /* initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);

    /* set the title bar */
    SDL_WM_SetCaption("Zee Animation", "Zee Animation");

    /* creation of various pointers */
    SDL_Surface *screen_srf_ptr, *temp, *launcher_srf_ptr, *frame_srf_ptr;

    /* create window */
    screen_srf_ptr = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

    /* set keyboard repeat */
    SDL_EnableKeyRepeat(10, 30);


    /* ****************************************************************************************************************
    *
    * ************************************************************************************************************** */

    bool debug = false ;

    if (!debug) {}


    /* ****************************************************************************************************************
    * GAME initialization
    * ************************************************************************************************************** */

    struct Game_t * game_t_ptr = (struct Game_t *) malloc (sizeof(struct Game_t)) ;

    game_init (game_t_ptr) ;


    /* ****************************************************************************************************************
    * BUB initialization
    * ************************************************************************************************************** */

    /*
     * create struct Bub_t object and pointer to it => from now on, bub_t is only accessed via the _ptr
     * */
    struct Bub_t bub_t, * bub_t_ptr ;

    bub_t_ptr = &bub_t ;

    bub_init (bub_t_ptr, game_t_ptr) ;



    /* Information about the current situation of the launcher sprite */
    /* 0 = pointing left, 22 = upwards, 45 = right*/
    int currentOrientation = 22 ;



    /* ******** SPRITES LOADING *************************************************************************************** */

    /* FRAME */
    temp  = SDL_LoadBMP("frame_1p.bmp");
    frame_srf_ptr = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    /* LAUNCHER */
    temp = SDL_LoadBMP("frame_launcher.bmp");
    launcher_srf_ptr = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    /* creation of a CACHE to hide the launcher sprite */
    SDL_Rect cache ;
    cache.x = 0 ;
    cache.y = 0 ;
    cache.w = SCREEN_WIDTH ;
    cache.h = SCREEN_HEIGHT ;

    /* cache will be black */
    int color = SDL_MapRGB(screen_srf_ptr->format, 0, 0, 0) ;

    /* TRANSPARENCY ****************************************** */

    /* set color that is to be transparent */
    int colorkey;
    colorkey = SDL_MapRGB(screen_srf_ptr->format, 255, 0, 255);

    /* FRAME transparency */
    SDL_SetColorKey(frame_srf_ptr, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

    /* LAUNCHER transparency */
    SDL_SetColorKey(launcher_srf_ptr, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

    /* BUB transparency*/
    int k ;
    for (k = 0 ; k < NUM_COLOR ; k += 1) {
        SDL_SetColorKey(game_t_ptr->bubs[k], SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
    }


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

        /* look for an event; possibly update the position and the shape
         * of the sprite. */
        if (SDL_PollEvent(&event)) {
            HandleEvent(event, &gameover, &currentOrientation, bub_t_ptr);
        }

        /* *********************
         * handling the changes on BUB
         * ****************** */

        if (bub_t_ptr->isLaunching) {

            bub_launch (bub_t_ptr, game_t_ptr, &currentOrientation) ;
        }

        /* if bub IS MOVING */
        if (bub_t_ptr->isMoving) {

            /* try to move bub
             * if !move : 2 reasons possible
             * 1. collision -> we check game over, otherwise we place bub
             * 2. hit top -> we place bub */
            if (!bub_move (bub_t_ptr, game_t_ptr)) {

                if (bub_isBelowLimit (bub_t_ptr)) {

                    // TODO put it syst_init and re-init game and bub
                    game_resetBubsArray (game_t_ptr) ;

                    bub_init (bub_t_ptr, game_t_ptr) ;

                }
                else {

                    SDL_Rect * bubJustPlaced_rect ;

                    /* place the new bub into non-moving bubs_array */
                    if ((bubJustPlaced_rect = bub_place (bub_t_ptr, game_t_ptr)) == false )
                        fatal("Problem placing the bub") ;

                    if (debug) {
                        printf ("Bub properly placed at line %d col %d\n", bubJustPlaced_rect->y, bubJustPlaced_rect->x) ;
                    }

                    /* get rid of bubs according to connexity rules */
                    if (!game_checkConnexity (game_t_ptr, bubJustPlaced_rect))
                        fatal ("Problem cheking connexity") ;

                    /* return bub to launcher */
                    bub_init (bub_t_ptr, game_t_ptr);
                }
            }
        }


        /* *********************
         * DRAWING
         * ****************** */


        // draw the cache
        SDL_FillRect(screen_srf_ptr, &cache, color) ;


	    // draw the frame
        // first a Rect for the position
	    SDL_Rect framePosition;
        // then set the position
        framePosition.x = 0 ;
        framePosition.y = 0 ;
        // then draw on screen
	    SDL_BlitSurface(frame_srf_ptr, NULL, screen_srf_ptr, &framePosition) ;


        // draw the sprit LAUNCHER
        SDL_Rect launcherImg_rect ;
        launcherImg_rect.w = LAUNCHER_WIDTH ;
        launcherImg_rect.h = LAUNCHER_HEIGHT ;
        launcherImg_rect.x = 0 ; // the image is moved in height, not in width
        launcherImg_rect.y = LAUNCHER_HEIGHT * currentOrientation ;

        SDL_BlitSurface(launcher_srf_ptr, &launcherImg_rect, screen_srf_ptr, &launcherPos_rect) ;


        /* used by all BUBs, moving or not
         * */

        SDL_Rect bub_rect ;
        bub_rect.w = BUB_SIZE ;
        bub_rect.h = BUB_SIZE ;
        bub_rect.x = 0 ;
        bub_rect.y = 0 ;



        /* draw BUBs on TO        if (!bub_placeOnTop (bub_t_ptr))
            fatal("Placement on top") ;P
        * */

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
                    SDL_BlitSurface(game_t_ptr->bubs[game_t_ptr->bubs_array[i][j] -1], &bub_rect, screen_srf_ptr, dumRect_ptr);
                }

            }
        }


        /*  draw bub MOVING
         * */
	
	    SDL_BlitSurface(bub_t_ptr->sprite_ptr, &bub_rect, screen_srf_ptr, &(bub_t_ptr->position)) ;


        /* update the screen */
        SDL_UpdateRect(screen_srf_ptr, 0, 0, 0, 0);

        //printf ("%d\n", currentOrientation) ;
    }

    /* clean up */
    SDL_FreeSurface(launcher_srf_ptr) ;
    SDL_FreeSurface(frame_srf_ptr);
    SDL_Quit();

    return 0;
}
