#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "data.h"
#include "bub.h"
#include "game.h"



void bub_init (bub_t * bub_t_ptr, game_t * game_t_ptr) {

    SDL_Surface *temp ;
 
    /* SDL_Surface array for color random pickup */
    SDL_Surface * bmps[8] ;
    
    //printf ("bub_t initialised") ;
    temp = SDL_LoadBMP("img/bub_black.bmp");
    bmps[0] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_blue.bmp");
    bmps[1] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_green.bmp");
    bmps[2] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_orange.bmp");
    bmps[3] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_purple.bmp");
    bmps[4] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_red.bmp");
    bmps[5] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_white.bmp");
    bmps[6] = SDL_DisplayFormat(temp) ;

    temp = SDL_LoadBMP("img/bub_yellow.bmp");
    bmps[7] = SDL_DisplayFormat(temp) ;

    /* make random choice of bub */

    bub_t_ptr->color = giveRandomNumber () ;

    bub_t_ptr->sprite_ptr = bmps[bub_t_ptr->color] ;
    //bub_t_ptr->sprite_ptr = SDL_DisplayFormat(temp) ;
    
    SDL_FreeSurface(temp) ;
    
    bub_t_ptr->isLaunching = false ;

    bub_t_ptr->isMoving = false ;

    bub_t_ptr->start_x = 720 / 2 - BUB_SIZE / 2 - 1;
    bub_t_ptr->start_y = BUB_START_Y;

    bub_getOnLauncher(bub_t_ptr, game_t_ptr) ;

}


int bub_getOnLauncher (bub_t * bub_t_ptr, game_t * game_t_ptr) {

    //DBG printf ("back on launcher\n") ;

    /* change bub color */
    bub_changeColor (bub_t_ptr, game_t_ptr) ;

    /* return bubble to launcher */
    bub_t_ptr->position.x = bub_t_ptr->start_x;
    bub_t_ptr->position.y = bub_t_ptr->start_y;

    bub_t_ptr->x = bub_t_ptr->start_x;
    bub_t_ptr->y = bub_t_ptr->start_y;

    return (0) ;
}

int bub_changeColor (bub_t * bub_t_ptr, game_t * game_t_ptr) {

    bub_t_ptr->color = giveRandomNumber() ;

    return (0) ;
}

void bub_launch (bub_t * bub_t_ptr, game_t * game_t_ptr, int *currOrientation) {

    /* launching only if bubble not moving*/
    if (!bub_t_ptr->isMoving) { // it is not moving

        /* if bub NOT on launcher (and NOT MOVING)
        * then return it to launcher */
        if (bub_t_ptr->position.y != BUB_START_Y) {

            bub_getOnLauncher(bub_t_ptr, game_t_ptr) ;

        } else {
            /* Bub is ON LAUNCHER
            * bubble starts moving from launcher*/

            /* let's calculate step_x and step_y*/

            /* x and y motions are calculated according to angle */

            /* theta angle = angle from horizontal
             * start position is PI / 2
             * we substract : (nb of orientation moved left or right (doesn't matter) from vertical) * (angle moved by each orientation of launcher)
             * */
            double theta = (PI / 2) - ( abs(*currOrientation - 22) * (PI / LAUNCHER_DIV)) ;
            //printf ("%f\n", theta) ;


            /* direction
             * to right = positive : if orientation of launcher if >= 22
             * to left -> negative : if < 22
             * */
            short direction = (*currOrientation >= 22) ? 1 : -1 ;
            //printf ("%d\n", direction) ;


            /* steps in x and y
             * reference is one unit in the direction of movement
             * x and y are calculated relative to this reference
             * */
            bub_t_ptr->step_x = cos(theta) * VELOCITY * direction ;
            bub_t_ptr->step_y = sin(theta) * VELOCITY ;
            //printf ("%f\n", step_y) ;

            /* launching the bub */
            bub_t_ptr->isMoving = true;
        }

        /* in all cases, launching is finished */
        bub_t_ptr->isLaunching = false;
    }

}

bool bub_move (bub_t * bub_t_ptr, int ** bubs_array, int *** bub_array_centers)
{

    bool debug = true ;

    /* target_pos x and y */

    double * target_pos_x = (double *) malloc (sizeof(double)) ;
    double * target_pos_y = (double *) malloc (sizeof(double)) ;

    *target_pos_x = bub_t_ptr->x + bub_t_ptr->step_x ;
    *target_pos_y = bub_t_ptr->y - bub_t_ptr->step_y ;

    if (debug) {
        printf("%f\n", *target_pos_x);
        printf("%f\n", *target_pos_y);
    }

    /* now we check the different scenarios : collision, hit top, hit borders, ...*/

    /* 1. COLLISION */
    if (bub_isColliding (bub_t_ptr, bubs_array, bub_array_centers, target_pos_x, target_pos_y)) {

        //printf ("paf\n") ;

        bub_t_ptr->isMoving = false ;

        return false ;
    }

    /* 2. hit TOP */
    if (*target_pos_y <= (double)BOARD_TOP) { // it went across board in Y
        *target_pos_y = (double)BOARD_TOP ;

        bub_t_ptr->isMoving = false ;

        return false ;
    }

    /* 3. REBOUND left */
    if (*target_pos_x <= (double)BOARD_LEFT) {

        /* recalculate tar_pos_x according to formula : x = x + 2d, with d = B_L -x */
        *target_pos_x = (double)(2 * BOARD_LEFT) - *target_pos_x ;

        /* change direction of motion */
        bub_t_ptr->step_x *= -1 ;
    }

    /* 4. REBOUND RIGHT*/
    else if (*target_pos_x >= (double)(BOARD_RIGHT - BUB_SIZE)) {

        /* recalculate tar_pos_x according to formula : x = x + B_S - 2d, with d = B_R + x + B_Size */
        *target_pos_x = *target_pos_x + (double)(2 * BOARD_RIGHT) + (double)(2 * BUB_SIZE)  ;

        /* change direction of motion */
        bub_t_ptr->step_x *= -1 ;
    }

    /* 5. NORMAL ROUTE*/
    else {

        bub_t_ptr->x = *target_pos_x ;
        bub_t_ptr->y = *target_pos_y ;

        bub_t_ptr->position.x = (int) bub_t_ptr->x ;
        bub_t_ptr->position.y = (int) bub_t_ptr->y ;
    }

    return true ;

}

