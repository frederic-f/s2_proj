#include <SDL.h>
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include "data.h"
#include "sys.h"
#include "game.h"
#include "bub.h"


/* ****************************************************************************************************************
*   Initialization
* ************************************************************************************************************** */
int sys_init (sys_t * sys_t_ptr) {

    /* initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);

    /* set the title bar */
    SDL_WM_SetCaption("Puzzle Bobble - FST Nancy", "Puzzle Bobble - FST Nancy");

    /* set keyboard repeat */
    SDL_EnableKeyRepeat(10, 30);

    sys_t_ptr->screen_srf_ptr = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);


    /* set font */
    //sys_t_ptr->police = TTF_OpenFont("/home/pv1/Documents/s2_proj/level8/ttf/arcadeclassic.ttf", 25);
    //if(!sys_t_ptr->police){printf("Unable to open font");exit(1);} //The program exits here


    /* set color that is to be transparent */
    sys_t_ptr->colorkey = SDL_MapRGB(sys_t_ptr->screen_srf_ptr->format, 255, 0, 255);

    /* load all sprites but bubbles*/
    sys_loadSprites (sys_t_ptr) ;


    /* launcher items */

    /* arrow */
    sys_t_ptr->launcher_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    /* set sprite LAUNCHER position in the bottoom of the window */
    sys_t_ptr->launcher_rect_ptr->x = (SCREEN_WIDTH - LAUNCHER_WIDTH) / 2; // in the middle of the window in WIDTH
    sys_t_ptr->launcher_rect_ptr->y = SCREEN_HEIGHT - LAUNCHER_HEIGHT - 19 ; // at the bottom in HEIGHT


    /* gears piece */
    sys_t_ptr->frameGears_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    /* set sprite LAUNCHER position in the bottoom of the window */
    sys_t_ptr->frameGears_rect_ptr->x = (SCREEN_WIDTH - GEARS_WIDTH) / 2 + 9; // in the middle of the window in WIDTH
	sys_t_ptr->frameGears_rect_ptr->y = SCREEN_HEIGHT - GEARS_HEIGHT - 19 ; // at the bottom in HEIGHT

	/* wheel piece */
    sys_t_ptr->frameWheel_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    /* set sprite LAUNCHER position in the bottoom of the window */
    sys_t_ptr->frameWheel_rect_ptr->x = (SCREEN_WIDTH - GEARS_WIDTH) / 2 + GEARS_WIDTH + 9; // in the middle of the window in WIDTH
    sys_t_ptr->frameWheel_rect_ptr->y = SCREEN_HEIGHT - WHEEL_HEIGHT - 19 ; // at the bottom in HEIGHT

    
    
    /* next bub */

    /* next bub place */
    sys_t_ptr->nextBub_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    /* */
    sys_t_ptr->nextBub_rect_ptr->x = (SCREEN_WIDTH - GEARS_WIDTH) / 2 - 30;
    sys_t_ptr->nextBub_rect_ptr->y = SCREEN_HEIGHT - 60 ;



    /* Tux */

    /* Tux place */
    sys_t_ptr->tux_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    /* */
    sys_t_ptr->tux_rect_ptr->x = (SCREEN_WIDTH - GEARS_WIDTH) / 2 - 30;
    sys_t_ptr->tux_rect_ptr->y = SCREEN_HEIGHT - 60 ;


	
    /* board frame*/
    sys_t_ptr->frame_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    sys_t_ptr->frame_rect_ptr->x = 0 ;
    sys_t_ptr->frame_rect_ptr->y = 0 ;


    /* roof - top frame*/
    sys_t_ptr->frameTop_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;
    sys_t_ptr->frameTop_rect_ptr->x = BOARD_LEFT ;

    /* chain */
    sys_t_ptr->frameChain1_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;
    sys_t_ptr->frameChain2_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    sys_t_ptr->frameChain1_rect_ptr->x = BOARD_LEFT + (BOARD_RIGHT - BOARD_LEFT) * 1 / 4 - (CHAIN_WIDTH / 2) ;
    sys_t_ptr->frameChain1_rect_ptr->y = BOARD_TOP ;
    sys_t_ptr->frameChain2_rect_ptr->x = BOARD_LEFT + (BOARD_RIGHT - BOARD_LEFT) * 3 / 4 - (CHAIN_WIDTH / 2) ;
    sys_t_ptr->frameChain2_rect_ptr->y = BOARD_TOP ;



    /* creation of a CACHE to hide the launcher sprite */
    sys_t_ptr->cache_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    sys_t_ptr->cache_rect_ptr->x = 0 ;
    sys_t_ptr->cache_rect_ptr->y = 0 ;
    sys_t_ptr->cache_rect_ptr->w = SCREEN_WIDTH ;
    sys_t_ptr->cache_rect_ptr->h = SCREEN_HEIGHT ;


    /* ******************************************************
    * Fonts and text
    * **************************************************** */

    /* font */
    SDL_Color fontColor = {255, 255, 255};
    sys_t_ptr->fontColor = fontColor ;

    sys_t_ptr->scoreFont = TTF_OpenFont("/home/pv1/Documents/s2_proj/level8/ttf/game_over.ttf", 65);

    sys_t_ptr->screenFont = TTF_OpenFont("/home/pv1/Documents/s2_proj/level8/ttf/game_over.ttf", 125);


    /* text score */
    sys_t_ptr->score = TTF_RenderText_Solid (sys_t_ptr->scoreFont, "000000", sys_t_ptr->fontColor);

    sys_t_ptr->scorePosition_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    sys_t_ptr->scorePosition_rect_ptr->x = 20 ;
    sys_t_ptr->scorePosition_rect_ptr->y = 20 ;


    /* text level */
    sys_t_ptr->text_level = TTF_RenderText_Solid (sys_t_ptr->scoreFont, "Niveau 1", sys_t_ptr->fontColor);

    sys_t_ptr->text_levelPosition_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    sys_t_ptr->text_levelPosition_rect_ptr->x = 570 ;
    sys_t_ptr->text_levelPosition_rect_ptr->y = 20 ;


    /* welcome text */
    sys_t_ptr->text_welcomeScreen_1 = TTF_RenderText_Solid (sys_t_ptr->screenFont, "Puzzle Bobble", sys_t_ptr->fontColor);
    sys_t_ptr->text_welcomeScreen_2 = TTF_RenderText_Solid (sys_t_ptr->screenFont, "(J)ouer or (Q)uitter", sys_t_ptr->fontColor);

    sys_t_ptr->text_welcomeScreen_1_position_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;
    sys_t_ptr->text_welcomeScreen_2_position_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    sys_t_ptr->text_welcomeScreen_1_position_rect_ptr->x = 100 ;
    sys_t_ptr->text_welcomeScreen_1_position_rect_ptr->y = 100 ;
    sys_t_ptr->text_welcomeScreen_2_position_rect_ptr->x = 100 ;
    sys_t_ptr->text_welcomeScreen_2_position_rect_ptr->y = 300 ;



    /* victory text */
    sys_t_ptr->text_victory_1 = TTF_RenderText_Solid (sys_t_ptr->screenFont, "Felicitations !", sys_t_ptr->fontColor);
    sys_t_ptr->text_victory_2 = TTF_RenderText_Solid (sys_t_ptr->screenFont, "(R)ejouer ?", sys_t_ptr->fontColor);

    sys_t_ptr->text_victory_1_position_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;
    sys_t_ptr->text_victory_2_position_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    sys_t_ptr->text_victory_1_position_rect_ptr->x = 100 ;
    sys_t_ptr->text_victory_1_position_rect_ptr->y = 100 ;
    sys_t_ptr->text_victory_2_position_rect_ptr->x = 100 ;
    sys_t_ptr->text_victory_2_position_rect_ptr->y = 300 ;


    /* gameover text */
    sys_t_ptr->text_gameover_1 = TTF_RenderText_Solid (sys_t_ptr->screenFont, "Perdu !!", sys_t_ptr->fontColor);
    sys_t_ptr->text_gameover_2 = TTF_RenderText_Solid (sys_t_ptr->screenFont, "(R)ejouer ?", sys_t_ptr->fontColor);

    sys_t_ptr->text_gameover_1_position_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;
    sys_t_ptr->text_gameover_2_position_rect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

    sys_t_ptr->text_gameover_1_position_rect_ptr->x = 100 ;
    sys_t_ptr->text_gameover_1_position_rect_ptr->y = 100 ;
    sys_t_ptr->text_gameover_2_position_rect_ptr->x = 100 ;
    sys_t_ptr->text_gameover_2_position_rect_ptr->y = 300 ;


    /* ******************************************************
    * Music and sounds
    * **************************************************** */
    sys_t_ptr->isPlayingMusic = false ;


    return (0) ;
}


