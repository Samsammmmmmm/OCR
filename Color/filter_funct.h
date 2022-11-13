#ifndef FILTER_FUNCT_H
#define FILTER_FUNCT_H

#include <err.h>
#include <stdio.h>
#include "basics.h"

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
void surface_to_grayscale(SDL_Surface* surface);
void otsu_tresholding(SDL_Surface *surface);
void sauvola_tresholding(SDL_Surface *surface, int radius, float k);
void median_filter(SDL_Surface *surface, int radius);
void gaussian_smoothing(SDL_Surface *surface, int sigma, int kernel);
void filter_contrast(SDL_Surface *surface, int contrast);
void filter_brightness(SDL_Surface *surface, int brightness);
void gamma_filter(SDL_Surface *surface, float gamma);

#endif
