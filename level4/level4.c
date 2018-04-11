#include <SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "bub.h"
#include "game.h"

#define SCREEN_WIDTH        720
#define SCREEN_HEIGHT       540

#define LAUNCHER_WIDTH      187
#define LAUNCHER_HEIGHT     157
#define LAUNCHER_CENTER     94






SDL_Rect * getBubPositionRect(int i, int j, SDL_Rect * dumRect_ptr) {

    /* distance between each bub */
    int d_x = (BOARD_RIGHT - BOARD_LEFT) / 8;

    /* there are 8 bubs on even rows
     * there are 7 bubs on odd rows
     * for odd rows (2d option of ternary op) we add a shift to the right */
    dumRect_ptr->x = (i % 2 == 0) ? BOARD_LEFT + j*d_x : BOARD_LEFT + j*d_x + BUB_SIZE / 2;

    dumRect_ptr->y = BOARD_TOP + (35 * i) ;

    return dumRect_ptr ;
}



int main(int argc, char* argv[])
{
    SDL_Surface *screen_srf_ptr, *temp, *launcher_srf_ptr, *frame_srf_ptr;

    /* ****************************************************************************************************************
    * SDL initialization
    * ************************************************************************************************************** */

    /* initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);

    /* set the title bar */
    SDL_WM_SetCaption("Zee Animation", "Zee Animation");

    /* create window */
    screen_srf_ptr = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

    /* set keyboard repeat */
    SDL_EnableKeyRepeat(10, 30);


    /* ****************************************************************************************************************
    * BUB initialization
    * ************************************************************************************************************** */

    /*
     * create struct Bub_t object and pointer to it => from now on, bub_t is only accessed via the _ptr
     * */
    struct Bub_t bub_t, * bub_t_ptr ;

    bub_t_ptr = &bub_t ;

    bub_init (bub_t_ptr) ;


    /*
     * non-moving bubs are kept track of in a pointer-style 2-dimension array
     * */
    int * * bubs_array = (int * *) malloc (BUB_NY * sizeof(int *)) ;

    int i, j ;

    for (i = 0 ; i < BUB_NY ; i += 1) {

        bubs_array[i] = (int *) malloc (BUB_NX * sizeof(int)) ;

        /* number of bubs in a row depends on odd/even number of row */
        int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

        for (j = 0 ; j < j_max ; j +=1 ) {

            //bubs_array[i][j] = (i==0 || i == 1 || i == 2) ? 1 : 0 ;
            bubs_array[i][j] = 1 ;
        }
    }



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

    /* BUBBLE */
    //temp = SDL_LoadBMP("bub_blue.bmp");
    //bub_t_ptr->sprite_ptr = SDL_DisplayFormat(temp) ;
    //SDL_FreeSurface(temp) ;

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
    SDL_SetColorKey(bub_t_ptr->sprite_ptr, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

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

        if (bub_t_ptr->isLaunching)
            bub_launch (bub_t_ptr, &currentOrientation) ;

        /* if bub IS MOVING */
        if (bub_t_ptr->isMoving) {

            /* try to move bub */
            bub_move (bub_t_ptr) ;

            /* if it stopped moving (reach top or other bub)
             * ==> we PLACE it */
            if (!bub_t_ptr->isMoving) {

                /* place the new bub into non-moving bubs_array */
                bub_place(bub_t_ptr, bubs_array);

                /* return bub to luncher */
                bub_getOnLauncher(bub_t_ptr);
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

        /* parsing the array of non-moving bubs : i=rows, j=cols */
        for (i = 0 ; i < BUB_NY ; i += 1) {

            for (j = 0 ; j < BUB_NX ; j +=1 ) {

                /* process only the bubs set to 1 */
                if (bubs_array[i][j] == 1) {

                    /* update the position of the bub to display */
                    dumRect_ptr = getBubPositionRect(i, j, dumRect_ptr);

                    /* display */
                    SDL_BlitSurface(bub_t_ptr->sprite_ptr, &bub_rect, screen_srf_ptr, dumRect_ptr);
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
