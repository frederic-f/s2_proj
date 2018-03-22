#include <SDL.h>

/* Size of the window */
#define SCREEN_WIDTH  720
#define SCREEN_HEIGHT 540


/* In the sprite, we have 45 images of a 187x157 picture */

/* Size of one image: */
#define SPRITE_WIDTH     187
#define SPRITE_HEIGHT    157



/* Handle events coming from the user:
        - quit the game?
        - use of arrows to move the sprite
   In the second case, we modify the current direction and the
   position of the sprite in the window.
   We also change the animation bit used for creating the "walk" effect.
   */
void HandleEvent(SDL_Event event,
        int *quit, int *currOrientation)
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

                default:
                    break;
            }
            break;
    }
}

int main(int argc, char* argv[])
{
    SDL_Surface *screen, *temp, *sprite, *frame ;

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
    SDL_EnableKeyRepeat(10, 10);

    /* load frame */
    temp  = SDL_LoadBMP("frame_1p.bmp");
    frame = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);


    /* load sprite (launcher) */
    temp = SDL_LoadBMP("frame_launcher.bmp");
    sprite = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    /* creation of a cache to hide the launcher sprite */
    SDL_Rect cache ;
    cache.x = 0 ;
    cache.y = 0 ;
    cache.w = SCREEN_WIDTH ;
    cache.h = SCREEN_HEIGHT ;

    /* cache will be black */
    int color = SDL_MapRGB(screen->format, 0, 0, 0) ;

    /* setup SPRITE colorkey and turn on RLE */
    colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
   
    SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

    /* setup FRAME colorkey and turn on RLE */
    SDL_SetColorKey(frame, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

    /* Rectangle to store the position of the sprite in the window. */
    SDL_Rect spritePosition;

    /* set sprite position in the bottoom of the window */
    spritePosition.x = (SCREEN_WIDTH - SPRITE_WIDTH) / 2; // in the middle of the window in WIDTH
    spritePosition.y = SCREEN_HEIGHT - SPRITE_HEIGHT ; // at the bottom in HEIGHT
    
    int gameover = 0;

    /* main loop: check events and re-draw the window until the end */
    while (!gameover)
    {
        SDL_Event event;

        /* look for an event; possibly update the position and the shape
         * of the sprite. */
        if (SDL_PollEvent(&event)) {
            HandleEvent(event, &gameover, &currentOrientation);
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


        // draw the sprit
        SDL_Rect spriteImage ;
        spriteImage.w = SPRITE_WIDTH ;
        spriteImage.h = SPRITE_HEIGHT ;
        spriteImage.x = 0 ; // the image is moved in height, not in width
        spriteImage.y = SPRITE_HEIGHT * currentOrientation ;

        SDL_BlitSurface(sprite, &spriteImage, screen, &spritePosition) ;


 

        /* update the screen */
        SDL_UpdateRect(screen, 0, 0, 0, 0);
    }

    /* clean up */
    SDL_FreeSurface(sprite) ;
    SDL_FreeSurface(frame);
    SDL_Quit();

    return 0;
}