/* ****************************************************************************************************************
*   Load Sprites controlled by sys
* ************************************************************************************************************** */
int sys_loadSprites (sys_t * sys_t_ptr) {

    SDL_Surface *temp ;


    /* Frame of board */
    temp  = SDL_LoadBMP("img/frame_1p.bmp");
    sys_t_ptr->frame_srf_ptr = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    sys_makeTransparent (sys_t_ptr, sys_t_ptr->frame_srf_ptr) ;


    /* Launcher */
    temp = SDL_LoadBMP("img/frame_launcher.bmp");
    sys_t_ptr->launcher_srf_ptr = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    sys_makeTransparent (sys_t_ptr, sys_t_ptr->launcher_srf_ptr) ;

    /* Gears */
    temp = SDL_LoadBMP("img/frame_gears.bmp");
    sys_t_ptr->frameGears_srf_ptr = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    sys_makeTransparent (sys_t_ptr, sys_t_ptr->frameGears_srf_ptr) ;

	/* wheel */
    temp = SDL_LoadBMP("img/frame_wheel.bmp");
    sys_t_ptr->frameWheel_srf_ptr = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    sys_makeTransparent (sys_t_ptr, sys_t_ptr->frameWheel_srf_ptr) ;
	
	/* Tux */
    temp = SDL_LoadBMP("img/frame_tux_wheel.bmp");
    sys_t_ptr->tux_srf_ptr = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    sys_makeTransparent (sys_t_ptr, sys_t_ptr->frameGears_srf_ptr) ;
	
    /* Roof */
    temp = SDL_LoadBMP("img/frame_top.bmp");
    sys_t_ptr->frameTop_srf_ptr = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    sys_makeTransparent (sys_t_ptr, sys_t_ptr->frameTop_srf_ptr) ;

    /* Chain  */
    temp = SDL_LoadBMP("img/frame_chain.bmp");
    sys_t_ptr->frameChain_srf_ptr = SDL_DisplayFormat(temp) ;
    SDL_FreeSurface(temp) ;

    sys_makeTransparent (sys_t_ptr, sys_t_ptr->frameChain_srf_ptr) ;


    return (0) ;
}


