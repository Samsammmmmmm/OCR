#ifndef ROTA_H
#define ROTA_H

#include <err.h>
/*#include </opt/homebrew/include/SDL2/SDL.h>
  #include </opt/homebrew/Cellar/sdl2_image/2.6.2/include/SDL2/SDL_image.h>*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../Image_processing/basics.h"

void draw(SDL_Renderer* renderer, SDL_Texture* texture, double angle);

void event_loop(SDL_Renderer* renderer, SDL_Texture* texture, double angle_p);

SDL_Surface* load_image(const char* path);

int IsPixelInImage(SDL_Surface *surface, int x, int y);

SDL_Surface *manual_rota(SDL_Surface *surface, int angle);

int main(int argc, char **argv);

#endif
