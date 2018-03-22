#include <SDL.h>
#include <math.h>
#include "SDL_TTF.h"

/* Size of the window */
#define SCREEN_WIDTH      720
#define SCREEN_HEIGHT     540

#define BOARD_LEFT        200
#define BOARD_RIGHT       520
#define BOARD_TOP         31

/* In the sprite, we have 45 images of a 187x157 picture */

/* Size of one image: */
#define LAUNCHER_WIDTH    187
#define LAUNCHER_HEIGHT   157
#define LAUNCHER_CENTER   94


#define PI 3.14159265359


#define BUB_SIZE          40 // horizontal and vertical size
#define BUB_START_Y       457
#define VELOCITY          2  // bubble velocity

/* Handle events coming from the user:
        - quit the game?
        - use of arrows to move the sprite
   In the second case, we modify the current direction and the
   position of the sprite in the window.
   We also change the animation bit used for creating the "walk" effect.
   */
void HandleEvent(SDL_Event event,
        int *quit, int *currOrientation, SDL_Rect * bubPosition)
{
    switch (event.type) {
        /* close button clicked */
        case SDL_QUIT:
            *quit = 1;
            break;

        /* handle the keyboard */
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                case SDLK_q:
                    *quit = 1;
                    break;
                case SDLK_LEFT:
                    // launcher rotates to the left, unless already at extreme left
                    if (*currOrientation > 0) {
                        *currOrientation -= 1 ;
                    }
                    break;
                case SDLK_RIGHT:
                    // launcher rotates to the right, unless already at far right
                    if (*currOrientation < 44) {
                        *currOrientation += 1 ;
                    }
                    break;

                case SDLK_SPACE:
                    // lauches the buble
                    // movement either to the right or left according to launcher direction
                    // stop when touch either left (resp) right touches left (resp) righ border

                    /* if bubble not on laucher : get it back on launcher
                     * otherwise, launch the bubble */

                    /* to check if bubble is on launcher, we use Y coordinates (since Y WILL be different if bubble moved */
                    if (bubPosition -> y != BUB_START_Y) {
                        /* bubble return to launcher */
                        bubPosition -> x = SCREEN_WIDTH / 2 - BUB_SIZE / 2 - 1;
                        bubPosition -> y = BUB_START_Y ;
                    } else {
                        /* bubble is launched */
                        if (*currOrientation < 22) {
                            /* stops when touches left border */
                            bubPosition -> y -= VELOCITY ;
                            while (bubPosition -> x > BOARD_LEFT) {
                                bubPosition -> x -= VELOCITY ;
                            }
                        } else if (*currOrientation > 22) {
                            /* stops when touches RIGHT border */

                            bubPosition -> y -= VELOCITY ;
                            while (bubPosition -> x < BOARD_RIGHT - BUB_SIZE) {
                                bubPosition -> x += VELOCITY ;
                            }
                        } else {
                            /* bubble should go straight up */
                            while (bubPosition -> y > BOARD_TOP) {
                                bubPosition -> y -= VELOCITY ;
                            }
                        }
                    }

                    break;
    
                default:
                    break;
            }
            break;
    }
}




int main(int argc, char* argv[])
{
    double sinx = sin (PI) ;

    printf("sin: %f", sinx) ;



    SDL_Surface *screen, *temp, *sprite, *frame, *bubSprite;

    int colorkey;

    /* Information about the current situation of the launcher sprite */
    int currentOrientation = 22 ; // 0 = pointing left, 22 = upwards, 45 = right

    /* initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);

    /* set the title bar */
    SDL_WM_SetCaption("Zee Animation", "Zee Animation");

    /* create window */
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

    /* set keyboard repeat */
    SDL_EnableKeyRepeat(100, 30);

    /* load frame */
    temp  = SDL_LoadBMP("frame_1p.bmp");
    frame = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    /* load sprite (launcher) */
    temp = SDL_LoadBMP("frame_launcher.bmp");
    sprite = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    /* BUBBLE SPRITE */
    temp = SDL_LoadBMP("bub_blue.bmp");
    bubSprite = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    /* creation of a cache to hide the launcher sprite */
    SDL_Rect cache ;
    cache.x = 0 ;
    cache.y = 0 ;
    cache.w = SCREEN_WIDTH ;
    cache.h = SCREEN_HEIGHT ;

    /* cache will be black */
    int color = SDL_MapRGB(screen->format, 0, 0, 0) ;

    /* TRANSPARENCY */

    /* set color that is to be transparent */
    colorkey = SDL_MapRGB(screen->format, 255, 0, 255);

    /* setup LAUNCHER SPRITE colorkey and turn on RLE */
    SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

    /* setup FRAME colorkey and turn on RLE */
    SDL_SetColorKey(frame, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

    /* setup BUBSPRITE colorkey and turn on RLE */
    SDL_SetColorKey(bubSprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


    /* Rectangle to store the position of the sprite in the window. */
    SDL_Rect spritePosition;

    /* set sprite LAUNCHER position in the bottoom of the window */
    spritePosition.x = (SCREEN_WIDTH - LAUNCHER_WIDTH) / 2; // in the middle of the window in WIDTH
    spritePosition.y = SCREEN_HEIGHT - LAUNCHER_HEIGHT ; // at the bottom in HEIGHT
    

    /* FF Rectangle to store the position of the bubble in the window
       Only the x and y coordinatse are used */
    SDL_Rect bubPosition ;

    /* set sprite BUBBLE position initialy on top of launcher */
    bubPosition.x = SCREEN_WIDTH / 2 - BUB_SIZE / 2 - 1;
    bubPosition.y = BUB_START_Y ;

    int gameover = 0;

    /* main loop: check events and re-draw the window until the end */
    while (!gameover)
    {
        SDL_Event event;

        /* look for an event; possibly update the position and the shape
         * of the sprite. */
        if (SDL_PollEvent(&event)) {
            HandleEvent(event, &gameover, &currentOrientation, &bubPosition);
        }

       // draw the cache
        SDL_FillRect(screen, &cache, color) ;


	    // draw the frame
        // first a Rect for the position
	    SDL_Rect framePosition;
        // then set the position
        framePosition.x = 0 ;
        framePosition.y = 0 ;
        // then draw on screen
	    SDL_BlitSurface(frame, NULL, screen, &framePosition) ;


        // draw the sprit LAUNCHER
        SDL_Rect spriteImage ;
        spriteImage.w = LAUNCHER_WIDTH ;
        spriteImage.h = LAUNCHER_HEIGHT ;
        spriteImage.x = 0 ; // the image is moved in height, not in width
        spriteImage.y = LAUNCHER_HEIGHT * currentOrientation ;

        SDL_BlitSurface(sprite, &spriteImage, screen, &spritePosition) ;


	    // draw the bubble
	    SDL_Rect bubImage ;
	    bubImage.w = BUB_SIZE ;
	    bubImage.h = BUB_SIZE ;
	    bubImage.x = 0 ;
	    bubImage.y = 0 ;
	
	    SDL_BlitSurface(bubSprite, &bubImage, screen, &bubPosition) ;


        /* update the screen */
        SDL_UpdateRect(screen, 0, 0, 0, 0);

        //printf ("%d\n", currentOrientation) ;
    }

    /* clean up */
    SDL_FreeSurface(sprite) ;
    SDL_FreeSurface(frame);
    SDL_Quit();

    return 0;
}