/* ****************************************************************************************************************
*   Makes sprite transparent
* ************************************************************************************************************** */
int sys_makeTransparent (sys_t * sys_t_ptr, SDL_Surface * surf_ptr) {

    return SDL_SetColorKey(surf_ptr, SDL_SRCCOLORKEY | SDL_RLEACCEL, sys_t_ptr->colorkey);

}


/* ****************************************************************************************************************
*   Draw all sprites on screen
* ************************************************************************************************************** */
int sys_draw (sys_t * sys_t_ptr, game_t * game_t_ptr, bub_t * bub_t_ptr) {


    bool debug = false ;


    /* cache */
    /* cache will be black */
    int blackColorForCache ;
    blackColorForCache = SDL_MapRGB(sys_t_ptr->screen_srf_ptr->format, 0, 0, 0) ;




    switch (sys_t_ptr->screen) {

        case SCREEN_WELCOME :

            /* back cacche */
            SDL_FillRect (sys_t_ptr->screen_srf_ptr, sys_t_ptr->cache_rect_ptr, blackColorForCache) ;

            /* welcome screen */
            SDL_BlitSurface(sys_t_ptr->text_welcomeScreen_1, NULL, sys_t_ptr->screen_srf_ptr, sys_t_ptr->text_welcomeScreen_1_position_rect_ptr); /* Blit du texte */
            SDL_BlitSurface(sys_t_ptr->text_welcomeScreen_2, NULL, sys_t_ptr->screen_srf_ptr, sys_t_ptr->text_welcomeScreen_2_position_rect_ptr); /* Blit du texte */



            break ;

        case SCREEN_VICTORY :

            /* back cacche */
            SDL_FillRect (sys_t_ptr->screen_srf_ptr, sys_t_ptr->cache_rect_ptr, blackColorForCache) ;

            /* welcome screen */
            SDL_BlitSurface(sys_t_ptr->text_victory_1, NULL, sys_t_ptr->screen_srf_ptr, sys_t_ptr->text_victory_1_position_rect_ptr); /* Blit du texte */
            SDL_BlitSurface(sys_t_ptr->text_victory_2, NULL, sys_t_ptr->screen_srf_ptr, sys_t_ptr->text_victory_2_position_rect_ptr); /* Blit du texte */



            break ;

        case SCREEN_GAMEOVER :

            /* back cacche */
            SDL_FillRect (sys_t_ptr->screen_srf_ptr, sys_t_ptr->cache_rect_ptr, blackColorForCache) ;

            /* welcome screen */
            SDL_BlitSurface(sys_t_ptr->text_gameover_1, NULL, sys_t_ptr->screen_srf_ptr, sys_t_ptr->text_gameover_1_position_rect_ptr); /* Blit du texte */
            SDL_BlitSurface(sys_t_ptr->text_gameover_2, NULL, sys_t_ptr->screen_srf_ptr, sys_t_ptr->text_gameover_2_position_rect_ptr); /* Blit du texte */



            break ;


        case SCREEN_PLAYING : /* playing */


            SDL_FillRect(sys_t_ptr->screen_srf_ptr, sys_t_ptr->cache_rect_ptr, blackColorForCache) ;


            /* frame */
            SDL_BlitSurface(sys_t_ptr->frame_srf_ptr, NULL, sys_t_ptr->screen_srf_ptr, sys_t_ptr->frame_rect_ptr) ;



            /* gears */
            SDL_Rect gearsImg_rect ;

            gearsImg_rect.w = GEARS_WIDTH ;
            gearsImg_rect.h = GEARS_HEIGHT ;
            gearsImg_rect.x = 0 ; // the image is moved in height, not in width
            gearsImg_rect.y = GEARS_HEIGHT * round(game_t_ptr->launcherOrientation / 45. * 20) ; /* there are only 40 positions for gears... */

            SDL_BlitSurface(sys_t_ptr->frameGears_srf_ptr, &gearsImg_rect, sys_t_ptr->screen_srf_ptr, sys_t_ptr->frameGears_rect_ptr) ;

            /* Wheel */
            SDL_Rect wheelImg_rect ;

            wheelImg_rect.w = WHEEL_WIDTH ;
            wheelImg_rect.h = WHEEL_HEIGHT ;
            wheelImg_rect.x = 0 ; // the image is moved in height, not in width
            wheelImg_rect.y = 0 ;

            SDL_BlitSurface(sys_t_ptr->frameWheel_srf_ptr, &wheelImg_rect, sys_t_ptr->screen_srf_ptr, sys_t_ptr->frameWheel_rect_ptr) ;


            /* launcher */
            SDL_Rect launcherImg_rect ;

            launcherImg_rect.w = LAUNCHER_WIDTH ;
            launcherImg_rect.h = LAUNCHER_HEIGHT ;
            launcherImg_rect.x = 0 ; // the image is moved in height, not in width
            launcherImg_rect.y = LAUNCHER_HEIGHT * game_t_ptr->launcherOrientation ;

            SDL_BlitSurface(sys_t_ptr->launcher_srf_ptr, &launcherImg_rect, sys_t_ptr->screen_srf_ptr, sys_t_ptr->launcher_rect_ptr) ;


            /* roof */

            sys_t_ptr->frameTop_rect_ptr->y = BOARD_TOP - ROOF_HEIGHT + 35 * game_t_ptr->roofShift ; /* 35 = 40 * sqrt(3) / 2 */

            /* show roof only when it has moved down, ie NOT in start position */
            bool showRoof = (sys_t_ptr->frameTop_rect_ptr->y != (BOARD_TOP - ROOF_HEIGHT)) ;

                if (showRoof) {

                SDL_BlitSurface(sys_t_ptr->frameTop_srf_ptr, NULL, sys_t_ptr->screen_srf_ptr, sys_t_ptr->frameTop_rect_ptr) ;
            }

            /* chains */
            SDL_Rect chainImg_rect ;

            chainImg_rect.w = CHAIN_WIDTH ;
            chainImg_rect.h = 35 * game_t_ptr->roofShift - ROOF_HEIGHT ;
            chainImg_rect.x = 0 ;
            chainImg_rect.y = 30 ;

            if (showRoof) {

                SDL_BlitSurface(sys_t_ptr->frameChain_srf_ptr, &chainImg_rect, sys_t_ptr->screen_srf_ptr,
                                sys_t_ptr->frameChain1_rect_ptr);
                SDL_BlitSurface(sys_t_ptr->frameChain_srf_ptr, &chainImg_rect, sys_t_ptr->screen_srf_ptr,
                                sys_t_ptr->frameChain2_rect_ptr);
            }
            /* BUBS*/

            SDL_Rect bub_rect ;
            bub_rect.w = BUB_SIZE ;
            bub_rect.h = BUB_SIZE ;
            bub_rect.x = 0 ;
            bub_rect.y = 0 ;

            /* moving bub */
            SDL_BlitSurface (bub_t_ptr->sprite_ptr, bub_t_ptr->spriteFrame, sys_t_ptr->screen_srf_ptr, bub_t_ptr->position) ;


            /* next bub */
            SDL_BlitSurface(game_t_ptr->bubs[game_t_ptr->nextBubColor - 1], &bub_rect, sys_t_ptr->screen_srf_ptr, sys_t_ptr->nextBub_rect_ptr);


            /* draw non-moving bubs */

            /* we use a pointer to place the non-moving bubs
             * the same pointer is used for all non-moving bubs
             * it is updated as we parse through the array of non-moving bubs
             * */
            SDL_Rect * dumRect_ptr = (SDL_Rect *) malloc (sizeof (SDL_Rect)) ;

            int i, j ;

            /* parsing the array of non-moving bubs : i=rows, j=cols */
            for (i = 0 ; i < BUB_NY ; i += 1) {

                /* number of bubs in a row depends on odd/even number of row */
                int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

                for (j = 0 ; j < j_max ; j +=1 ) {

                    /* process only the bubs set to 1 */
                    if (game_t_ptr->bubs_array[i][j] > 0) {

                        /* update the position of the bub to display */
                        dumRect_ptr = sys_getBubPositionRect(game_t_ptr, i, j, dumRect_ptr);

                        /* display */
                        SDL_BlitSurface(game_t_ptr->bubs[game_t_ptr->bubs_array[i][j] - 1], &bub_rect, sys_t_ptr->screen_srf_ptr, dumRect_ptr);
                    }
                }
            }

            /* draw falling bubs */
            if (game_t_ptr->bub_numFallingBubs > 0) {

                int i ;

                if (debug)
                    printf ("[sys_draw] Num of bubs falling %d\n", game_t_ptr -> bub_numFallingBubs) ;

                for (i = 0 ; i < (game_t_ptr -> bub_numFallingBubs) ; i += 1) {


                    struct Bub_t * bub_ptr = game_t_ptr->bub_fallingBubs[i] ;

                    if (debug)
                        printf ("[sys_draw] Bub #%d (color: %d) (Pos x:%d, y:%d) is falling\n", i, bub_ptr->color, bub_ptr->position->x, bub_ptr->position->y) ;

                    SDL_BlitSurface (bub_ptr->sprite_ptr, bub_ptr->spriteFrame, sys_t_ptr->screen_srf_ptr, bub_ptr->position) ;

                }
            }

            free (dumRect_ptr) ;


            /* text */

            /* score */
            SDL_BlitSurface(sys_t_ptr->score, NULL, sys_t_ptr->screen_srf_ptr, sys_t_ptr->scorePosition_rect_ptr); /* Blit du texte */

            /* level */
            //SDL_BlitSurface(sys_t_ptr->text_level, NULL, sys_t_ptr->screen_srf_ptr, sys_t_ptr->text_levelPosition_rect_ptr);

            break ;

    }



    /* update the screen */
    SDL_UpdateRect(sys_t_ptr->screen_srf_ptr, 0, 0, 0, 0);


    return (0) ;
}


