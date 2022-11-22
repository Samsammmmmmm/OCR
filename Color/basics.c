#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "basics.h"
#include <limits.h>

SDL_Surface* load_image(const char* path)
{
    //load image
    SDL_Surface* image=IMG_Load(path);
    if (image == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    //create new surface
    SDL_Surface* result =SDL_ConvertSurfaceFormat (image, SDL_PIXELFORMAT_RGB888, 0);
    if (result == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    //free temporary surface
    SDL_FreeSurface(image);
    //return desired image
    return result;
}


// - Save the surface into a PNG file.
void save_image(SDL_Surface *image, char *path)
{
    int saved = IMG_SavePNG(image, path);
    if (saved != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
}


// - get the pixel color at (x, y)
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


// - set the pixel color at (x, y)
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


//create an histogram of the image
int *surface_to_histogram(SDL_Surface *surface)
{
    int *histogram = calloc(256, sizeof(int));
    if (histogram == NULL)
        errx(EXIT_FAILURE, "calloc failed");
    Uint32 pixel;
    Uint8 r, g, b;
    for (int x = 0; x < surface->w; x++)
    {
        for (int y = 0; y < surface->h; y++)
        {
            pixel = get_pixel(surface, x, y);
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            histogram[r]++;
        }
    }
    return histogram;
}

//print the image
void print_pic(SDL_Renderer* renderer,SDL_Texture* texture )
{
    SDL_Event event;

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;

            // If the window is resized, updates and redraws the diagonals.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                    SDL_RenderPresent(renderer);
                }
                break;

        }
    }
}

// clamp function
int clamp(int value)
{
    if (value < 0)
        return 0;
    else if (value > 255)
        return 255;
    else
        return value;
}

int image_pixel_average(SDL_Surface *surface)
{
    int sum_red = 0;
    int sum_green = 0;
    int sum_blue = 0;

    for (int x = 0; x < surface->w; x++)
    {
        for (int y = 0; y < surface->h; y++)
        {
            uint32_t pixel = get_pixel(surface, x, y);
            uint8_t r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            sum_blue += b;
            sum_green += g;
            sum_red += r;
        }
    }

    int numberPixels = surface->h * surface->w;
    SDL_UnlockSurface(surface);
    return (sum_red / numberPixels + sum_green / numberPixels + sum_blue / numberPixels) / 3;
}

uint8_t min(uint8_t r, uint8_t g, uint8_t b)
{
    if (r < g)
        if (r < b)
            return r;
    if (g < b)
        return g;
    return b;
}
