#ifndef S2_PROJ_GAME_H
#define S2_PROJ_GAME_H

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "data.h"



int game_init (game_t * game_t_ptr) ;

int game_loadSprites (game_t * game_t_ptr) ;

int game_resetBubsArray (game_t * game_t_ptr) ;

int game_setBubsArrayCenters (game_t * game_t_ptr) ;


// TODO : put the following functions in SYST.h/.c module

void HandleEvent (SDL_Event event, int * quit, int * currOrientation, bub_t * bub_t_ptr) ;

SDL_Rect * getBubPositionRect (int i, int j, SDL_Rect * dumRect_ptr) ;

short giveRandomNumber () ;


#endif //S2_PROJ_GAME_H
