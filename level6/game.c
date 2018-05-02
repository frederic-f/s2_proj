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
*
* ************************************************************************************************************** */
int game_init (game_t * game_t_ptr, sys_t * sys_t_ptr) {

    bool debug = true ;

    int i ; /* for malloc loops */

    game_t_ptr->bub_ny = BUB_NY ;

    if (debug) {
        printf ("**game initialized\n\n") ;
    }

    /* Load all Sprites */
    game_loadSprites (game_t_ptr, sys_t_ptr) ;

    /* launcher is vertical by default */
    game_t_ptr->launcherOrientation = 22 ;

    /* gameover */
    game_t_ptr -> quit = 0 ;


    /* bubs_array*/

    /* Initializes bubs_array */
    /* non-moving bubs /presence/ are kept track of in a pointer-style 2-dimension array */
    game_t_ptr->bubs_array = (int * *) malloc (BUB_NY * sizeof (int *)) ;

    for (i = 0 ; i < BUB_NY ; i += 1) {

        game_t_ptr->bubs_array[i] = (int *) malloc(BUB_NX * sizeof(int)) ;
    }

    /* Reset bubs_array = removes all bubs from screen */
    game_resetBubsArray (game_t_ptr) ;


    /* bub_array_center*/
    game_setBubsArrayCenters (game_t_ptr) ;


    /* bub_connected_component */

    /* Connected components*/
    game_t_ptr->bub_connected_component = (int * *) malloc (BUB_NY * sizeof (int *)) ;

    for (i = 0 ; i < BUB_NY ; i += 1) {

        game_t_ptr->bub_connected_component[i] = (int *) malloc(BUB_NX * sizeof(int));
    }

    /* bub_fifo */

    /* Queue for connexity*/
    game_t_ptr->bub_fifo = (int * *) malloc ((BUB_NY * BUB_NX) * sizeof (int *)) ;

    for (i = 0 ; i < (BUB_NY * BUB_NX) ; i += 1) {

        game_t_ptr->bub_fifo[i] = (int *) malloc (2 * sizeof (short)) ;
    }


    /* bubs falling */
    game_t_ptr->bub_fallingBubs = (struct Bub_t * *) malloc ((BUB_NY * BUB_NX) * sizeof (struct Bub_t *)) ;

    for (i = 0 ; i < (BUB_NY * BUB_NX) ; i += 1) {

        game_t_ptr->bub_fallingBubs[i] = (struct Bub_t *) malloc (sizeof (struct Bub_t)) ;
        /* bub position */
        game_t_ptr->bub_fallingBubs[i]->position = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;
    }


    game_t_ptr->bub_numFallingBubs = 0 ;


    /* reset bub_connected_component to 0
     * and set head and tail to 0*/
    game_resetConnexity (game_t_ptr) ;


    return (0) ;
}



int game_newGame (game_t * game_t_ptr, bub_t * bub_t_ptr) {

    /* launcher is vertical by default */
    game_t_ptr->launcherOrientation = 22 ;

    game_resetBubsArray (game_t_ptr) ;

    bub_init (bub_t_ptr, game_t_ptr) ;

    return (0) ;
}

