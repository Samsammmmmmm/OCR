#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <pthread.h>
#include "basics.h"
#include <maths.h>

// Converts a colored pixel into grayscale tyhen in black and white.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
}

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    if (SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    else
    {
        for (int i = 0; i < len; i++)
        {
            pixels[i] = pixel_to_grayscale(pixels[i], format);
        }
        SDL_UnlockSurface(surface);
    }
}

//apply otsu tresholding to a surface
void otsu_tresholding(SDL_Surface* surface)
{
    int* histogram = surface_to_histogram(surface);
    int total = surface->w * surface->h;
    float sum = 0;
    for (int i = 0; i < 256; i++)
        sum += i * histogram[i];
    float sumB = 0;
    int wB = 0;
    int wF = 0;
    float varMax = 0;
    int threshold = 0;
    for (int i = 0; i < 256; i++)
    {
        wB += histogram[i];
        if (wB == 0)
            continue;
        wF = total - wB;
        if (wF == 0)
            break;
        sumB += i * histogram[i];
        float mB = sumB / wB;
        float mF = (sum - sumB) / wF;
        float varBetween = wB * wF * (mB - mF) * (mB - mF);
        if (varBetween > varMax)
        {
            varMax = varBetween;
            threshold = i;
        }
    }
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    if (SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    else
    {
        for (int i = 0; i < len; i++)
        {
            Uint8 r, g, b;
            SDL_GetRGB(pixels[i], format, &r, &g, &b);
            Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;
            if (average > threshold)
                pixels[i] = SDL_MapRGB(format, 0, 0, 0);
            else
                pixels[i] = SDL_MapRGB(format, 255, 255, 255);
        }
        SDL_UnlockSurface(surface);
    }
}


//apply sauvola tresholding to a surface
void sauvola_tresholding(SDL_Surface* surface, int radius, float k)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    if (SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    else
    {
        for (int i = 0; i < len; i++)
        {
            int x = i % surface->w;
            int y = i / surface->w;
            int sum = 0;
            int count = 0;
            for (int j = -radius; j <= radius; j++)
            {
                for (int k = -radius; k <= radius; k++)
                {
                    if (x + j >= 0 && x + j < surface->w && y + k >= 0 && y + k < surface->h)
                    {
                        Uint8 r, g, b;
                        SDL_GetRGB(pixels[(y + k) * surface->w + (x + j)], format, &r, &g, &b);
                        sum += 0.3 * r + 0.59 * g + 0.11 * b;
                        count++;
                    }
                }
            }
            Uint8 average = sum / count;
            sum = 0;
            for (int j = -radius; j <= radius; j++)
            {
                for (int k = -radius; k <= radius; k++)
                {
                    if (x + j >= 0 && x + j < surface->w && y + k >= 0 && y + k < surface->h)
                    {
                        Uint8 r, g, b;
                        SDL_GetRGB(pixels[(y + k) * surface->w + (x + j)], format, &r, &g, &b);
                        sum += pow(0.3 * r + 0.59 * g + 0.11 * b - average, 2);
                    }
                }
            }
            float variance = sum / count;
            float treshold = average * (1 + k * ((variance / 128) - 1));
            if (average > treshold)
                pixels[i] = SDL_MapRGB(format, 0, 0, 0);
            else
                pixels[i] = SDL_MapRGB(format, 255, 255, 255);
        }
        SDL_UnlockSurface(surface);
    }
}

//median filter function
void filter_median(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    if (SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    else
    {
        for (int i = 0; i < len; i++)
        {
            int x = i % surface->w;
            int y = i / surface->w;
            int r = 0;
            int g = 0;
            int b = 0;
            int count = 0;
            for (int j = -1; j <= 1; j++)
            {
                for (int k = -1; k <= 1; k++)
                {
                    if (x + j >= 0 && x + j < surface->w && y + k >= 0 && y + k < surface->h)
                    {
                        Uint8 r1, g1, b1;
                        SDL_GetRGB(pixels[(y + k) * surface->w + (x + j)], format, &r1, &g1, &b1);
                        r += r1;
                        g += g1;
                        b += b1;
                        count++;
                    }
                }
            }
            pixels[i] = SDL_MapRGB(format, r / count, g / count, b / count);
        }
        SDL_UnlockSurface(surface);
    }
}


// contrast filter function
void filter_contrast(SDL_Surface* surface, int contrast)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    if (SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    else
    {
        for (int i = 0; i < len; i++)
        {
            Uint8 r, g, b;
            SDL_GetRGB(pixels[i], format, &r, &g, &b);
            r = (r - 128) * contrast / 100 + 128;
            g = (g - 128) * contrast / 100 + 128;
            b = (b - 128) * contrast / 100 + 128;
            pixels[i] = SDL_MapRGB(format, r, g, b);
        }
        SDL_UnlockSurface(surface);
    }
}
