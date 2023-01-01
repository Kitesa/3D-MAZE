#include "../headers/header.h"
#include <stdio.h>
/* define map globally a lot of functions care about this */
int map[MAP_NUM_ROWS][MAP_NUM_COLS];

void loadMap(Point* playerPosition, double *playerEyeAngle){
	int i;
	int j;
	double s;
	int mapCoord[MAP_NUM_ROWS][MAP_NUM_COLS] = {
	{6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
	{6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 6},
	{6, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 6},
	{6, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 6, 1, 7, 7, 0, 0, 0, 0, 6},
	{6, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 6, 9, 0, 0, 0, 0, 7, 0, 6},
	{6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 7, 0, 6},
	{6, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 6},
	{6, 2, 2, 2, 1, 1, 7, 7, 7, 2, 2, 1, 1, 2, 2, 0, 0, 7, 2, 6},
	{6, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 6, 1, 0, 0, 0, 0, 0, 0, 6},
	{6, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 6},
	{6, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 6},
	{6, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 7, 0, 0, 0, 0, 0, 0, 6},
	{6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
	{6, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
	{6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
	{6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6}
	};
	/* read player starting x and y positions, as well as a starting angle of vision */
	s = 10.0;
	playerPosition->x = s;
	s = 16.0;
	playerPosition->y = s;
	s = 0;
	*playerEyeAngle = s;
	/* readin the map */ 
	for ( i = 0; i < 16; i++){
		for(j = 0; j < 20; j++){
			if (mapCoord[i][j] == 0){
				map[i][j] = 0;
			}else if(mapCoord[i][j] == 9){
				map[i][j] = 9;
			}else{
				map[i][j] = 1;
			}
		}
	}
}

Point distanceToWall(Point playerPosition, double playerEyeRAyAngle){
	const double raystep = 0.010;
	double delta_x = raystep * cos(playerEyeRAyAngle);
	double delta_y = raystep * sin(playerEyeRAyAngle);
	double distance = 0.0;
	Point r;
	double x, y;
	for (x = playerPosition.x, y = playerPosition.y; !map[(int)round(x)][(int)round(y)]; x += delta_x, y += delta_y){
		distance += raystep;
		if (distance > 100){
			r.x = 100.0;
			r.y = 0.0;
			return r;
		}
	}
	if (fabs(x - round(x)) > fabs(y - round(y))){
		r.y = modf(y, &delta_x);
	}else{
		r.y = modf(x, &delta_y);
	}
	r.x = distance;
	return r;
}


void playerMovementControl(double horizontal_change, double vertical_change){
	if (!map[(int)round(playerPos.x + horizontal_change)][(int)round(playerPos.y)])
	{
		playerPos.x += horizontal_change;
		
	}
	if (!map[(int)round(playerPos.x)][(int)round(playerPos.y + vertical_change)])
	{
		playerPos.y += vertical_change;
		
	}
	
}

/**
 * whereIsThePLayer - function to draw a map to show th player position
 * @ins: the given SDL instance
 *
 * Return: nothing
 **/
void whereIsThePLayer(SDL_Renderer *renderer)
{
	int counter_x, counter_y;
	SDL_Rect rect;
	SDL_Rect player;
	for (counter_x = 0; counter_x < map_x; counter_x++)
	{
		for (counter_y = 0; counter_y < map_y; counter_y++)
		{
			if (map[counter_x][counter_y] == 1)
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			else
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			rect.x = (counter_y * map_s * MAP_SCALE) + 1;
			rect.y = (counter_x * map_s * MAP_SCALE) + 1;
			rect.w = (map_s * MAP_SCALE) - 1;
			rect.h = (map_s * MAP_SCALE) - 1;
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	player.x = playerPos.x;
	player.y = playerPos.y;
	player.w = (map_s * MAP_SCALE) - 1;
	player.h = (map_s * MAP_SCALE) - 1;
	SDL_RenderFillRect(renderer, &player);
	SDL_RenderPresent(renderer);
}
