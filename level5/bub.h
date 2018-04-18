#ifndef S2_PROJ_BUB_H
#define S2_PROJ_BUB_H

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "data.h"


int bub_init (bub_t * bub_t_ptr, game_t * game_t_ptr) ;

void bub_launch (bub_t * bub_t_ptr, game_t * game_t_ptr, int * currOrientation) ;

int bub_getOnLauncher (bub_t * bub_t_ptr, game_t * game_t_ptr) ;

bool bub_move (bub_t * bub_t_ptr, game_t * game_t_ptr) ;

SDL_Rect * bub_place (bub_t * bub_t_ptr, game_t * game_t_ptr) ;

bool bub_isColliding (bub_t * bub_t_ptr, game_t * game_t_ptr, double *target_pos_x, double *target_pos_y) ;

bool bub_isBelowLimit (bub_t * bub_t_ptr) ;

double bub_getDistanceBetweenTwoBubs (double bub1_x, double bub1_y, double bub2_x, double bub2_y) ;

#endif
