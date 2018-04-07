#include <SDL.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "bub.h"

#define SCREEN_WIDTH      720
#define SCREEN_HEIGHT     540

#define LAUNCHER_WIDTH    187
#define LAUNCHER_HEIGHT   157
#define LAUNCHER_CENTER   94


void HandleEvent(SDL_Event event,
        int * quit, int * currOrientation, bub_t * bub_t_ptr)
{
    switch (event.type) {
        /* close button clicked */
        case SDL_QUIT:
            *quit = 1;
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
                    *quit = 1;
                    break;
                case SDLK_LEFT:

                    /* launcher rotates to the left, unless already at extreme left */
                    if (*currOrientation > 0) {
                        *currOrientation -= 1 ;
                    }
                    break;
                case SDLK_RIGHT:
                    // launcher rotates to the right, unless already at far right
                    if (*currOrientation < 44) {
                        *currOrientation += 1 ;
                    }
                    break;

                default:
                    break;
            }
            break;
    }
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

    /* create struct Bub_t object and pointer to it => from now on, bub_t is only accessed via the _ptr
     * */
    struct Bub_t bub_t, * bub_t_ptr ;

    bub_t_ptr = &bub_t ;

    bub_init (bub_t_ptr) ;



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
    temp = SDL_LoadBMP("bub_blue.bmp");
    bub_t_ptr->sprite_ptr = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    /* creation of a cache to hide the launcher sprite */
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


    /* setup BUBSPRITE colorkey and turn on RLE */
    SDL_SetColorKey(bub_t_ptr->sprite_ptr, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


    /* Rectangle to store the position of the sprite in the window. */
    SDL_Rect spritePosition;

    /* set sprite LAUNCHER position in the bottoom of the window */
    spritePosition.x = (SCREEN_WIDTH - LAUNCHER_WIDTH) / 2; // in the middle of the window in WIDTH
    spritePosition.y = SCREEN_HEIGHT - LAUNCHER_HEIGHT ; // at the bottom in HEIGHT


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

        if (bub_t_ptr->isMoving)
            bub_move (bub_t_ptr) ;

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
        SDL_Rect spriteImage ;
        spriteImage.w = LAUNCHER_WIDTH ;
        spriteImage.h = LAUNCHER_HEIGHT ;
        spriteImage.x = 0 ; // the image is moved in height, not in width
        spriteImage.y = LAUNCHER_HEIGHT * currentOrientation ;

        SDL_BlitSurface(launcher_srf_ptr, &spriteImage, screen_srf_ptr, &spritePosition) ;

	    // draw the bubble
	    SDL_Rect bubImage ;
	    bubImage.w = BUB_SIZE ;
	    bubImage.h = BUB_SIZE ;
	    bubImage.x = 0 ;
	    bubImage.y = 0 ;
	
	    SDL_BlitSurface(bub_t_ptr->sprite_ptr, &bubImage, screen_srf_ptr, &(bub_t_ptr->position)) ;


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
