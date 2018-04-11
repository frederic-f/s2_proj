#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "constants.h"
#include "game.h"
#include "bub.h"


/* function that receives [i=lig][j=col] of a cell from the bubs_array
 * returns a _ptr to SDL_Rect object with coords /x and y OF TOP LEFT CORNER/
 * so that main program can position the bub
 * WARNING : coords are for top left corner */
SDL_Rect * getBubPositionRect(int i, int j, SDL_Rect * dumRect_ptr) {

    /* distance between each bub */
    int d_x = (BOARD_RIGHT - BOARD_LEFT) / 8;

    /* there are 8 bubs on even rows
     * there are 7 bubs on odd rows
     * for odd rows (2d option of ternary op) we add a shift to the right */
    dumRect_ptr->x = (i % 2 == 0) ? BOARD_LEFT + j*d_x : BOARD_LEFT + j*d_x + BUB_SIZE / 2;

    dumRect_ptr->y = BOARD_TOP + (35 * i) ; //35 because 40 * sqrt(3)/2 = 35 and with that bubs are close to each other

    return dumRect_ptr ;
}



void HandleEvent(SDL_Event event,
                 int * quit, int * currOrientation, bub_t * bub_t_ptr)
{
    switch (event.type) {
        /* close button  clicked */
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



void fatal (char *message) {
    char error_message[100] ;

    strcpy (error_message, "[!!] Fatal Error\n") ;
    strncat (error_message, message, 83) ;
    perror (error_message) ;
    //exit (-1) ;
}