/* ****************************************************************************************************************
*   Free memory and Exit SDL
* ************************************************************************************************************** */
int sys_cleanUp (sys_t * sys_t_ptr, game_t * game_t_ptr, bub_t * bub_t_ptr)  {


    bool debug = false ;


    if (debug)
        printf ("-> [sys_cleanUp]\n") ;

    TTF_CloseFont(sys_t_ptr->screenFont);
    TTF_CloseFont(sys_t_ptr->scoreFont);
    int i, j ;

    /* font */
    //TTF_CloseFont(sys_t_ptr->police);

    /* game_t_ptr pointers */

    /* bubs_array */
    for (i = 0 ; i < BUB_NY ; i += 1) {

        free (game_t_ptr->bubs_array[i]) ;
    }

    free (game_t_ptr->bubs_array) ;

    /* bub_array_centers */
    for (i = 0 ; i < BUB_NY ; i += 1) {

        int j_max = (i % 2 == 0) ? BUB_NX : BUB_NX - 1 ;

        for (j = 0 ; j < j_max ; j +=1 ) {

            free (game_t_ptr->bub_array_centers[i][j]) ;
        }

        free (game_t_ptr->bub_array_centers[i]) ;
    }

    free (game_t_ptr->bub_array_centers) ;

    /* bub_connected_component */
    for (i = 0 ; i < BUB_NY ; i += 1) {

        free (game_t_ptr->bub_connected_component[i]) ;
    }

    free (game_t_ptr->bub_connected_component) ;

    /* bub_fifo */
    for (i = 0 ; i < (BUB_NY * BUB_NX) ; i += 1) {

        free (game_t_ptr->bub_fifo[i]) ;
    }


    if (debug)
        printf ("Pos 2\n") ;

    free (game_t_ptr->bub_fifo) ;
    free (game_t_ptr->colorsOnBoard) ;


    free (game_t_ptr) ;

    if (debug)
        printf ("Pos 2.5\n") ;

    /* bub_t pointers */

    //free (bub_t_ptr->position) ;
    //free (bub_t_ptr->sprite_ptr) ;
    //free (bub_t_ptr) ;


    if (debug)
        printf ("Pos 2.6\n") ;


    /* SDL pointers */

    SDL_FreeSurface (sys_t_ptr->launcher_srf_ptr) ;

    if (debug)
        printf ("Pos 2.7\n") ;


    SDL_FreeSurface (sys_t_ptr->frame_srf_ptr) ;
    SDL_FreeSurface (sys_t_ptr->frameTop_srf_ptr) ;
    SDL_FreeSurface (sys_t_ptr->frameChain_srf_ptr) ;

    SDL_FreeSurface (sys_t_ptr->screen_srf_ptr) ;


    if (debug)
        printf ("Pos 3\n") ;

    /* sys_t_ptr pointers */

    free (sys_t_ptr->cache_rect_ptr) ;
    free (sys_t_ptr->frame_rect_ptr) ;
    free (sys_t_ptr->frameTop_rect_ptr) ;
    free (sys_t_ptr->launcher_rect_ptr) ;
    free (sys_t_ptr->frameChain1_rect_ptr) ;
    free (sys_t_ptr->frameChain2_rect_ptr) ;
    free (sys_t_ptr) ;


    /* Quit framework */
    SDL_Quit ();




    return (0) ;
}



