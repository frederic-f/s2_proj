//
// Created by ramses on 11/04/18.
//

#ifndef S2_PROJ_CONSTANTS_H
#define S2_PROJ_CONSTANTS_H

#define SCREEN_WIDTH        720
#define SCREEN_HEIGHT       540

#define LAUNCHER_WIDTH      187
#define LAUNCHER_HEIGHT     157
#define LAUNCHER_CENTER     94

#define BOARD_LEFT          200
#define BOARD_RIGHT         520
#define BOARD_TOP           31

/* In the sprite, we have 45 images of a 187x157 picture */
#define LAUNCHER_DIV        48 // PI / LAUNCHER_DIV is the angular distance between each of the 45 launcher orientations
#define PI 3.14159265359

#define VELOCITY            1  // bubble velocity
#define BUB_SIZE            40 // horizontal and vertical size
#define BUB_START_Y         457

#define BUB_NX              8       // max number of bubs in hrztl direction
#define BUB_NY              11      // vrtcl

#define NUM_COLOR	        8


#endif //S2_PROJ_CONSTANTS_H
