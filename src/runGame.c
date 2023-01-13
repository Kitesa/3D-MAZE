#include "../headers/header.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

/*
 * loopDisplay - display the window loop
 * Description: 
 * Return: void
 */
 
void loopDisplay(){
	bool quit = false;
	const int FPS = 60;
	while(!quit){
		Uint32 framestart, frameTime, delayTime;
		unsigned int time;
		framestart = SDL_GetTicks();
		quit = handleAllEvents(e);
		drawScreen(renderer, screen);
		playerWeapon();
		frameTime = SDL_GetTicks() - framestart;
		delayTime = 1000 / FPS - frameTime;
		time = 1000;
		if (delayTime > time / FPS) delayTime = 0;
		SDL_Delay(delayTime);	
	}
}
/*
 * runGame - a function that run a game
 * Description:
 * Return: None
 *
 */
int runGame(void){
	double playerA = 0.0;
	const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return (1);
	}
	window = SDL_CreateWindow("ALX Maze project", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (renderer == NULL)
	{
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (1);
	}
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		fprintf(stderr, "Failed to initialize SDL_image.\n");
		return (1);
	}	
	SDL_SetRelativeMouseMode(SDL_TRUE);
	loadMap(&playerPos, &playerA);
	w_surf = SDL_LoadBMP("../images/wall3.bmp");
	loopDisplay();
	SDL_DestroyTexture(screen);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (1);
}
