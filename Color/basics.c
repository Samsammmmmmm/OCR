#include <stdio.h>
#include <maths.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <float.h>
#include <gtk/gtk.h>
#include <limits.h>
#include "basics.h"

SDL_Surface *load_image(const char *path)
{
    SDL_Surface *image = IMG_Load(path);
    if (image == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    return image;
}

void save_image(SDL_Surface *image, char *path)
{
    int saved = IMG_SavePNG(image, path);
    if (saved != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
}

static inline Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch (bpp)
    {
    case 1:
        return *p;
    case 2:
        return *(Uint16 *)p;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32 *)p;
    default:
        return 0;
    }
}

void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch (bpp)
    {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(Uint16 *)p = pixel;
        break;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

Image SDL_Surface_to_Image(SDL_Surface *surface)
{
    Image image;
    image.width = surface->w;
    image.height = surface->h;
    image.pixels = malloc(image.width * image.height * sizeof(Uint32));
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            Uint32 pixel = get_pixel(surface, i, j);
            image.pixels[i + j * image.width] = pixel;
        }
    }
    return image;
}

SDL_Surface Image_to_SDL_Surface(Image image)
{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, image.width, image.height, 32, 0, 0, 0, 0);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            Uint32 pixel = image.pixels[i + j * image.width];
            put_pixel(surface, i, j, pixel);
        }
    }
    return surface;
}

int *Image_to_Array(Image *source)
{
    int *result = malloc(sizeof(int) * source->width * source->height);
    int w = source->width, h = source->height;
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            result[x + y * w] = source->pixels[x][y].r;
        }
    }
    return result;
}

Image *Array_to_Image(int *source, int w, int h)
{
    Image *result = new_Image(w, h);
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            result->pixels[x][y].r = source[x + y * w];
            result->pixels[x][y].g = source[x + y * w];
            result->pixels[x][y].b = source[x + y * w];
        }
    }
    return result;
}

void free_image(Image *image)
{
    for (int x = 0; x < image->width; x++)
    {
        free(image->pixels[x]);
    }
    free(image->pixels);
    free(image);
}

int *image_grayscale_histogram(Image *image, int startx, int endx, int starty, int endy)
{
    int *hist = calloc(sizeof(int), 256);
    for (int x = startx; x < endx && x < image->width; x++)
    {
        for (int y = starty; y < endy && y < image->height; y++)
        {
            hist[image->pixels[x][y].r]++;
        }
    }
    return hist;
}

int get_histogram_min(int *hist)
{
    int min = INT_MAX;
    int minc = 0;
    for (int i = 0; i < 256; i++)
    {
        if (hist[i] < min)
        {
            min = hist[i];
            minc = i;
        }
    }
    return minc;
}

int get_histogram_max(int *hist)
{
    int max = INT_MIN;
    int maxc = 0;
    for (int i = 0; i < 256; i++)
    {
        if (hist[i] > max)
        {
            max = hist[i];
            maxc = i;
        }
    }
    return maxc;
}

void image_filter(Image *image, int startx, int endx, int starty, int endy, int (*filter)(int))
{
    for (int x = startx; x < endx && x < image->width; x++)
    {
        for (int y = starty; y < endy && y < image->height; y++)
        {
            int r = filter(image->pixels[x][y].r);
            int g = filter(image->pixels[x][y].g);
            int b = filter(image->pixels[x][y].b);
            image->pixels[x][y].r = r;
            image->pixels[x][y].g = g;
            image->pixels[x][y].b = b;
        }
    }
}