/* ****************************************************************************************************************
* function that receives [i=lig][j=col] of a cell from the bubs_array
* returns the _ptr to SDL_Rect object updated with coords /x and y OF TOP LEFT CORNER/
* so that main program can position the bub
* WARNING : coords are for top left corner
* ************************************************************************************************************** */

SDL_Rect * sys_getBubPositionRect(game_t * game_t_ptr, int i, int j, SDL_Rect * dumRect_ptr) {

    /* distance between each bub */
    int d_x = (BOARD_RIGHT - BOARD_LEFT) / 8;

    /* there are 8 bubs on even rows
     * there are 7 bubs on odd rows
     * for odd rows (2d option of ternary op) we add a shift to the right */
    dumRect_ptr->x = (i % 2 == 0) ? BOARD_LEFT + j*d_x : BOARD_LEFT + j*d_x + BUB_SIZE / 2;

    dumRect_ptr->y = BOARD_TOP  + (35 * i)  + (35 * game_t_ptr->roofShift) ; //35 because 35 = 40 * sqrt(3)/2 and with that bubs are close to each other

    return dumRect_ptr ;
}


/* ****************************************************************************************************************
*   Handles changes to make when changing states
* ************************************************************************************************************** */
int sys_changeScreen (sys_t * sys_t_ptr, game_t * game_t_ptr, bub_t * bub_t_ptr, int newScreen) {


    /* stop music */
    sys_stopAllSounds (sys_t_ptr) ;

    switch (newScreen) {

        case SCREEN_WELCOME :

            break ;

        case SCREEN_PLAYING :

            game_newGame (sys_t_ptr, game_t_ptr, bub_t_ptr) ;

            break ;

        case SCREEN_VICTORY :

            sys_playSound (sys_t_ptr, SOUND_VICTORY) ;

            break ;

        case SCREEN_GAMEOVER :

            sys_playSound (sys_t_ptr, SOUND_GAMEOVER) ;

            break ;


    }

    sys_t_ptr->screen = newScreen ;

    return 0 ;
}


