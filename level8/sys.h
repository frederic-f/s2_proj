#ifndef S2_PROJ_SYS_H
#define S2_PROJ_SYS_H

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "data.h"


int sys_init (sys_t * sys_t_ptr) ;

int sys_loadSprites (sys_t * sys_t_ptr) ;

int sys_makeTransparent (sys_t * sys_t_ptr, SDL_Surface * surf_ptr) ;

int sys_draw (sys_t * sys_t_ptr, game_t * game_t_ptr, bub_t * bub_t_ptr) ;

int sys_cleanUp (sys_t * sys_t_ptr, game_t * game_t_ptr, bub_t * bub_t_ptr) ;


void sys_handleEvent (SDL_Event event, sys_t * sys_t_ptr, game_t * game_t_ptr, bub_t * bub_t_ptr) ;

SDL_Rect * sys_getBubPositionRect (game_t * game_t_ptr, int i, int j, SDL_Rect * dumRect_ptr) ;

int sys_changeState (sys_t * sys_t_ptr, int newState) ;

int sys_loadSounds (sys_t * sys_t_ptr) ;

int sys_playSound (sys_t * sys_t_ptr, int sound) ;

short getRandomNumber (int max) ;


#endif //S2_PROJ_SYS_H
