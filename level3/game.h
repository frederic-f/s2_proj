#ifndef S2_PROJ_GAME_H
#define S2_PROJ_GAME_H


#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "bub.h"


void HandleEvent(SDL_Event event, int * quit, int * currOrientation, bub_t * bub_t_ptr) ;



#endif //S2_PROJ_GAME_H
