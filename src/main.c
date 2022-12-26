#include "../headers/header.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
double playerA = 0.0;

/* define map globally a lot of functions care about this */
int map[MAPSIZE][MAPSIZE];

int main()
{
  runGame();
  return EXIT_SUCCESS;
}


/* read map afrom stdin it sets the user position too */

void readMap(Point* playerPosition, double *playerEyeAngle){
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
  /*First line on the map is player x position at the beginning */
  fgets(s, MAPSIZE, mapfile);
  playerPosition->x = atof(s);
  /*second line on the map is player y position at the beginning */
  fgets(s, MAPSIZE, mapfile);
  playerPosition->y = atof(s);
  /*THird line on the map is player angle of player eye position at the beginning it is set to 0*/
  fgets(s, MAPSIZE, mapfile);
  *playerEyeAngle = atof(s);
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

Point distanceToWall(Point playerPosition, double playerEyeRAyAngle){
  const double raystep = 0.010;
  double delta_x = raystep * cos(playerEyeRAyAngle);
  double delta_y = raystep * sin(playerEyeRAyAngle);
  double distance = 0.0;
  Point r;

  /* cast a ray from player eye to the wall*/
  double x, y;
  for (x = playerPosition.x, y = playerPosition.y; !map[(int)round(x)][(int)round(y)]; x += delta_x, y += delta_y){
    distance += raystep;
    if (distance > 100){
      r.x = 100.0;
      r.y = 0.0;
      return r;
    }
  }
  /* decide where we hit the wall. if an x or y value is very close to
     0.5, it has just hit a wall. so we wan.................


  */
  if (fabs(x - round(x)) > fabs(y - round(y))){
    r.y = modf(y, &delta_x);
  }else{
    r.y = modf(x, &delta_y);
  }
  r.x = distance;

  return r;
}

bool mouseEvents(SDL_Event mouse_event){
  /* Assume speed at which the player can rotate */
  const double player_angular_speed_of_rotation = 0.025;
  
  /* mouse events for 360 degree rotation */
  while(SDL_PollEvent(&mouse_event) != 0){
    if (mouse_event.type == SDL_QUIT){
      return true;
    }
    if (mouse_event.type == SDL_MOUSEMOTION){
      /* relative motion in the X direction times angular speed*/
      playerA += mouse_event.motion.xrel * player_angular_speed_of_rotation;
    }
  }
  return false;
}


bool handleAllEvents(SDL_Event e){
  const double player_linear_speed= 0.0875;
  const Uint8* currentKeyStates;
  currentKeyStates = SDL_GetKeyboardState(0);
  /* call mouse based event for 360 degree rotation of a player */
  mouseEvents(e);
  /* scan keyboard events */
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

void drawScreen(SDL_Renderer *renderer, SDL_Texture *screen){
  const double DIST_TO_PLANE_OF_PROJECTION= 1.0;
  /* screen dimension contants */
  const int SCREEN_WIDTH = 640;
  const int SCREEN_HEIGHT = 480;
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

  /* draw the wall throughout width of the screen and */
  for (x = 0; x < SCREEN_WIDTH; x++){
    double playerEyerayAngle;
    double rayCastAngle;
    Point rayReturn;
    double horizontal_distance;
    int wallHeight;
    int actualWAllHeight;
    int wallY;
    double wallcoord;
    Uint8 color;
    
    /* tan(rayAngle) = (change in y) / (change in x) 
       implies that rayAngle = tan inverse of (change in y) / (change in x).
       change in x is neasured from player position to wall */
    playerEyerayAngle = atan((x - SCREEN_WIDTH / 2.0) / (SCREEN_WIDTH / 2.0));
    /* raycast angle is equal to rayangle */
    rayCastAngle = playerA + playerEyerayAngle;
    
    /* return the distance from player to the wall */
    rayReturn = distanceToWall(playerPos, rayCastAngle);

    /* modify the distance to counter effect */
    /* horizontal distance = hypoteneous * cos(teta)
       where teta is an angle from horizontal eye level
       and hypoteneous is measured along eye view from player eye to
       wall. eye vision have two dim, hor and vertical.... we take hor
       here
    */
    
    horizontal_distance = rayReturn.x * cos(playerEyerayAngle);

    /* calculate apparent height of the wall to the player */
    wallHeight = DIST_TO_PLANE_OF_PROJECTION / horizontal_distance * SCREEN_HEIGHT;
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
      if (horizontal_distance > DIST_TO_PLANE_OF_PROJECTION){
	texture_x = wallcoord * 127;
	texture_y = (y - wallY) / (float)wallHeight * 127;
      }else{
	texture_x = wallcoord * 127;
	texture_y = ((actualWAllHeight - wallHeight) / 2.0 + y) / (float)actualWAllHeight * 127;
      }

      /* read the color on the image texture in /images/wall3.bmp */
      r = *(((Uint8*) (wall_img_surf->pixels)) + texture_y * 3*128 + 3*texture_x + 0);
      g = *(((Uint8*) (wall_img_surf->pixels)) + texture_y * 3*128 + 3*texture_x + 1);
      b = *(((Uint8*) (wall_img_surf->pixels)) + texture_y * 3*128 + 3*texture_x + 2);
      
      /* make a little bit adjustement to color of the wall based on
	 distamce from the wall */
      /* calculate what color the wall should be based on distance */
      color= 255.0 * exp(-horizontal_distance/30);
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

/* moves the player if there is no wall only. no wall meand the map read is zero */
void playerMovementControl(double horizontal_change, double vertical_change){
  /* moves the player in x dxn if there is no wall only */
  if (!map[(int)round(playerPos.x + horizontal_change)][(int)round(playerPos.y)])
    {
      /*configuration can be done here to increase speed control */
      playerPos.x += horizontal_change;
      
    }
  
  /* moves the player in y dxn if there is no wall only */
  if (!map[(int)round(playerPos.x)][(int)round(playerPos.y + vertical_change)])
    {
      playerPos.y += vertical_change;
    }
  
}