/* ****************************************************************************************************************
*   Play sound
* ************************************************************************************************************** */
int sys_playSound (sys_t * sys_t_ptr, int sound) {

    switch (sound) {

        case SOUND_VICTORY :

            if(Mix_PlayChannel(-1, sys_t_ptr->snd_levelCompleted, 0)==-1) {
                printf("Mix_PlayChannel: %s\n",Mix_GetError()) ;
            }

            break ;


        case SOUND_GAMEOVER :

            if(Mix_PlayChannel(-1, sys_t_ptr->snd_gameOver, 0)==-1) {
                printf("Mix_PlayChannel: %s\n",Mix_GetError()) ;
            }

            break ;



    }

    return 0 ;
}



/* ****************************************************************************************************************
*   Stop all sounds and Music playing
* ************************************************************************************************************** */
int sys_stopAllSounds (sys_t * sys_t_ptr) {

    /* stop music */
    Mix_HaltChannel(-1);

    sys_t_ptr->isPlayingMusic = false ;

    return 0 ;
}





/* ****************************************************************************************************************
*   Returns a random number between 0 and (max - 1)
* ************************************************************************************************************** */
short getRandomNumber(int max) {

    clock_t start_t ;

    /* Get number of CPU ticks since program started */
    start_t = clock();

    /* Initializes random number generator */
    srand (start_t) ;

    /* Generates numbers from 0 to max */
    return rand() % max ;
}


