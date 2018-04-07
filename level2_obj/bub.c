#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "bub.h"



void bub_init (bub_t * bub_t_ptr) {

    //printf ("bub_t initialised") ;

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




            /* then launching the bub */
            bub_t_ptr->isMoving = true;
        }

        /* in all cases, launching is finished */
        bub_t_ptr->isLaunching = false;
    }

}

void bub_move (bub_t * bub_t_ptr)
{

    /* target_pos x and y
     * */
    double target_pos_x = bub_t_ptr->x + bub_t_ptr->step_x ;
    double target_pos_y = bub_t_ptr->y - bub_t_ptr->step_y ;
    //printf ("%f\n", target_pos_y) ;


    /* check if target_pos is within window
     * first with X coordinates
     * second with Y coord
     *
     * if bub within window : bub keeps moving -> coord are updated
     * if not : bub stop
     * */
    if ((target_pos_x >= BOARD_LEFT) && (target_pos_x <= BOARD_RIGHT - BUB_SIZE)) {
        // if for Y
        if (target_pos_y <= BOARD_TOP) { // it went across board in Y
            target_pos_y = BOARD_TOP ;
            bub_t_ptr->isMoving = false ;
        }

        // bub still in window

        bub_t_ptr->x = target_pos_x ;
        bub_t_ptr->y = target_pos_y ;

        bub_t_ptr->position.x = (int) bub_t_ptr->x ;
        bub_t_ptr->position.y = (int) bub_t_ptr->y ;

    }
    else {
        // bubble not in window in X
        bub_t_ptr->isMoving = false ;
    }


}