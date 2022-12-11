#ifndef PRINT_SOLU_H
#define PRINT_SOLU_H

#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Image_processing/basics.h"

void image_result(SDL_Surface* grid, char file01[], char file02[]);

void image_result_solu(char file01[], char file02[]);

#endif
