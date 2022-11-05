#ifndef BASICS_H
#define BASICS_H

#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Pixel
{
    Uint8 r, g, b;
};
typedef struct Pixel Pixel;

struct Image
{
    int height;
    int width;
    SDL_Surface *surface;
    Pixel **pixels;
    double average_color;
};
typedef struct Image Image;

double clamp(double d, double min, double max);
SDL_Surface *load_image(const char *path);
void save_image(SDL_Surface *image, char *path);
Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int *image_grayscale_histogram(Image *image, int startx, int endx, int starty,
                               int endy);
int get_histogram_min(int *hist);
int get_histogram_max(int *hist);
void print_pic(SDL_Renderer* renderer,SDL_Texture* texture );
#endif
