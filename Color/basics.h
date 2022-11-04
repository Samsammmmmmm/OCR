#ifndef BASICS_H
#define BASICS_H

#include <gtk/gtk.h>
#include <stdio.h>

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


SDL_Surface *load_image(const char *path);
void save_image(SDL_Surface *image, char *path);
static inline Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
Image SDL_Surface_to_Image(SDL_Surface *surface);
SDL_Surface Image_to_SDL_Surface(Image image);
int *Image_to_Array(Image *source);
Image *Array_to_Image(int *source, int w, int h);
void free_image(Image *image);
int *image_grayscale_histogram(Image *image, int startx, int endx, int starty,
                               int endy);
int get_histogram_min(int *hist);
int get_histogram_max(int *hist);
void image_filter(Image *image, int startx, int endx, int starty, int endy, int
                  (*filter)(int));
#endif
