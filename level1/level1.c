#include <SDL.h>

/* Size of the window */
#define SCREEN_WIDTH  720
#define SCREEN_HEIGHT 540
/* Size of the frame picture */
#define FRAME_WIDTH   720
#define FRAME_HEIGHT  540

/* In the sprite, we have 8 images of a 32x32 picture,
 * 2 images for each direction. */
/* Size of one image: */
#define SPRITE_SIZE     32

/* Order of the different directions in the picture: */
#define DIR_UP          0
#define DIR_RIGHT       1
#define DIR_DOWN        2
#define DIR_LEFT        3

/* Number of pixels for one step of the sprite */
#define SPRITE_STEP     5

/* Handle events coming from the user:
        - quit the game?
        - use of arrows to move the sprite
   In the second case, we modify the current direction and the
   position of the sprite in the window.
   We also change the animation bit used for creating the "walk" effect.
   */
void HandleEvent(SDL_Event event,
        int *quit, int *currDirection, int *animFlip, SDL_Rect *position)
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
                    *currDirection = DIR_LEFT;
                    *animFlip = 1 - *animFlip;
                    position->x -= SPRITE_STEP;
                    break;
                case SDLK_RIGHT:
                    *currDirection = DIR_RIGHT;
                    *animFlip = 1 - *animFlip;
                    position->x += SPRITE_STEP;
                    break;
                case SDLK_UP:
                    *currDirection = DIR_UP;
                    *animFlip = 1 - *animFlip;
                    position->y -= SPRITE_STEP;
                    break;
                case SDLK_DOWN:
                    *currDirection = DIR_DOWN;
                    *animFlip = 1 - *animFlip;
                    position->y += SPRITE_STEP;
                    break;
                default:
                    break;
            }
            break;
    }
}

int main(int argc, char* argv[])
{
    SDL_Surface *screen, *temp, *sprite, *frame;
    int colorkey;

    /* Information about the current situation of the sprite: */
    int currentDirection = DIR_RIGHT;
    /* Walking in a direction is animated with 2 images; we use a boolean
     * that we flip after each movement to show them in turn and create
     * the illusion */
    int animationFlip = 0;

    /* Rectangle to store the position of the sprite in the window.
     * Only the x and y coordinates are used. */
    SDL_Rect spritePosition;


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


    /* load arrow */
    temp = SDL_LoadBMP("frame_launcher.bmp");
    launcher = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    /* set sprite position in the bottoom of the window */
    spritePosition.x = (SCREEN_WIDTH - SPRITE_SIZE) / 2;
    spritePosition.y = (SCREEN_HEIGHT - SPRITE_SIZE) / 2;
    
    int gameover = 0;

    /* main loop: check events and re-draw the window until the end */
    while (!gameover)
    {
        SDL_Event event;

        /* look for an event; possibly update the position and the shape
         * of the sprite. */
        if (SDL_PollEvent(&event)) {
            HandleEvent(event, &gameover, &currentDirection,
                    &animationFlip, &spritePosition);
        }

        /* collide with edges of screen 
        if (spritePosition.x <= 0)
            spritePosition.x = 0;
        if (spritePosition.x >= SCREEN_WIDTH - SPRITE_SIZE) 
            spritePosition.x = SCREEN_WIDTH - SPRITE_SIZE;

        if (spritePosition.y <= 0)
            spritePosition.y = 0;
        if (spritePosition.y >= SCREEN_HEIGHT - SPRITE_SIZE) 
	spritePosition.y = SCREEN_HEIGHT - SPRITE_SIZE;*/

	
	// draw the frame
	SDL_Rect position;
	position.x = 0 ;
	position.y = 0 ;
	SDL_BlitSurface(frame, NULL, screen, &position) ;






        /* update the screen */
        SDL_UpdateRect(screen, 0, 0, 0, 0);
    }

    /* clean up */

    SDL_FreeSurface(frame);
    SDL_Quit();

    return 0;
}
