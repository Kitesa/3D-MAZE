#include "../headers/header.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
void *pixels = NULL;
double plA = 0.0;
/**
 * handleAllEvents - function to handle keyboard and mouse event
 * @e: event
 * Description: handles all events
 * Return: false
 */

bool handleAllEvents(SDL_Event e)
{
	const double sp = 0.0875, player_ang_speed = 0.025;
	const Uint8 *currentKeyStates;

	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
			return (true);
		if (e.type == SDL_MOUSEMOTION)
			plA += e.motion.xrel * player_ang_speed;
	}
	currentKeyStates = SDL_GetKeyboardState(0);
	if (currentKeyStates[SDL_SCANCODE_ESCAPE])
	{
		return (true);
	}
	if (currentKeyStates[SDL_SCANCODE_UP])
	{
		double d_x = cos(plA) * sp, d_y = sin(plA) * sp;

		playerMovementControl(d_x, d_y);
	}
	if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		double d_x = cos(plA) * sp, d_y = sin(plA) * sp;

		playerMovementControl(-d_x, -d_y);
	}
	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		double d_x = cos(plA - 1.57) * sp, d_y = sin(plA - 1.57) * sp;

		playerMovementControl(d_x, d_y);
	}
	if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		double d_x = cos(plA + 1.57) * sp, d_y = sin(plA + 1.57) * sp;

		playerMovementControl(d_x, d_y);
	}
	return (false);
}

/**
 * drawFloor - a function to draw a floor
 * Description: draw a floor coloer
 * Return: void
 *
 */

void drawFloor(void)
{
	const int SCR_WIDTH = 640, SCREEN_HEIGHT = 480;

	for (y = 0; y < SCREEN_HEIGHT / 2; y++)
	{
		Uint8 color = 255.0 - 255.0 * ((float)y / (float)SCREEN_HEIGHT);
		Uint32 floorColor = ((0 + color) << 16) + 255;

		for (x = 0; x < SCR_WIDTH; x++)
		{
			*(((Uint32 *) pixels) + SCR_WIDTH * (SCREEN_HEIGHT - 1)
					- y * SCR_WIDTH + x) = floorColor;
		}
	}
}

/**
 * drawCiel - a function to draw a ciel
 * Description: draw a ciel from color
 * Return: void
 *
 */
void drawCiel(void)
{
	const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;

	for (y = 0; y < SCREEN_HEIGHT / 2; y++)
	{
		Uint8 color = 255.0 - 255.0 * ((float)y / (float)SCREEN_HEIGHT);
		Uint32 ceilColor = ((0 + color) << 8) + 255;

		for (x = 0; x < SCREEN_WIDTH; x++)
			*(((Uint32 *) pixels) + SCREEN_WIDTH * y + x) = ceilColor;
	}
}

/**
 * drawWall - a function that draws a wall
 * Description: draws a wall
 * Return: Void
 *
 */

void drawWall(void)
{
	const int S_W = 640, S_H = 480;
	const double DIST_TO_P_PRO = 1.0;

	for (x = 0; x < S_W; x++)
	{
		double plyAng, rayCastAngle, hor_dis, wallcoord;
		Point rayReturn;
		int wallHeight, actualWAllHeight, wallY;
		Uint8 color;

		plyAng = atan((x - S_W / 2.0) / (S_W / 2.0));
		rayCastAngle = plA + plyAng;
		rayReturn = distanceToWall(playerPos, rayCastAngle);
		hor_dis = rayReturn.x * cos(plyAng);
		wallHeight = DIST_TO_P_PRO / hor_dis * S_H * 1.5;
		actualWAllHeight = wallHeight;
		wallHeight = (wallHeight > S_H) ? S_H : wallHeight;
		wallY = (int)((S_H - wallHeight) / 2);
		wallcoord = rayReturn.y;
		for (y = wallY; y < wallHeight + wallY; y++)
		{
			int tex_x = 0, tex_y = 0;
			Uint8 r, g, b;
			Uint32 wallColor = 0;

			if (hor_dis > DIST_TO_P_PRO)
			{
				tex_x = wallcoord * 127;
				tex_y = (y - wallY) / (float)wallHeight * 127;
			}
			else
			{
				tex_x = wallcoord * 127;
				tex_y = ((actualWAllHeight - wallHeight) / 2.0 + y)
					/ (float)actualWAllHeight * 127;
			}
			r = *(((Uint8 *) (w_surf->pixels)) + tex_y * 3 * 128 + 3 * tex_x + 0);
			g = *(((Uint8 *) (w_surf->pixels)) + tex_y * 3 * 128 + 3 * tex_x + 1);
			b = *(((Uint8 *) (w_surf->pixels)) + tex_y * 3 * 128 + 3 * tex_x + 2);
			color = 255.0 * exp(-hor_dis / 30);
			r *= color / 255.0, g *= color / 255.0, b *= color / 255.0;
			wallColor = (((((b << 8) + g) << 8) + r) << 8) + 255;
			*(((Uint32 *) pixels) + S_W * y + x) = wallColor;
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

void drawScreen(SDL_Renderer *renderer, SDL_Texture *screen)
{
	int pitch;

	SDL_LockTexture(screen, NULL, &pixels, &pitch);
	drawFloor();
	drawCiel();
	drawWall();
	whereIsThePLayer(renderer);
	SDL_UnlockTexture(screen);
	SDL_RenderCopy(renderer, screen, NULL, NULL);
}