/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_loadSprites (game_t * game_t_ptr, sys_t * sys_t_ptr) {

    SDL_Surface *temp ;

    /* Loading the BUBS */
    temp = SDL_LoadBMP("img/bub_black.bmp");
    game_t_ptr->bubs[0] = SDL_DisplayFormat(temp) ;
    temp = SDL_LoadBMP("img/bubex_black.bmp");
    game_t_ptr->bubsEx[0] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_blue.bmp");
    game_t_ptr->bubs[1] = SDL_DisplayFormat(temp) ;
    temp = SDL_LoadBMP("img/bubex_blue.bmp");
    game_t_ptr->bubsEx[1] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_green.bmp");
    game_t_ptr->bubs[2] = SDL_DisplayFormat(temp) ;
    temp = SDL_LoadBMP("img/bubex_green.bmp");
    game_t_ptr->bubsEx[2] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_orange.bmp");
    game_t_ptr->bubs[3] = SDL_DisplayFormat(temp) ;
    temp = SDL_LoadBMP("img/bubex_orange.bmp");
    game_t_ptr->bubsEx[3] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_purple.bmp");
    game_t_ptr->bubs[4] = SDL_DisplayFormat(temp) ;
    temp = SDL_LoadBMP("img/bubex_purple.bmp");
    game_t_ptr->bubsEx[4] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_red.bmp");
    game_t_ptr->bubs[5] = SDL_DisplayFormat(temp) ;
    temp = SDL_LoadBMP("img/bubex_red.bmp");
    game_t_ptr->bubsEx[5] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_white.bmp");
    game_t_ptr->bubs[6] = SDL_DisplayFormat(temp) ;
    temp = SDL_LoadBMP("img/bubex_white.bmp");
    game_t_ptr->bubsEx[6] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_yellow.bmp");
    game_t_ptr->bubs[7] = SDL_DisplayFormat(temp) ;
    temp = SDL_LoadBMP("img/bubex_yellow.bmp");
    game_t_ptr->bubsEx[7] = SDL_DisplayFormat(temp) ;


    SDL_FreeSurface(temp) ;


    /* make sprites transparent */
    int k ;
    for (k = 0 ; k < NUM_COLOR ; k += 1) {
        sys_makeTransparent (sys_t_ptr, game_t_ptr->bubs[k]);
        sys_makeTransparent (sys_t_ptr, game_t_ptr->bubsEx[k]);
    }


    return (0) ;
}


/* ****************************************************************************************************************
*
* **************************************************1.0************************************************************ */
int game_resetBubsArray (game_t * game_t_ptr) {

    bool debug = false ;

    int i, j ;

    for (i = 0 ; i < BUB_NY ; i += 1) {

        /* number of bubs in a row depends on odd/even number of row */
        int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

        for (j = 0 ; j < j_max ; j +=1 ) {

            int col = j + getRandomNumber(NUM_COLOR) ;
            col = (col > 8) ? 8 : col ;
            //col = 6 ; //  0-black 1-blue 2-green 3-orange 4-pourpre 5-red 6-whi 7-yell

            /* set whole lines of bubs here if you want to test */
            game_t_ptr->bubs_array[i][j] = (i==0 || i == 1 || i == 2) ? col : 0 ;

            /* set all bubs to value */
            //game_t_ptr->bubs_array[i][j] = 7 ;
        }
    }


    if (debug) {

        printf ("Values of bubs_array \n") ;

        for (i = 0 ; i < BUB_NY ; i += 1) {

            /* number of bubs in a row depends on odd/even number of row */
            int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

            for (j = 0 ; j < j_max ; j +=1 ) {

                printf ("Bubs array [ %d ] [ %d ] : %d\n",i, j, game_t_ptr->bubs_array[i][j]) ;

            }
        }
    }

    return (0) ;
}


/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_setBubsArrayCenters (game_t * game_t_ptr) {

    /* Initializes bub_array_centers */

    /* Load bub_array_centers */
    /* all possible spaces for a bub /centers coordinates/ are kept track of in a pointer-syle 3-dimension array */
    game_t_ptr->bub_array_centers = (int * * *) malloc (BUB_NY * sizeof (int * *)) ;

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
            rectForCenters_ptr = sys_getBubPositionRect(i, j, rectForCenters_ptr) ;

            /* and add BUB_SIZE/2 to get coords of centers */
            game_t_ptr->bub_array_centers[i][j][0] = rectForCenters_ptr->x + BUB_SIZE/2 ;
            game_t_ptr->bub_array_centers[i][j][1] = rectForCenters_ptr->y + BUB_SIZE/2 ;
        }
    }

    free (rectForCenters_ptr) ;

    return (0) ;
}


