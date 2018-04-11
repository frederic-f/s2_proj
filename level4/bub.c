#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "constants.h"
#include "bub.h"



void bub_init (bub_t * bub_t_ptr) {

    SDL_Surface *temp ;
 
    /* SDL_Surface array for color random pickup */
    //SDL_Surface * bmps[]
    
    //printf ("bub_t initialised") ;
    temp = SDL_LoadBMP("bub_blue.bmp");
    //bmps[0] = SDL_DisplayFormat(temp) ;
    
    
    //bub_t_ptr->sprite_ptr = bmps[0] ;
    bub_t_ptr->sprite_ptr = SDL_DisplayFormat(temp) ;
    
    SDL_FreeSurface(temp) ;
    
    bub_t_ptr->isLaunching = false ;

    bub_t_ptr->isMoving = false ;

    bub_t_ptr->start_x = 720 / 2 - BUB_SIZE / 2 - 1;
    bub_t_ptr->start_y = BUB_START_Y;

    bub_getOnLauncher(bub_t_ptr) ;

}


void bub_getOnLauncher(bub_t * bub_t_ptr) {

    //DBG printf ("back on launcher\n") ;

    /* return bubble to launcher */
    bub_t_ptr->position.x = bub_t_ptr->start_x;
    bub_t_ptr->position.y = bub_t_ptr->start_y;

    bub_t_ptr->x = bub_t_ptr->start_x;
    bub_t_ptr->y = bub_t_ptr->start_y;
}


void bub_launch (bub_t * bub_t_ptr, int *currOrientation) {

    /* launching only if bubble not moving*/
    if (!bub_t_ptr->isMoving) { // it is not moving

        /* if bub NOT on launcher (and NOT MOVING)
        * then return it to launcher */
        if (bub_t_ptr->position.y != BUB_START_Y) {

            bub_getOnLauncher(bub_t_ptr) ;

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

void bub_move (bub_t * bub_t_ptr, int ** bubs_array, int *** bub_array_centers)
{

    /* target_pos x and y */
    double target_pos_x = bub_t_ptr->x + bub_t_ptr->step_x ;
    double target_pos_y = bub_t_ptr->y - bub_t_ptr->step_y ;
    //printf ("%f\n", target_pos_y) ;

    /* now we check the different scenarios : collision, hit top, hit borders, ...*/

    /* 1. COLLISION */
    if (bub_isColliding (bub_t_ptr, bubs_array, bub_array_centers)) {

        printf ("paf\n") ;

        bub_t_ptr->isMoving = false ;
    }

    /* 2. hit TOP */
    else if (target_pos_y <= BOARD_TOP) { // it went across board in Y
        target_pos_y = BOARD_TOP ;

        bub_t_ptr->isMoving = false ;
    }

    /* 3. REBOUND left */
    else if (target_pos_x <= BOARD_LEFT) {

        /* recalculate tar_pos_x according to formula : x = x + 2d, with d = B_L -x */
        target_pos_x = 2 * BOARD_LEFT - target_pos_x ;

        /* change direction of motion */
        bub_t_ptr->step_x *= -1 ;
    }

    /* 4. REBOUND RIGHT*/
    else if (target_pos_x >= BOARD_RIGHT - BUB_SIZE) {

        /* recalculate tar_pos_x according to formula : x = x + B_S - 2d, with d = B_R + x + B_Size */
        target_pos_x = target_pos_x + (2 * BOARD_RIGHT) + (2 * BUB_SIZE)  ;

        /* change direction of motion */
        bub_t_ptr->step_x *= -1 ;
    }

    /* 5. NORMAL ROUTE*/
    else {

        bub_t_ptr->x = target_pos_x ;
        bub_t_ptr->y = target_pos_y ;

        bub_t_ptr->position.x = (int) bub_t_ptr->x ;
        bub_t_ptr->position.y = (int) bub_t_ptr->y ;
    }


}

void bub_place(bub_t * bub_t_ptr, int ** bubs_array) {


    //printf ("placeing bub !\n") ;

    /* row_num depends on Y
     * for now we suppose it reaches the BOARD_TOP */
    int row_num = 0 ;

    /* calculate the width taken by a bubble */
    int bub_width = (BOARD_RIGHT - BOARD_LEFT) / BUB_NX ;

    //printf ("bub_width = %d\n", bub_width) ;

    /* number of bubs on this row */
    short num_bubs = (row_num % 2 == 0) ? 8 : 7 ;

    //printf ("num_bubs = %d\n", num_bubs) ;

    short j, place_pos = -1 ;

    /* the reference x is the middle of the bub */
    double x = bub_t_ptr-> x + BUB_SIZE / 2 ;

    //printf ("x = %f\n", x) ;

    for (j = 0; j < num_bubs ; j += 1) {

        short left = BOARD_LEFT + bub_width*(j) ;
        short right = BOARD_LEFT + bub_width*(j+1) ;

        /* we check if our moving bubble just stopped between left and right x */
        if ((x >= left) && (x < right)) {

            place_pos = j ;
            break ;
        }
    }

    //printf("placing bub in %d\n", place_pos) ;

    /* let's update bubs_array */
    bubs_array[row_num][place_pos] = 1 ;
}

bool bub_isColliding (bub_t * bub_t_ptr, int ** bubs_array, int *** bub_array_centers) {

    /* loop through non-moving bubs */
    short i, j, j_max ;

    for (i = 0 ; i < BUB_NY ; i += 1) {

        j_max = (i % 2 == 0) ? 7 : 8 ;

        for (j = 0 ; j < j_max ; j += 1) {

            /* if there is a bub at this position */
            if (bubs_array[i][j]) {

                float x_myBub = bub_t_ptr->x ;
                float y_myBub = bub_t_ptr->y ;

                float x_otherBub = bub_array_centers[i][j][0] ;
                float y_otherBub = bub_array_centers[i][j][1] ;

                /* see if there is a collision */
                float dist_between_centers = bub_getDistanceBetweenTwoBubs(x_myBub, y_myBub, x_otherBub, y_otherBub) ;

                float collison_dist = BUB_SIZE / 2. ;

                if (dist_between_centers <= collison_dist) {
                    return true ;
                }
            }
        }
    }

    return false ;

}

float bub_getDistanceBetweenTwoBubs (float bub1_x, float bub1_y, float bub2_x, float bub2_y) {

    float dx = bub1_x - bub2_x ;
    float dy = bub1_y - bub2_y ;

    return sqrt (pow (dx, 2) + pow (dy, 2)) ;
}