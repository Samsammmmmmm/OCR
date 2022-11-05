#ifndef BASICS_H
#define BASICS_H

#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


SDL_Surface *load_image(const char *path);
void save_image(SDL_Surface *image, char *path);
Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int *surface_to_histogram(SDL_Surface *surface);
void print_pic(SDL_Renderer* renderer,SDL_Texture* texture );
#endif