/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_resetBubConnectedComponent (game_t * game_t_ptr) {

    bool debug = false ;

    int i, j ;

    for (i = 0 ; i < BUB_NY ; i += 1) {

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

    return (0) ;
}

/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_resetConnexity (game_t * game_t_ptr) {

    game_t_ptr->fifoTail = 0 ;
    game_t_ptr->fifoHead = 0 ;

    game_resetBubConnectedComponent (game_t_ptr) ;

    return (0) ;
}

/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_cleanBoard (game_t * game_t_ptr, SDL_Rect * bubJustPlaced_rect) {

    bool debug = false ;

    if (debug) {

        printf("**CLEANING BOARD \n\n");
        printf("  Coordinates just received : line = %d, col = %d\n", bubJustPlaced_rect->y, bubJustPlaced_rect->x);
        printf("  Starting COLOR connexity check\n");
    }

    /* color-connexity check with bub that arrived */
    /* will update fifo */
    game_checkConnexity(game_t_ptr, bubJustPlaced_rect, true);

    if (debug) {
        printf("**Color-Connexity checked\n");
        printf("  Number of bubs to delete : %d \n", game_t_ptr->fifoHead);
    }

    /* Clean according to color-connexity
     * if 3 bubs or more are connected...*/
    if (game_t_ptr->fifoHead >= 3) {



        /* delete these bubs */
        int i ;
        for (i = 0; i < game_t_ptr->fifoHead; i += 1) {

            //printf ("[game_clean] Deleted bub at y=%d x=%d\n", game_t_ptr->bub_fifo[i][0], game_t_ptr->bub_fifo[i][1]) ;

            /* add bub to fallingBubs*/
            /* create bub_t pointer */


            /* TODO game_addFallingBub (game_t_ptr, color, rect_ptr, bool isExploding) */

            struct Bub_t * bub_ptr = game_t_ptr->bub_fallingBubs[game_t_ptr->bub_numFallingBubs] ;

            /* get color from bubs_array*/
            bub_ptr->color = game_t_ptr->bubs_array[game_t_ptr->bub_fifo[i][0]][game_t_ptr->bub_fifo[i][1]] ;
            bub_setSpriteNormal (game_t_ptr, bub_ptr) ;

            /* get the coordinates of bub */
            SDL_Rect * rect_ptr = (SDL_Rect *) malloc (sizeof(SDL_Rect)) ;

            rect_ptr = sys_getBubPositionRect(game_t_ptr->bub_fifo[i][0], game_t_ptr->bub_fifo[i][1], rect_ptr) ;

            bub_ptr->position = rect_ptr ;

            bub_ptr->isExploding = true ;

            /* update number of falling bubs */
            game_t_ptr->bub_numFallingBubs += 1 ;

            /* delete bub from bubs_array */
            game_t_ptr->bubs_array[game_t_ptr->bub_fifo[i][0]][game_t_ptr->bub_fifo[i][1]] = 0 ;
        }

    }

    /* reset head, tail, bub_connected */
    game_resetConnexity (game_t_ptr);


    /* roof-connexity check with all bubs on roof */

    /* memory allocation for temp variables */

    /* this temp will hold the current coordinates of spot whose connexity is checked */
    SDL_Rect * bubCoord_rect = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    /* this temp will hold the neighboring bubs */
    bub_t * bub_t_top_ptr = (bub_t *) malloc (sizeof (bub_t)) ;

    /* we keep the address in memory to re-assign it after NULL is assigned to it by function game_getBubAt */
    bub_t * bub_t_top_ptr_2 = bub_t_top_ptr ;

    int i, j ;

    bubCoord_rect->y = 0 ;

    /* for each bub on roof */
    for (j = 0 ; j < BUB_NX ; j += 1) {

        bubCoord_rect->x = j ;

        /* if there is a bub */
        if ((bub_t_top_ptr = game_getBubAt (game_t_ptr, bub_t_top_ptr, bubCoord_rect)) != NULL) {

            if (debug)
                printf("  Roof bub at x = %d. Calling checkConnexity() on that bub\n", j);

            /* check connexity */
            game_checkConnexity (game_t_ptr, bubCoord_rect, false) ;
        }

        /* reassign pointer to malloc address (in case previous function returned NULL pointer) */
        bub_t_top_ptr = bub_t_top_ptr_2 ;
    }


    if (debug) {

        printf ("\n\n **Values of ROOF bub_connected_components \n") ;

        for (i = 0 ; i < BUB_NY ; i += 1) {

            /* number of bubs in a row depends on odd/even number of row */
            int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

            for (j = 0 ; j < j_max ; j +=1 ) {

                printf ("Connected components [ %d ] [ %d ] : %d\n",i, j,  game_t_ptr->bub_connected_component[i][j]) ;

            }
        }

        printf ("Values of bubs_array \n") ;

        for (i = 0 ; i < BUB_NY ; i += 1) {

            /* number of bubs in a row depends on odd/even number of row */
            int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

            for (j = 0 ; j < j_max ; j +=1 ) {

                printf ("Bubs array [ %d ] [ %d ] : %d\n", i, j, game_t_ptr->bubs_array[i][j]) ;

            }
        }
    }




    /* delete non-connected bubs */

    /* loop through bub_array
     * keep to 1 only the bubs in connexity table */
    for (i = 0 ; i < BUB_NY ; i += 1) {

        /* number of bubs in a row depends on odd/even number of row */
        int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

        for (j = 0 ; j < j_max ; j +=1 ) {

            /* if bub in bubs_array is present (1)
             * but is not in connexity table
             * we delete it (0) */
            if ((game_t_ptr->bubs_array[i][j] > 0) && (game_t_ptr->bub_connected_component[i][j] == 0) ) {

                game_t_ptr->bubs_array[i][j] = 0 ;

                /* TODO : add bub to fallingBubs */
            }
        }
    }

    /* reset head, tail, bub_connected */
    game_resetConnexity (game_t_ptr) ;

    /* free pointers */
    free (bubCoord_rect) ;
    free (bub_t_top_ptr) ;

    return (0) ;
}



/* ****************************************************************************************************************
*   Connexity function used for :
 *   color-connexity (colorConnexity == true)
 *   roof-connexity (colorConnexity == false)
* ************************************************************************************************************** */
int game_checkConnexity (game_t * game_t_ptr, SDL_Rect * bubJustPlaced_rect, bool colorConnexity) {

    bool debug = false ;

    if (debug) {

        if (colorConnexity) {
            printf("**\nCOLOR-Connexity\n\n");
        }
        else {
            printf("**\nROOF-Connexity\n\n");
        }

        printf("  Coordinates just received : line = %d, col = %d\n", bubJustPlaced_rect->y, bubJustPlaced_rect->x);
        printf("  Starting connexity check\n");

        printf("**Values of bub_connected_components \n");
    }

    /* if bub we just placed can be added ... */
    if (!game_addBubConnected (game_t_ptr, bubJustPlaced_rect)) {

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

            /* dequeue right away */
            game_t_ptr->fifoTail += 1 ;

            /* get all the neighbours */

            if ((bubCoord_rect->y % 2) == 0) { // bub is on even row

                if (debug)
                    printf("  Current bub on even row\n");

                /* 1st neighbour, 1 o'clock*/
                bubCoord_rect->y -= 1 ;

                game_spotCheckConnexity(game_t_ptr, bub_t_neighbour_ptr, bub_t_neighbour_ptr_2, bubCoord_rect, colorConnexity, colorForConnexity) ;


                /* 2nd neighbour, 3 o'clock*/
                bubCoord_rect->x += 1 ;
                bubCoord_rect->y += 1 ;

                game_spotCheckConnexity(game_t_ptr, bub_t_neighbour_ptr, bub_t_neighbour_ptr_2, bubCoord_rect, colorConnexity, colorForConnexity) ;


                /* 3rd neighbour, 5 o'clock*/
                bubCoord_rect->x -= 1 ;
                bubCoord_rect->y += 1 ;

                game_spotCheckConnexity(game_t_ptr, bub_t_neighbour_ptr, bub_t_neighbour_ptr_2, bubCoord_rect, colorConnexity, colorForConnexity) ;


                /* 4th neighbour, 7 o'clock*/
                bubCoord_rect->x -= 1 ;

                game_spotCheckConnexity(game_t_ptr, bub_t_neighbour_ptr, bub_t_neighbour_ptr_2, bubCoord_rect, colorConnexity, colorForConnexity) ;


                /* 5th neighbour, 9 o'clock*/
                bubCoord_rect->y -= 1 ;

                game_spotCheckConnexity(game_t_ptr, bub_t_neighbour_ptr, bub_t_neighbour_ptr_2, bubCoord_rect, colorConnexity, colorForConnexity) ;


                /* 6th neighbour, 11 o'clock*/
                bubCoord_rect->y -= 1 ;

                game_spotCheckConnexity(game_t_ptr, bub_t_neighbour_ptr, bub_t_neighbour_ptr_2, bubCoord_rect, colorConnexity, colorForConnexity) ;

            }

            else {

                if (debug) {
                    printf ("  Current bub on odd row\n") ;
                }

                /* 1st neighbour, 1 o'clock*/
                bubCoord_rect->x += 1 ;
                bubCoord_rect->y -= 1 ;

                game_spotCheckConnexity(game_t_ptr, bub_t_neighbour_ptr, bub_t_neighbour_ptr_2, bubCoord_rect, colorConnexity, colorForConnexity) ;


                /* 2nd neighbour, 3 o'clock*/
                bubCoord_rect->y += 1 ;

                game_spotCheckConnexity(game_t_ptr, bub_t_neighbour_ptr, bub_t_neighbour_ptr_2, bubCoord_rect, colorConnexity, colorForConnexity) ;


                /* 3rd neighbour, 5 o'clock*/
                bubCoord_rect->y += 1 ;

                game_spotCheckConnexity(game_t_ptr, bub_t_neighbour_ptr, bub_t_neighbour_ptr_2, bubCoord_rect, colorConnexity, colorForConnexity) ;


                /* 4th neighbour, 7 o'clock*/
                bubCoord_rect->x -= 1 ;

                game_spotCheckConnexity(game_t_ptr, bub_t_neighbour_ptr, bub_t_neighbour_ptr_2, bubCoord_rect, colorConnexity, colorForConnexity) ;


                /* 5th neighbour, 9 o'clock*/
                bubCoord_rect->x -= 1 ;
                bubCoord_rect->y -= 1 ;

                game_spotCheckConnexity(game_t_ptr, bub_t_neighbour_ptr, bub_t_neighbour_ptr_2, bubCoord_rect, colorConnexity, colorForConnexity) ;


                /* 6th neighbour, 11 o'clock*/
                bubCoord_rect->x += 1 ;
                bubCoord_rect->y -= 1 ;

                game_spotCheckConnexity(game_t_ptr, bub_t_neighbour_ptr, bub_t_neighbour_ptr_2, bubCoord_rect, colorConnexity, colorForConnexity) ;
	        }
        }

        free (bub_t_neighbour_ptr) ;
        free (bubCoord_rect) ;
    }

    return (0) ;
}