/* ****************************************************************************************************************
*   Detects SDL events
* ************************************************************************************************************** */
void sys_handleEvent (SDL_Event event, sys_t * sys_t_ptr, game_t * game_t_ptr, bub_t * bub_t_ptr)
{
    switch (event.type) {
        /* close button  clicked */
        case SDL_QUIT:
            game_t_ptr->quit = 1;
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
                case SDLK_SPACE:



                    if(!bub_t_ptr->isLaunching) {
                        bub_t_ptr->isLaunching = true ;
                    }
                    break ;

                case SDLK_j :

                    /* switch to state "playing" */
                    sys_changeScreen (sys_t_ptr, game_t_ptr, bub_t_ptr, SCREEN_PLAYING) ;

                    break ;

                case SDLK_r :

                    /* switch to state "playing" */
                    sys_changeScreen (sys_t_ptr, game_t_ptr, bub_t_ptr, SCREEN_PLAYING) ;

                    break ;


                default:
                    break;
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                case SDLK_q:
                    game_t_ptr->quit = 1;
                    break;
                case SDLK_LEFT:
                    /* launcher rotates to the left, unless already at extreme left */
                    if (game_t_ptr->launcherOrientation > 0) {
                        game_t_ptr->launcherOrientation -= 1 ;
                    }
                    break;
                case SDLK_RIGHT:
                    // launcher rotates to the right, unless already at far right
                    if (game_t_ptr->launcherOrientation < 44) {
                        game_t_ptr->launcherOrientation += 1 ;
                    }
                    break;

                default:
                    break;
            }
            break;
    }
}