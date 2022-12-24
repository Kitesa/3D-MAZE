#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#define MAPSIZE 256

/* game engine contants */
const double MOVESPEED = 0.0875;
const double ANGLESPEED = 0.0025625;
const double FOG = 20.0;
const double VERTICALFOV = 1.257;
const double DIST_TO_PROJ_PLANE = 1.0;
const int FPS = 60;

/* useful struct tyoedef */
typedef struct fvector2 {
	double x;
	double y;
} fvector2;

/* FUNCTIONS PROTOTYPE */
fvector2 distanceToWall(fvector2 playerPos, double playerA);
void readMap(fvector2* playerPos, double *playerA);
void playerMove(double dx, double dy);
bool handleEvents(SDL_Event);
double veclen(fvector2 vec);
void drawScreen(SDL_Renderer* renderer, SDL_Texture* screen);

/* screen dimension contants */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
/* create player position */
fvector2 playerPos;
double playerA = 0.0;


/* define map globally a lot of functions care about this */
int map[MAPSIZE][MAPSIZE];


/* textures can be global as well */
SDL_Surface* wall_img_surf;


int main()
{
	bool quit = false;
	
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event e;
	SDL_Texture* screen;
	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);

	/* CREATE WINDOW */


	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	/* create new texture */
	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	/* GRAB MOUSE AND OTHER EVENYS */
	SDL_SetRelativeMouseMode(SDL_TRUE);
	

	/*READ MAP FROM INPUT */
	readMap(&playerPos, &playerA);

	/*load in wall texture image */
	wall_img_surf = SDL_LoadBMP("images/wall.bmp");

	/* main loop */
	while(!quit){
		/* get time at start of frame */
		Uint32 framestart;
		Uint32 frameTime;
		Uint32 delayTime;
		unsigned int time;
		framestart = SDL_GetTicks();
		quit = handleEvents(e);
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
	return EXIT_SUCCESS;
}

fvector2 distanceToWall(fvector2 playerPos, double playerA){
	const double raystep = 0.0150;

	double dx = raystep * cos(playerA);
	double dy = raystep * sin(playerA);
	double distance = 0.0;

	fvector2 r;

	/* cast ray out from player */
	double x, y;
	for (x = playerPos.x, y = playerPos.y; !map[(int)round(x)][(int)round(y)]; x += dx, y += dy){
		distance += raystep;
		if (distance > 1000){
			r.x = 1000.0;
			r.y = 0.0;
			return r;
		}
	}
		/* decide where we hit the wall. if an x or y value is very close to
		0.5, it has just hit a wall. so we wan.................


		*/
	if (fabs(x - round(x)) > fabs(y - round(y))){
		r.y = modf(y, &dx);
	}else{
		r.y = modf(x, &dx);
	}
	r.x = distance;

	return r;
}

/* read map afrom stdin it sets the user position too */
void readMap(fvector2* playerPos, double *playerA){
	int i;
	int j;
	FILE* mapfile;
	char s[MAPSIZE] = {0};
	for (i = 0; i < MAPSIZE; i++)
		for (j= 0; j < MAPSIZE; j++)
			map[i][j] = 0;
	/* open map file for reading */
	mapfile = fopen("map.txt", "r");
	/* read player starting x and y positions, as well as a starting rectangle */

	fgets(s, MAPSIZE, mapfile);
	playerPos->x = atof(s);
	fgets(s, MAPSIZE, mapfile);
	playerPos->y = atof(s);
	fgets(s, MAPSIZE, mapfile);
	*playerA = atof(s);
	/* readin the map */ 
	for ( i = 0; fgets(s, MAPSIZE, mapfile) != NULL; i++){
		for(j = 0; s[j] != '\0'; j++){
			if (s[j] == '#'){
				map[j][i] = 1;
			}else{
				map[j][i] = 0;
			}
		}
	}
	fclose(mapfile);
}
bool handleEvents(SDL_Event e){
	const Uint8* currentKeyStates;
	currentKeyStates = SDL_GetKeyboardState(0);
	/* handle events in queue */
	while(SDL_PollEvent(&e) != 0){
		if (e.type == SDL_QUIT){
			return true;
		}
		if (e.type == SDL_MOUSEMOTION){
			playerA += e.motion.xrel * ANGLESPEED;
		}
	}
	/* scan keyboard events */
	if (currentKeyStates[SDL_SCANCODE_ESCAPE]){
		return true;
	}
	if(currentKeyStates[SDL_SCANCODE_W]){
		double dx = cos(playerA) * MOVESPEED;
		double dy = sin(playerA) * MOVESPEED;
		playerMove(dx, dy);
	}
	if(currentKeyStates[SDL_SCANCODE_S]){
		double dx = cos(playerA) * MOVESPEED;
		double dy = sin(playerA) * MOVESPEED;
		playerMove(-dx, -dy);
	}

	if(currentKeyStates[SDL_SCANCODE_A]){
		double dx = cos(playerA - 1.57) * MOVESPEED;
		double dy = sin(playerA - 1.57) * MOVESPEED;
		playerMove(dx, dy);
	}

	if(currentKeyStates[SDL_SCANCODE_D]){
		double dx = cos(playerA + 1.57) * MOVESPEED;
		double dy = sin(playerA + 1.57) * MOVESPEED;
		playerMove(dx, dy);
	}

	return false;
}

