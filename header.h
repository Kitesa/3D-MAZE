#ifndef header_h
#define header_h
#include <stdbool.h>
#include <SDL2/SDL.h>
#define MAPSIZE 1000
/* useful struct tyoedef */
typedef struct Point {
	double x;
	double y;
} Point;
double playerA = 0.0;
/* create player position */
Point playerPos;
/* textures can be global as well */
SDL_Surface* wall_img_surf;

/* FUNCTIONS PROTOTYPE */
Point distanceToWall(Point playerPosition, double playerEyeRAyAngle);
void readMap(Point* playerPosition, double *playerEyeAngle);
void playerMovementControl(double horizontal_change, double vertical_change);

/* MOUSE AND KEYBOARD EVENT HANDLER */
bool handleAllEvents(SDL_Event);

/* DRAW EVERY THING ON WINDOW SCREEN */
void drawScreen(SDL_Renderer* renderer, SDL_Texture* screen);
#endif
