#include <err.h>
#include </opt/homebrew/include/SDL2/SDL.h>
#include </opt/homebrew/Cellar/sdl2_image/2.6.2/include/SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define THRESHOLD 0.4

void hough(SDL_Surface* image_surface, SDL_Renderer* renderer);
