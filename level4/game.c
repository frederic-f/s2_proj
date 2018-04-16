#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include "data.h"
#include "game.h"
#include "bub.h"


/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_init (game_t * game_t_ptr) {

    bool debug = true ;

    if (debug) {
        printf ("game initialized\n") ;
    }

    /* Load all Sprites */
    if (!game_loadSprites (game_t_ptr))
        fatal ("Could not load Sprites") ;

    /* Initializes bubs_array */
    /* non-moving bubs /presence/ are kept track of in a pointer-style 2-dimension array */
    game_t_ptr->bubs_array = (int * *) malloc (BUB_NY * sizeof(int *)) ;

    /* Reset bubs_array = removes all bubs from screen */
    game_resetBubsArray (game_t_ptr) ;

    /* Initializes bub_array_centers */
    /* all possible spaces for a bub /centers coordinates/ are kept track of in a pointer-syle 3-dimension array */
    game_t_ptr->bub_array_centers = (int * * *) malloc (BUB_NY * sizeof(int * *)) ;

    /* Load bub_array_centers */
    game_setBubsArrayCenters (game_t_ptr) ;

    return (1) ;
}


/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_loadSprites (game_t * game_t_ptr) {

    SDL_Surface *temp ;

    /* Loading the BUBS */
    temp = SDL_LoadBMP("img/bub_black.bmp");
    game_t_ptr->bubs[0] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_blue.bmp");
    game_t_ptr->bubs[1] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_green.bmp");
    game_t_ptr->bubs[2] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_orange.bmp");
    game_t_ptr->bubs[3] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_purple.bmp");
    game_t_ptr->bubs[4] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_red.bmp");
    game_t_ptr->bubs[5] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_white.bmp");
    game_t_ptr->bubs[6] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_yellow.bmp");
    game_t_ptr->bubs[7] = SDL_DisplayFormat(temp) ;


    SDL_FreeSurface(temp) ;

    return (1) ;
}


/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_resetBubsArray (game_t * game_t_ptr) {

    bool debug = false ;

    int i, j ;

    for (i = 0 ; i < BUB_NY ; i += 1) {

        game_t_ptr->bubs_array[i] = (int *) malloc (BUB_NX * sizeof(int)) ;

        /* number of bubs in a row depends on odd/even number of row */
        int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

        for (j = 0 ; j < j_max ; j +=1 ) {

            /* set whole lines of bubs here if you want to test */
            //bubs_array[i][j] = (i==0 || i == 1 || i == 2) ? 1 : 0 ;

            /* set all bubs to value */
            game_t_ptr->bubs_array[i][j] = 0 ;
        }
    }


    if (debug) {
        //printf("Check x = %d\n", bub_array_centers[1][0][0]);
        //printf("Check y = %d\n", bub_array_centers[1][0][1]);

        printf ("Values of bubs_array \n") ;

        for (i = 0 ; i < BUB_NY ; i += 1) {

            /* number of bubs in a row depends on odd/even number of row */
            int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

            for (j = 0 ; j < j_max ; j +=1 ) {

                printf ("Value %d\n", game_t_ptr->bubs_array[i][j]) ;

            }
        }
    }

    return (1) ;
}


/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_setBubsArrayCenters (game_t * game_t_ptr) {

    SDL_Rect * rectForCenters_ptr = (SDL_Rect *) malloc (sizeof(SDL_Rect)) ;

    int i, j ;

    for (i = 0 ; i < BUB_NY ; i += 1) {

        game_t_ptr->bub_array_centers[i] = (int * *) malloc (BUB_NX * sizeof(int *)) ;

        /* number of bubs in a row depends on odd/even number of row */
        int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

        for (j = 0 ; j < j_max ; j +=1 ) {

            /* array centers */
            game_t_ptr->bub_array_centers[i][j] = (int *) malloc (2 * sizeof(int)) ;

            /* we use the function to get coords of TOP LEFT corner */
            rectForCenters_ptr = getBubPositionRect(i, j, rectForCenters_ptr) ;

            /* and add BUB_SIZE/2 to get coords of centers */
            game_t_ptr->bub_array_centers[i][j][0] = rectForCenters_ptr->x + BUB_SIZE/2 ;
            game_t_ptr->bub_array_centers[i][j][1] = rectForCenters_ptr->y + BUB_SIZE/2 ;
        }
    }

    free (rectForCenters_ptr) ;

    return (1) ;
}


/* ****************************************************************************************************************
* SYSTEM functions
* ************************************************************************************************************** */


/* ****************************************************************************************************************
* function that receives [i=lig][j=col] of a cell from the bubs_array
* returns a _ptr to SDL_Rect object with coords /x and y OF TOP LEFT CORNER/
* so that main program can position the bub
* WARNING : coords are for top left corner
* ************************************************************************************************************** */

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



/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
short giveRandomNumber() {

    time_t t ;

    /* Initializes random number generator */
    srand ((unsigned) time (&t)) ;

    /* Generates numbers from 0 to NUM_COLOR */
    return rand() % NUM_COLOR ;
}



/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
void HandleEvent (SDL_Event event, int * quit, int * currOrientation, bub_t * bub_t_ptr)
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