bool bub_place (bub_t * bub_t_ptr, int ** bubs_array, int *** bub_array_centers) {

    bool debug = false ;

    if (debug)
        printf ("placeing bub !\n") ;

    /* reference for positioning is the /center/ of the bub
     * so we have to add BUB_SIZE/2 to each coordinate */
    float x_myBub = bub_t_ptr->x + BUB_SIZE / 2 ;
    float y_myBub = bub_t_ptr->y + BUB_SIZE / 2 ;

    if (debug)
        printf ("x_mybub = %f\n", x_myBub) ;
        printf ("y_mybub = %f\n", y_myBub) ;

    /* loop through empty spaces */
    short i, j, j_max ;

    for (i = 0 ; i < BUB_NY ; i += 1) {

        j_max = (i % 2 == 0) ? 8 : 7 ;

        for (j = 0 ; j < j_max ; j += 1) {

            /* if there is NO bub at this position */
            if (bubs_array[i][j] == 0) {

                float x_otherBub = bub_array_centers[i][j][0] ;
                float y_otherBub = bub_array_centers[i][j][1] ;

                float dist_between_centers = bub_getDistanceBetweenTwoBubs(x_myBub, y_myBub, x_otherBub, y_otherBub) ;

                 if (debug)
                    printf ("Ligne : %d, Colonne : %d, Dist = %f\n", i, j,dist_between_centers);

                if (dist_between_centers <= BUB_SIZE / 2.) {
                    bubs_array[i][j] = 1 ;

                    if (debug)
                        printf ("Bub placed at Line %d Col %d\n", i, j) ;

                    }
            }
        }
    }

    return true ;

    /* row_num depends on Y
     * for now we suppose it reaches the BOARD_TOP *//*
    int row_num = 0 ;

    *//* calculate the width taken by a bubble *//*
    int bub_width = (BOARD_RIGHT - BOARD_LEFT) / BUB_NX ;

    //printf ("bub_width = %d\n", bub_width) ;

    *//* number of bubs on this row *//*
    short num_bubs = (row_num % 2 == 0) ? 8 : 7 ;

    //printf ("num_bubs = %d\n", num_bubs) ;

    short j, place_pos = -1 ;

    *//* the reference x is the middle of the bub *//*
    double x = bub_t_ptr-> x + BUB_SIZE / 2 ;

    //printf ("x = %f\n", x) ;

    for (j = 0; j < num_bubs ; j += 1) {

        short left = BOARD_LEFT + bub_width*(j) ;
        short right = BOARD_LEFT + bub_width*(j+1) ;

        *//* we check if our moving bubble just stopped between left and right x *//*
        if ((x >= left) && (x < right)) {

            place_pos = j ;
            break ;
        }
    }

    //printf("placing bub in %d\n", place_pos) ;

    *//* let's update bubs_array *//*
    bubs_array[row_num][place_pos] = 1 ;*/
}

bool bub_isColliding (bub_t * bub_t_ptr, int ** bubs_array, int *** bub_array_centers, double *target_pos_x, double *target_pos_y) {

    bool debug = false ;

    double x_myBub = *target_pos_x + (double)(BUB_SIZE /2) ;
    double y_myBub = *target_pos_y + (double)(BUB_SIZE /2) ;

    /* loop through non-moving bubs */
    short i, j, j_max ;

    for (i = 0 ; i < BUB_NY ; i += 1) {

        j_max = (i % 2 == 0) ? 8 : 7 ;

        for (j = 0 ; j < j_max ; j += 1) {

            /* if there is a bub at this position */
            if (bubs_array[i][j] > 0) {

                double x_otherBub = bub_array_centers[i][j][0] ;
                double y_otherBub = bub_array_centers[i][j][1] ;

                /* see if there is a collision */
                double dist_between_centers = bub_getDistanceBetweenTwoBubs(x_myBub, y_myBub, x_otherBub, y_otherBub) ;

                double collison_dist = BUB_SIZE * 0.87 ;

                if (dist_between_centers < collison_dist) {

                    if(debug) {
                        printf("-----------------\n");
                        printf("COLLISION avec ligne = %d, colonne = %d\n", i, j);
                        printf("-----------------\n");
                    }

                    return true;
                }
            }
        }
    }

    return false ;

}

double bub_getDistanceBetweenTwoBubs (double bub1_x, double bub1_y, double bub2_x, double bub2_y) {

    double dx = bub1_x - bub2_x ;
    double dy = bub1_y - bub2_y ;

    return sqrt (pow (dx, 2) + pow (dy, 2)) ;
}