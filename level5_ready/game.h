#ifndef S2_PROJ_GAME_H
#define S2_PROJ_GAME_H

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "data.h"


int game_init (game_t * game_t_ptr, sys_t * sys_t_ptr) ;

int game_newGame (game_t * game_t_ptr, bub_t * bub_t_ptr) ;

int game_loadSprites (game_t * game_t_ptr, sys_t * sys_t_ptr) ;

int game_resetBubsArray (game_t * game_t_ptr) ;

int game_setBubsArrayCenters (game_t * game_t_ptr) ;

int game_resetBubConnectedComponent (game_t * game_t_ptr) ;

int game_resetConnexity (game_t * game_t_ptr) ;

int game_cleanBoard (game_t * game_t_ptr, SDL_Rect * bubJustPlace_rect) ;

int game_checkConnexity (game_t * game_t_ptr, SDL_Rect * bubJustPlace_rect, bool colorConnexity) ;

int game_spotCheckConnexity (game_t * game_t_ptr, bub_t * bub_t_neighbour_ptr, bub_t * bub_t_neighbour_ptr_2, SDL_Rect * bubCoord_rect, bool colorConnexity, short colorForConnexity) ;

int game_addBubConnected (game_t * game_t_ptr, SDL_Rect * bubJustPlaced_rect) ;

bub_t * game_getBubAt (game_t * game_t_ptr, bub_t * bub_t_neighbour_ptr, SDL_Rect * rect_ptr) ;

bool game_checkVictory (game_t * game_t_ptr) ;

#endif //S2_PROJ_GAME_H
