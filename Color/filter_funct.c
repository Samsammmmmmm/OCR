#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <pthread.h>
#include "basics.h"
#include <math.h>

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

// median filter function
void median_filter(SDL_Surface* surface, int radius)
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
            int* values = malloc(sizeof(int) * (2 * radius + 1) * (2 * radius + 1));
            for (int j = -radius; j <= radius; j++)
            {
                for (int k = -radius; k <= radius; k++)
                {
                    if (x + j >= 0 && x + j < surface->w && y + k >= 0 && y + k < surface->h)
                    {
                        Uint8 r, g, b;
                        SDL_GetRGB(pixels[(y + k) * surface->w + (x + j)], format, &r, &g, &b);
                        values[count] = 0.3 * r + 0.59 * g + 0.11 * b;
                        count++;
                    }
                }
            }
            int temp;
            for (int j = 0; j < count; j++)
            {
                for (int k = j + 1; k < count; k++)
                {
                    if (values[j] > values[k])
                    {
                        temp = values[j];
                        values[j] = values[k];
                        values[k] = temp;
                    }
                }
            }
            if (count % 2 == 0)
                pixels[i] = SDL_MapRGB(format, values[count / 2], values[count / 2], values[count / 2]);
            else
                pixels[i] = SDL_MapRGB(format, values[count / 2 + 1], values[count / 2 + 1], values[count / 2 + 1]);
            free(values);
        }
        SDL_UnlockSurface(surface);
    }
}

// gaussian smoothing function
void gaussian_smoothing(SDL_Surface* surface, int sigma, int kernel)
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
            for (int j = -kernel; j <= kernel; j++)
            {
                for (int k = -kernel; k <= kernel; k++)
                {
                    if (x + j >= 0 && x + j < surface->w && y + k >= 0 && y + k < surface->h)
                    {
                        Uint8 r1, g1, b1;
                        SDL_GetRGB(pixels[(y + k) * surface->w + (x + j)], format, &r1, &g1, &b1);
                        r += r1 * exp(-(pow(j, 2) + pow(k, 2)) / (2 * pow(sigma, 2)));
                        //no need to do it for the other colors
                        count++;
                    }
                }
            }
            pixels[i] = SDL_MapRGB(format, r / count, r / count, r / count);
        }
        SDL_UnlockSurface(surface);
    }
}

//contrast function
void contrast(SDL_Surface* surface, int contrast)
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
            double c=clamp (contrast,0,128);
            r = clamp((259 * (c + 255) / 255 * (259 - c)) * (r - 128) + 128, 0, 255);
            g = clamp((259 * (c + 255) / 255 * (259 - c)) * (g - 128) + 128, 0, 255);
            b = clamp((259 * (c + 255) / 255 * (259 - c)) * (b - 128) + 128, 0, 255);
            pixels[i] = SDL_MapRGB(format, r, g, b);
        }
        SDL_UnlockSurface(surface);
    }
}

//clamp function
int clamp(int value, int min, int max)
{
    if (value < min)
        return min;
    else if (value > max)
        return max;
    else
        return value;
}

// gamma filter function
void gamma_filter(SDL_Surface* surface, float gamma)
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
            double r1 =gamma/128;
            r = clamp(pow(r / 255, 1 / r1) *255, 0, 255);
            g = clamp(pow(g / 255, 1 / r1) *255, 0, 255);
            b = clamp(pow(b / 255, 1 / r1)*255, 0, 255);
            pixels[i] = SDL_MapRGB(format, r, g, b);
        }
        SDL_UnlockSurface(surface);
    }
}