void drawScreen(SDL_Renderer *renderer, SDL_Texture *screen){
	void *pixels = NULL;
	int pitch;
	int y;
	int x;
	SDL_LockTexture(screen, NULL, &pixels, &pitch);

	/* draw floor */
	for (y = 0; y < SCREEN_HEIGHT / 2; y++){
		/* set the gradient color depending on how far 
		from the edge of the screen  we draw */
		Uint8 color = 255.0 - 255.0 * ((float)y / (float)SCREEN_HEIGHT);
		Uint32 ceilColor = ((0 + color) << 8) + 255;
		Uint32 floorColor = ((0 + color) << 16) + 255;

		/* set texture pixel to gradient colors */
		for (x = 0; x < SCREEN_WIDTH; x++){
			*(((Uint32*) pixels) + SCREEN_WIDTH * y + x) = ceilColor;
			*(((Uint32*) pixels) + SCREEN_WIDTH * (SCREEN_HEIGHT - 1) - y * SCREEN_WIDTH + x) = floorColor;
		}
	}

	/* draw the wall */
	for (x = 0; x < SCREEN_WIDTH; x++){
		double rayAngle;
		double castAngle;
		fvector2 rayReturn;
		double distance;
		int wallHeight;
		int actualWAllHeight;
		int wallY;
		double wallcoord;
		Uint8 color;
		
		rayAngle = atan((x - SCREEN_WIDTH / 2.0) / (SCREEN_WIDTH / 2.0));
		castAngle = playerA + rayAngle;
		rayReturn = distanceToWall(playerPos, castAngle);

		/* modify the distance to counter fisheyye iffect */
		distance = cos(rayAngle) * rayReturn.x;

		/* calculate what color the wall should be based on distance */
		color= 256.0 * exp(-distance/FOG);

		/* calculate apparent height of the wall to the player */
		wallHeight = DIST_TO_PROJ_PLANE / distance * SCREEN_HEIGHT;
		actualWAllHeight = wallHeight;
		wallHeight = (wallHeight > SCREEN_HEIGHT) ? SCREEN_HEIGHT : wallHeight;
		wallY = (int)((SCREEN_HEIGHT - wallHeight) / 2);

		/* get where we hit the wall */
		wallcoord = rayReturn.y;
		/* draw the wall */
		for (y = wallY; y < wallHeight + wallY; y++){
			/* get where on the texture we are */
			int texture_x = 0;
			int texture_y = 0;
			Uint8 r;
			Uint8 g;
			Uint8 b;
			Uint32 wallColor;
			if (distance > DIST_TO_PROJ_PLANE){
				texture_x = wallcoord * 127;
				texture_y = (y - wallY) / (float)wallHeight * 127;
			}else{
				texture_x = wallcoord * 127;
				texture_y = ((actualWAllHeight - wallHeight) / 2.0 + y) / (float)actualWAllHeight * 127;
			}

			/* read the color on the texture */
			r = *(((Uint8*) (wall_img_surf->pixels)) + texture_y * 3*128 + 3*texture_x + 0);
			g = *(((Uint8*) (wall_img_surf->pixels)) + texture_y * 3*128 + 3*texture_x + 1);
			b = *(((Uint8*) (wall_img_surf->pixels)) + texture_y * 3*128 + 3*texture_x + 2);
			
			r *= color / 255.0;
			g *= color / 255.0;
			b *= color / 255.0;

			
			wallColor = 0;
			wallColor = (((((b << 8) + g) << 8) + r) << 8) + 255;
			*(((Uint32*) pixels) + SCREEN_WIDTH * y + x) = wallColor;
		}

	}
	SDL_UnlockTexture(screen);
	SDL_RenderCopy(renderer, screen, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void playerMove(double dx, double dy){
	if (!map[(int)round(playerPos.x + dx)][(int)round(playerPos.y)])
	{
		playerPos.x += dx;
	}
	if (!map[(int)round(playerPos.x)][(int)round(playerPos.y + dy)])
	{
		playerPos.y += dy;
	}

}

double veclen(fvector2 vec){
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}
