#ifndef FILTER_FUNCT_H
#define FILTER_FUNCT_H

#include <err.h>
#include <stdio.h>
#include "basics.h"

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
void surface_to_grayscale(SDL_Surface* surface);
void otsu_tresholding(SDL_Surface *surface);

#endif
