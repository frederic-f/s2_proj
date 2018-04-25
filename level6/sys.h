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

int sys_cleanUp (sys_t * sys_t_ptr) ;

#endif //S2_PROJ_SYS_H
