#include "../headers/header.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * playerWeapon - function to draw a weapon
 * Description: a function to add a player gun
 *
 * Return: nothing
 **/
void playerWeapon(void)
{
	/** variables for the sdl, width & height **/
	const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;
	SDL_Texture *texture = NULL;
	SDL_Rect gun;
	int w, h;
	SDL_Surface *surf = IMG_Load("../images/gun1.png");
	/** if the image found **/
	if (surf)
	{
		/** remove the background white color **/
		SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 255, 255, 255));
		/** create a texture **/
		texture = SDL_CreateTextureFromSurface(renderer, surf);
		/** query the texture to find width & height of the image **/
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		gun.w = w * gun_scale, gun.h =  h * gun_scale;
		gun.x = (SCREEN_WIDTH - gun.w) / 2;
		gun.y = (SCREEN_HEIGHT - gun.h) + 10;
		/** draw the weapon with the above specification **/
		if (texture)
			SDL_RenderCopy(renderer, texture, NULL, &gun);
		else
			printf("unable to create texture\n");
		/** free & destroy the surface **/
		SDL_FreeSurface(surf);
		SDL_DestroyTexture(texture);
	}
	else
		printf("unable to crate a surface\n");
}