/* ****************************************************************************************************************
*   gets the coordinate of an hypthetical bub
 *   if there is a bub, and the bub matches the conditions : bub is added to connexity table
* ************************************************************************************************************** */
int game_spotCheckConnexity (game_t * game_t_ptr, bub_t * bub_t_neighbour_ptr, bub_t * bub_t_neighbour_ptr_2, SDL_Rect * bubCoord_rect, bool colorConnexity, short colorForConnexity) {

    bool debug = false ;

    /* if bub exists ...*/
    if ((bub_t_neighbour_ptr = game_getBubAt (game_t_ptr, bub_t_neighbour_ptr, bubCoord_rect)) != NULL) {
        if (debug)
            printf ("  Neigh exists. Color = %d\n", bub_t_neighbour_ptr->color) ;

        /* we check the conditions */

        /* if roof-connexity (colorConnexity == false) */
        if (!colorConnexity) {
            /* ... we add it to the queue*/
            game_addBubConnected (game_t_ptr, bubCoord_rect) ;

            return (0) ;
        }

        /* OR color-connexity and colors match... */
        if ((colorConnexity) && (bub_t_neighbour_ptr->color == colorForConnexity)) {
            //printf ("    same color!") ;

            /* ... we add it to the queue*/
            game_addBubConnected (game_t_ptr, bubCoord_rect) ;

            return (0) ;
        }
    }
    else {
        bub_t_neighbour_ptr = bub_t_neighbour_ptr_2 ;

        if (debug) {
            printf ("  Neigh DONT exists\n") ;
        }
    }

    return (0) ;
}



