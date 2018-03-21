#include <SDL.h>

/* Size of the window */
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
/* Size of the sprite; this is a square */
#define SPRITE_SIZE   32
/* Size of the grass texture picture */
#define GRASS_SIZE    32

int main(int argc, char* argv[])
{
    SDL_Surface *screen, *temp, *sprite, *grass;
    SDL_Rect spritePosition;
    int colorkey, gameover;

    /* initialize video system */
    SDL_Init(SDL_INIT_VIDEO);

    /* set the title bar */
    SDL_WM_SetCaption("SDL Move", "SDL Move");

    /* create window */
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

    /* load sprite */
    temp   = SDL_LoadBMP("sprite.bmp");
    sprite = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    /* setup sprite colorkey and turn on RLE */
    colorkey = SDL_MapRGB(screen->format, 255, 0, 255);
    SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

    /* load grass */
    temp  = SDL_LoadBMP("grass.bmp");
    grass = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    /* set sprite position */
    spritePosition.x = 0;
    spritePosition.y = 0;

    gameover = 0;

    /* main loop: check events and re-draw the window until the end */
    while (!gameover)
    {
        SDL_Event event;
        Uint8 *keystate;

        /* look for an event */
        if (SDL_PollEvent(&event)) {
            /* an event was found */
            switch (event.type) {
                /* close button clicked */
                case SDL_QUIT:
                    gameover = 1;
                    break;

                /* handle the keyboard */
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                        case SDLK_q:
                            gameover = 1;
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }

        /* handle sprite movement */
        keystate = SDL_GetKeyState(NULL);

        if (keystate[SDLK_LEFT] ) {
            spritePosition.x -= 2;
        }
        if (keystate[SDLK_RIGHT] ) {
            spritePosition.x += 2;
        }
        if (keystate[SDLK_UP] ) {
            spritePosition.y -= 2;
        }
        if (keystate[SDLK_DOWN] ) {
            spritePosition.y += 2;
        }

        /* collide with edges of screen */
        if (spritePosition.x < 0) {
            spritePosition.x = 0;
        } else if (spritePosition.x > SCREEN_WIDTH - SPRITE_SIZE) {
            spritePosition.x = SCREEN_WIDTH - SPRITE_SIZE;
        }

        if (spritePosition.y < 0) {
            spritePosition.y = 0;
        } else if (spritePosition.y > SCREEN_HEIGHT - SPRITE_SIZE) {
            spritePosition.y = SCREEN_HEIGHT - SPRITE_SIZE;
        }

        /* map the grass texture on the whole background */
        for (int x = 0; x < SCREEN_WIDTH/GRASS_SIZE; x++) {
            for (int y = 0; y < SCREEN_HEIGHT/GRASS_SIZE; y++) {
                SDL_Rect position;
                position.x = x * GRASS_SIZE;
                position.y = y * GRASS_SIZE;
                SDL_BlitSurface(grass, NULL, screen, &position);
            }
        }

        /* draw the sprite */
        SDL_BlitSurface(sprite, NULL, screen, &spritePosition);

        /* update the screen */
        SDL_UpdateRect(screen,0,0,0,0);
    }

    /* clean up */
    SDL_FreeSurface(sprite);
    SDL_FreeSurface(grass);
    SDL_Quit();

    return 0;
}
