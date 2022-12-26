#include "../headers/header.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

int runGame(void){
	bool quit = false;
	double playerA = 0.0;
  	/* screen dimension contants */
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const int FPS = 60;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event e;
	SDL_Texture* screen;
	/** if the window is not initialized **/
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return (1);
	}
	/* CREATE WINDOW */
	window = SDL_CreateWindow("ALX Maze project", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	
	/** create the renderer **/
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	/* create new texture */
	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (renderer == NULL)
	{
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	/** if the sdl image is not initialized **/
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		fprintf(stderr, "Failed to initialize SDL_image.\n");
		return (1);
	}
	/* GRAB MOUSE AND OTHER EVENYS */
	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	/*READ MAP FROM INPUT */
	readMap(&playerPos, &playerA);
	
	/*load in wall texture image */
	wall_img_surf = SDL_LoadBMP("../images/wall3.bmp");
	
	/* main loop */
	while(!quit){
		/* get time at start of frame */
		Uint32 framestart;
		Uint32 frameTime;
		Uint32 delayTime;
		unsigned int time;
		framestart = SDL_GetTicks();
		quit = handleAllEvents(e);
		drawScreen(renderer, screen);
		
		/* sdl update windowsurfacewindow */
		frameTime = SDL_GetTicks() - framestart;
		/* wait until we hit 60 fps */
		
		delayTime = 1000 / FPS - frameTime;
		/*printf frametime*/
		
		time = 1000;
		if (delayTime > time / FPS) delayTime = 0;
		SDL_Delay(delayTime);
	}
       
	SDL_DestroyTexture(screen);
	
	/* destroy window */
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return(1);
}