/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_addBubConnected (game_t * game_t_ptr, SDL_Rect * bubJustPlaced_rect) {

    bool debug = false ;

    if (debug) {
        printf("**Adding bub to bub_connected line %d, col %d\n", bubJustPlaced_rect->y, bubJustPlaced_rect->x) ;
    }

    /* if the bub was not already added ... */
    if (game_t_ptr->bub_connected_component[bubJustPlaced_rect->y][bubJustPlaced_rect->x]==0) {

        /* add to bub_connected_component */
        game_t_ptr->bub_connected_component[bubJustPlaced_rect->y][bubJustPlaced_rect->x] = 1 ;

        /* add to queue for connexity processing */
        game_t_ptr->bub_fifo[game_t_ptr->fifoHead][0] = bubJustPlaced_rect->y ;
        game_t_ptr->bub_fifo[game_t_ptr->fifoHead][1] = bubJustPlaced_rect->x ;

        game_t_ptr->fifoHead += 1 ;

        if (debug) {
            printf ("**Bub added to queue. Head is now : %d \n", game_t_ptr->fifoHead) ;
        }
    }

    return (0) ;
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

    /* Y coordinate : if y < 0 (beyond roof)
     * or y == bub_ny (beyond bottom line)
     * -> NULL */
    if ((rect_ptr->y < 0) || (rect_ptr->y == game_t_ptr->bub_ny)) {
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

        /* if so, return bub_t_ptr
         * with updated  Position_x, Position_y and color */

        bub_t_neighbour_ptr->color = game_t_ptr->bubs_array[rect_ptr->y][rect_ptr->x] ;

        return bub_t_neighbour_ptr ;
    }
    else {

        /* there is no bub */
        return NULL ;
    }
}

/* ****************************************************************************************************************
*
* ************************************************************************************************************** */
int game_moveFallingBub (game_t * game_t_ptr) {

    /* move each falling bub until it disappears below screen */
    int i ;

    for (i = 0; i < game_t_ptr->bub_numFallingBubs ; i += 1) {

        /* slow down the fall */
        if (i % 1 == 0) {

            game_t_ptr->bub_fallingBubs[i]->position->y += 1;
        }

        /* if the bub reach limit */
        if (game_t_ptr->bub_fallingBubs[i]->position->y > SCREEN_HEIGHT) {

            /* we delete this bub */

            /* we replace the bub with the last bub in array */
            game_t_ptr->bub_fallingBubs[i] = game_t_ptr->bub_fallingBubs[game_t_ptr->bub_numFallingBubs - 1] ;

            /* we decrement falling bub count */
            game_t_ptr->bub_numFallingBubs -= 1 ;
        }
    }
    return (0) ;
}