#include "header.h"
#include <SDL2/SDL.h>
#include <stdio.h>


int main(void){
	int game_running = show_window();
  	while (game_running)
  	{
  		show_window();
  	}
  	return 0;
}




