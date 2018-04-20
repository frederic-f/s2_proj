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

    game_t_ptr->bub_ny = BUB_NY ;

    if (debug) {
        printf ("**game initialized\n\n") ;
    }

    /* Load all Sprites */
    if (!game_loadSprites (game_t_ptr))
        fatal ("!!Could not load Sprites") ;


    /* bubs_array*/

    /* Initializes bubs_array */
    /* non-moving bubs /presence/ are kept track of in a pointer-style 2-dimension array */
    game_t_ptr->bubs_array = (int * *) malloc (BUB_NY * sizeof (int *)) ;

    /* Reset bubs_array = removes all bubs from screen */
    game_resetBubsArray (game_t_ptr) ;


    /* bub_array_center*/

    /* Initializes bub_array_centers */
    /* all possible spaces for a bub /centers coordinates/ are kept track of in a pointer-syle 3-dimension array */
    game_t_ptr->bub_array_centers = (int * * *) malloc (BUB_NY * sizeof (int * *)) ;

    /* Load bub_array_centers */
    game_setBubsArrayCenters (game_t_ptr) ;


    /* bub_connected_component */

    /* Connected components*/
    game_t_ptr->bub_connected_component = (int * *) malloc (BUB_NY * sizeof (int *)) ;

    /* set bub_connected_component */
    game_setBubConnectedComponent (game_t_ptr) ;


    /* bub_fifo */

    /* Queue for connexity*/
    game_t_ptr->bub_fifo = (int * *) malloc ((BUB_NY * BUB_NX) * sizeof (int *)) ;

    /* set bub_connected_component */
    game_setBubFifo (game_t_ptr) ;

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
* **************************************************1.0************************************************************ */
int game_resetBubsArray (game_t * game_t_ptr) {

    bool debug = false ;

    int i, j ;

    for (i = 0 ; i < BUB_NY ; i += 1) {

        game_t_ptr->bubs_array[i] = (int *) malloc (BUB_NX * sizeof(int)) ;

        /* number of bubs in a row depends on odd/even number of row */
        int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

        for (j = 0 ; j < j_max ; j +=1 ) {

            int col = j + getRandomNumber(NUM_COLOR) ;
            col = (col > 8) ? 8 : col ;

            /* set whole lines of bubs here if you want to test */
            game_t_ptr->bubs_array[i][j] = (i==0 || i == 1 || i == 2) ? col : 0 ;

            /* set all bubs to value */
            //game_t_ptr->bubs_array[i][j] = 0 ;
        }
    }


    if (debug) {

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
*
* ************************************************************************************************************** */
int game_setBubConnectedComponent (game_t * game_t_ptr) {

    bool debug = false ;

    int i, j ;

    for (i = 0 ; i < BUB_NY ; i += 1) {

        game_t_ptr->bub_connected_component[i] = (int *) malloc (BUB_NX * sizeof(int)) ;

        /* number of bubs in a row depends on odd/even number of row */
        int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

        for (j = 0 ; j < j_max ; j +=1 ) {

            int col = j + getRandomNumber(NUM_COLOR) ;
            col = (col > 8) ? 8 : col ;

            /* set whole lines of bubs here if you want to test */
            game_t_ptr->bub_connected_component[i][j] = 0 ;
        }
    }


    if (debug) {

        printf ("Values of bub_connected_components \n") ;

        for (i = 0 ; i < BUB_NY ; i += 1) {

            /* number of bubs in a row depends on odd/even number of row */
            int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

            for (j = 0 ; j < j_max ; j +=1 ) {

                printf ("Connected components [ %d ] [ %d ] : %d\n",i, j,  game_t_ptr->bub_connected_component[i][j]) ;

            }
        }
    }

    return (1) ;
}


/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_setBubFifo (game_t * game_t_ptr) {

    int i ;

    for (i = 0 ; i < (BUB_NY * BUB_NX) ; i += 1) {

        game_t_ptr->bub_connected_component[i] = (int *) malloc (2 * sizeof (short)) ;
    }

    game_t_ptr->fifoHead = 0 ;
    game_t_ptr->fifoTail = 0 ;

    return (1) ;
}


/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_checkConnexityColor (game_t * game_t_ptr, SDL_Rect * bubJustPlaced_rect) {

    bool debug = true ;

    if (debug) { 

	    printf ("**Connexity check \n\n") ;
	    printf ("  Coordinates just received : line = %d, col = %d\n", bubJustPlaced_rect->y, bubJustPlaced_rect->x) ;
	    printf ("  Starting connexity check\n") ;
        }
	
    /* if bub we just placed can be added ... */
    if (game_addBubConnected (game_t_ptr, bubJustPlaced_rect)) {

        /* ... then we check the new network of connexity */

        /* memory allocation for temp variables */

        /* this temp will hold the current coordinates of spot whose connexity is checked */
        SDL_Rect * bubCoord_rect = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

	    /* this temp will hold the neighboring bubs */
	    bub_t * bub_t_neighbour_ptr = (bub_t *) malloc (sizeof (bub_t)) ;
	    /* we keep the address in memory to re-assign it after NULL is assigned to it by function game_getBubAt */
        bub_t * bub_t_neighbour_ptr_2 = bub_t_neighbour_ptr ;

        /* get the color for connexity */
        bub_t_neighbour_ptr = game_getBubAt(game_t_ptr, bub_t_neighbour_ptr, bubJustPlaced_rect) ;
        short colorForConnexity = bub_t_neighbour_ptr->color ;

        /* loop to explore connexity */

        /* while the queue is not empty...*/
        while (game_t_ptr->fifoHead != game_t_ptr->fifoTail) {

            /* get coordinates of next bub in queue */
            bubCoord_rect->y = game_t_ptr->bub_fifo[game_t_ptr->fifoTail][0] ;
            bubCoord_rect->x = game_t_ptr->bub_fifo[game_t_ptr->fifoTail][1] ;

            /* get all the neighbours */

            if ((bubCoord_rect->y % 2) == 0) { // bub is on even row
                if (debug)
                    printf("  Current bub on even row\n");
            }

            else {

                if (debug) {
                    printf ("  Current bub on odd row\n") ;
                }

                /* 1st neighbour, 1 o'clock*/
                bubCoord_rect->x += 1 ;
                bubCoord_rect->y -= 1 ;

                printf ("  1st neigh coord %d %d\n", bubCoord_rect->x, bubCoord_rect->y) ;

                if ((bub_t_neighbour_ptr = game_getBubAt (game_t_ptr, bub_t_neighbour_ptr, bubCoord_rect)) != NULL) {
                    printf ("  1st neigh exists. Color = %d\n", bub_t_neighbour_ptr->color) ;

                    /* if the color is the same... */
                    if (bub_t_neighbour_ptr->color == colorForConnexity) {
                        printf ("    same color!") ;

                        /* ... we add it to the queue*/


                    }
                }
                else {
                    bub_t_neighbour_ptr = bub_t_neighbour_ptr_2 ;
                    printf ("  1st neigh DONT exists\n") ;
                }

                /* 2nd neighbour, 3 o'clock*/
                bubCoord_rect->y += 1 ;

                if ((bub_t_neighbour_ptr = game_getBubAt (game_t_ptr, bub_t_neighbour_ptr, bubCoord_rect)) != NULL) {
                    printf ("  2nd neigh exists. Color = %d\n", bub_t_neighbour_ptr->color) ;

                    /* we check the color */
                    if (bub_t_neighbour_ptr->color == colorForConnexity) {
                        printf ("    same color!") ;
                    }
                }
                else {
                    bub_t_neighbour_ptr = bub_t_neighbour_ptr_2 ;
                    printf ("  2nd neigh DONT exists\n") ;
                }

                /* 3rd neighbour, 5 o'clock*/
                bubCoord_rect->y += 1 ;

                if ((bub_t_neighbour_ptr = game_getBubAt (game_t_ptr, bub_t_neighbour_ptr, bubCoord_rect)) != NULL) {
                    printf ("  3rd neigh exists. Color = %d\n", bub_t_neighbour_ptr->color) ;

                    /* we check the color */
                    if (bub_t_neighbour_ptr->color == colorForConnexity) {
                        printf ("    same color!") ;
                    }
                }
                else {
                    bub_t_neighbour_ptr = bub_t_neighbour_ptr_2 ;
                    printf ("  3rd neigh DONT exists\n") ;
                }

                /* 4th neighbour, 7 o'clock*/
                bubCoord_rect->x -= 1 ;

                if ((bub_t_neighbour_ptr = game_getBubAt (game_t_ptr, bub_t_neighbour_ptr, bubCoord_rect)) != NULL) {
                    printf ("  4th neigh exists. Color = %d\n", bub_t_neighbour_ptr->color) ;

                    /* we check the color */
                    if (bub_t_neighbour_ptr->color == colorForConnexity) {
                        printf ("    same color!") ;
                    }
                }
                else {
                    bub_t_neighbour_ptr = bub_t_neighbour_ptr_2 ;
                    printf ("  4th neigh DONT exists\n") ;
                }

                /* 5th neighbour, 9 o'clock*/
                bubCoord_rect->x -= 1 ;
                bubCoord_rect->y -= 1 ;

                if ((bub_t_neighbour_ptr = game_getBubAt (game_t_ptr, bub_t_neighbour_ptr, bubCoord_rect)) != NULL) {
                    printf ("  5th neigh exists. Color = %d\n", bub_t_neighbour_ptr->color) ;

                    /* we check the color */
                    if (bub_t_neighbour_ptr->color == colorForConnexity) {
                        printf ("    same color!") ;
                    }
                }
                else {
                    bub_t_neighbour_ptr = bub_t_neighbour_ptr_2 ;
                    printf ("  5th neigh DONT exists\n") ;
                }

                /* 6th neighbour, 11 o'clock*/
                bubCoord_rect->x += 1 ;
                bubCoord_rect->y -= 1 ;

                if ((bub_t_neighbour_ptr = game_getBubAt (game_t_ptr, bub_t_neighbour_ptr, bubCoord_rect)) != NULL) {
                    printf ("  6th neigh exists. Color = %d\n", bub_t_neighbour_ptr->color) ;

                    /* we check the color */
                    if (bub_t_neighbour_ptr->color == colorForConnexity) {
                        printf ("    same color!") ;
                    }
                }
                else {
                    bub_t_neighbour_ptr = bub_t_neighbour_ptr_2 ;
                    printf ("  6th neigh DONT exists\n") ;
                }
	        }

            game_t_ptr->fifoTail += 1 ;
        }

        free (bub_t_neighbour_ptr) ;
        
    }
    
    if (debug) {
        printf ("**Connexity checked\n") ;
    }
    

    return (1) ;

}



/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_addBubConnected (game_t * game_t_ptr, SDL_Rect * bubJustPlaced_rect) {

    bool debug = true ;

    /* add to bub_connected_component */
    game_t_ptr->bub_connected_component[bubJustPlaced_rect->y][bubJustPlaced_rect->x] = 1 ;

    /* add to queue for connexity processing */
    game_t_ptr->bub_fifo[game_t_ptr->fifoHead][0] = bubJustPlaced_rect->y ;
    game_t_ptr->bub_fifo[game_t_ptr->fifoHead][1] = bubJustPlaced_rect->x ;

    game_t_ptr->fifoHead += 1 ;


    if (debug) {
	printf ("**Bub added to queue. Head is now : %d \n", game_t_ptr->fifoHead) ;
    }

    return (1) ;

}

/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
bub_t * game_getBubAt (game_t * game_t_ptr, bub_t * bub_t_neighbour_ptr, SDL_Rect * rect_ptr) {

    bool debug = false ;

    if (debug) {
        printf ("**entering getBubAt\n") ;
        printf (" returning bub at line = %d, col = %d\n", rect_ptr->y, rect_ptr->x) ;
    }

    /* first check if it is within boundaries */

    /* Y coordinate : if y < 0 or y > bub_ny : NULL */
    if ((rect_ptr->y < 0) || (rect_ptr->y > game_t_ptr->bub_ny)) {
        return NULL ;
    }

    /* X coordinate
     * if even row : min_x = 0 max_x = 7 */
    if ((rect_ptr->y % 2) == 0) {

        if ((rect_ptr->x < 0) || (rect_ptr->x > 7)) {
            return NULL ;
        }
    }
    /* if odd row : min_x = 0, max x = 6 */
    else {

        if ((rect_ptr->x < 0) || (rect_ptr->x > 6)) {
            return NULL;
        }
    }

    /* then check if there is a bub : use bubs_array */
    if (game_t_ptr->bubs_array[rect_ptr->y][rect_ptr->x] > 0) {

        /* if so return bub_t_ptr
         * with updated  Position_x, Position_y and color */

        bub_t_neighbour_ptr->color = game_t_ptr->bubs_array[rect_ptr->y][rect_ptr->x] ;

        return bub_t_neighbour_ptr ;
    }
    else {

        /* no bub */
        return NULL ;
    }
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
short getRandomNumber(int max) {


    time_t t ;

    /* Initializes random number generator */
    srand ((unsigned) time (&t)) ;

    /* Generates numbers from 0 to NUM_COLOR */
    return rand() % max ;
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