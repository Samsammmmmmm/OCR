#ifndef FILTER_FUNCT_H
#define FILTER_FUNCT_H

#include <err.h>
#include <stdio.h>
#include "basics.h"

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
void surface_to_grayscale(SDL_Surface* surface);
void otsu_tresholding(SDL_Surface *surface);
<<<<<<< HEAD
void sauvola_tresholding(SDL_Surface *surface, int radius, float k);
=======
void sauvola_tresholding(SDL_Surface* surface);
void filter_median(SDL_Surface* surface);
void filter_contrast(SDL_Surface* surface, int contrast);
>>>>>>> 7340b3e7c42286ffa36bc40b14f74f1cec2d4107
#endif
