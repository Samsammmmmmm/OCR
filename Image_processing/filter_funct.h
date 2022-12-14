#ifndef FILTER_FUNCT_H
#define FILTER_FUNCT_H

#include <err.h>
#include <stdio.h>
#include "basics.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <pthread.h>
#include <math.h>

SDL_Surface* block_resize_ff(SDL_Surface *block, int w, int h);
void save_resize_ff(SDL_Surface *block, int pix_w, int pix_h);
void surface_to_grayscale(SDL_Surface* surface);
void otsu_tresholding(SDL_Surface *surface);
void sauvola_tresholding(SDL_Surface *surface, int radius, float k);
void median_filter(SDL_Surface *surface);
void contrast(SDL_Surface *surface);
void gamma_filter(SDL_Surface *surface);
void sobel_filter(SDL_Surface *surface);
int img_process(char* filename);
#endif
