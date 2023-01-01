#include "../headers/header.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
double playerA = 0.0;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
void *pixels = NULL;
int y;
int x;
/* 
 * mouseEvents - 
 * @mouse_event: mouse event
 * Description:
 * Return: false
 */
bool mouseEvents(SDL_Event mouse_event){
	const double player_angular_speed_of_rotation = 0.025;
	while(SDL_PollEvent(&mouse_event) != 0){
		if (mouse_event.type == SDL_QUIT){
			return true;
		}
		if (mouse_event.type == SDL_MOUSEMOTION){
			playerA += mouse_event.motion.xrel * player_angular_speed_of_rotation;
		}
	}
	return false;
}

/*
 * handleAllEvents - 
 * @e: event
 * Description:
 * Return: false
 */
 
bool handleAllEvents(SDL_Event e){
	const double player_linear_speed= 0.0875;
	const Uint8* currentKeyStates;

	currentKeyStates = SDL_GetKeyboardState(0);
	mouseEvents(e);
	if (currentKeyStates[SDL_SCANCODE_ESCAPE]){
		return true;
	}
	if(currentKeyStates[SDL_SCANCODE_UP]){
		double delta_x = cos(playerA) * player_linear_speed;
		double delta_y = sin(playerA) * player_linear_speed;
		playerMovementControl(delta_x, delta_y);
	}
	if(currentKeyStates[SDL_SCANCODE_DOWN]){
		double delta_x = cos(playerA) * player_linear_speed;
		double delta_y = sin(playerA) * player_linear_speed;
		playerMovementControl(-delta_x, -delta_y);
	}

	if(currentKeyStates[SDL_SCANCODE_LEFT]){
		double delta_x = cos(playerA - 1.57) * player_linear_speed;
		double delta_y = sin(playerA - 1.57) * player_linear_speed;
		playerMovementControl(delta_x, delta_y);
	}

	if(currentKeyStates[SDL_SCANCODE_RIGHT]){
		double delta_x = cos(playerA + 1.57) * player_linear_speed;
		double delta_y = sin(playerA + 1.57) * player_linear_speed;
		playerMovementControl(delta_x, delta_y);
	}
	return false;
}

/*
 * drawFloor - a function to draw a floor
 * Description:
 * Return: void
 *
 */
 
void drawFloor(){
	for (y = 0; y < SCREEN_HEIGHT / 2; y++){
		Uint8 color = 255.0 - 255.0 * ((float)y / (float)SCREEN_HEIGHT);
		Uint32 floorColor = ((0 + color) << 16) + 255;

		for (x = 0; x < SCREEN_WIDTH; x++){
			*(((Uint32*) pixels) + SCREEN_WIDTH * (SCREEN_HEIGHT - 1) - y * SCREEN_WIDTH + x) = floorColor;
		}
	}
}

/*
 * drawCiel - a function to draw a ciel
 * Description:
 * Return: void
 *
 */
void drawCiel(){
	for (y = 0; y < SCREEN_HEIGHT / 2; y++){
		Uint8 color = 255.0 - 255.0 * ((float)y / (float)SCREEN_HEIGHT);
		Uint32 ceilColor = ((0 + color) << 8) + 255;
		for (x = 0; x < SCREEN_WIDTH; x++){
			*(((Uint32*) pixels) + SCREEN_WIDTH * y + x) = ceilColor;
		}
	}

}

/* 
 * drawWall - a function that draws a wall
 * Description:
 * Return: Void
 *
 */
  
void drawWall(){
	const double DIST_TO_PLANE_OF_PROJECTION= 1.0;
		Point rayReturn;
		int wallHeight, actualWAllHeight, wallY;
		Uint8 color;
		playerEyerayAngle = atan((x - SCREEN_WIDTH / 2.0) / (SCREEN_WIDTH / 2.0));
		rayCastAngle = playerA + playerEyerayAngle;
		rayReturn = distanceToWall(playerPos, rayCastAngle);
		horizontal_distance = rayReturn.x * cos(playerEyerayAngle);
		wallHeight = DIST_TO_PLANE_OF_PROJECTION / horizontal_distance * SCREEN_HEIGHT * 1.5;
		actualWAllHeight = wallHeight;
		wallHeight = (wallHeight > SCREEN_HEIGHT) ? SCREEN_HEIGHT : wallHeight;
		wallY = (int)((SCREEN_HEIGHT - wallHeight) / 2);
		wallcoord = rayReturn.y;
		for (y = wallY; y < wallHeight + wallY; y++){
			int texture_x = 0, texture_y = 0;
			Uint8 r, g, b;
			Uint32 wallColor = 0;
			if (horizontal_distance > DIST_TO_PLANE_OF_PROJECTION){
				texture_x = wallcoord * 127;
				texture_y = (y - wallY) / (float)wallHeight * 127;
			}else{
				texture_x = wallcoord * 127;
				texture_y = ((actualWAllHeight - wallHeight) / 2.0 + y) / (float)actualWAllHeight * 127;
			}
			r = *(((Uint8*) (wall_img_surf->pixels)) + texture_y * 3*128 + 3*texture_x + 0);
			g = *(((Uint8*) (wall_img_surf->pixels)) + texture_y * 3*128 + 3*texture_x + 1);
			b = *(((Uint8*) (wall_img_surf->pixels)) + texture_y * 3*128 + 3*texture_x + 2);
			color= 255.0 * exp(-horizontal_distance/30);
			r *= color / 255.0, g *= color / 255.0, b *= color / 255.0;
			wallColor = (((((b << 8) + g) << 8) + r) << 8) + 255;
			*(((Uint32*) pixels) + SCREEN_WIDTH * y + x) = wallColor;
		}

	}
}

/*
 * drawScreen - a function that draws every thing on the screen
 * @renderer: a renderer on which every thing is displayed
 * @screen: a creen of texture
 * Description:
 * Return: void
 *
 */
 
void drawScreen(SDL_Renderer *renderer, SDL_Texture *screen){
	int pitch;
	SDL_LockTexture(screen, NULL, &pixels, &pitch);
	drawFloor();
	drawCiel();
	drawWall();
	whereIsThePLayer(renderer);
	SDL_UnlockTexture(screen);
	SDL_RenderCopy(renderer, screen, NULL, NULL);

}

