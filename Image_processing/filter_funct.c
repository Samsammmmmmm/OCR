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

void median_filter(SDL_Surface *surface)
{
    int width = surface->w;
    int height = surface->h;

    Uint32 window[25];

    for (int x = 1; x < width - 1; x++)
    {
        for (int y = 1; y < height - 1; y++)
        {
            window[0] = get_pixel(surface, x - 1, y - 1);
            window[1] = get_pixel(surface, x, y - 1);
            window[2] = get_pixel(surface, x + 1, y - 1);
            window[3] = get_pixel(surface, x - 1, y);
            window[4] = get_pixel(surface, x, y);
            window[5] = get_pixel(surface, x + 1, y);
            window[6] = get_pixel(surface, x - 1, y + 1);
            window[7] = get_pixel(surface, x, y + 1);
            window[8] = get_pixel(surface, x + 1, y + 1);

            InsertSort(window);

            put_pixel(surface, x, y, window[4]);
        }
    }
}

// median filter function
/*void median_filter(SDL_Surface* surface, int radius)
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
*/

//contrast function
void contrast(SDL_Surface* surface)
{
    int c_value = image_pixel_average(surface);
    int av = image_pixel_average(surface);

    for (int x = 0; x < surface->w; x++)
    {
        for (int y = 0; y < surface->h; y++)
        {
            Uint8 r, g, b;
            Uint32 pixel = get_pixel(surface, x, y);
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);

            double factor = (259 * (c_value + 255)) / (255.0* (259.0 - c_value));

            int new_r = clamp(factor * (r - av) + av);
            int new_g = clamp(factor * (g - av) + av);
            int new_b = clamp(factor * (b - av) + av);
            int min = min_color(new_r, new_g, new_b);

            pixel = SDL_MapRGB(surface->format, min, min, min);
            put_pixel(surface, x, y, pixel);
        }
    }
    SDL_UnlockSurface(surface);
}

// gamma filter function
void gamma_filter(SDL_Surface* surface)
{
    float gamma = 2.0f / (float) (255 - image_pixel_average(surface));

    for (int x = 0; x < surface->w; x++)
    {
        for (int y = 0; y < surface->h; y++)
        {
            uint32_t pixel = get_pixel(surface, x, y);
            uint8_t r, g, b;
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            int new_r = clamp(pow((float) (r / 255.0), gamma) * 255.0f);
            int new_g = clamp(pow((float) (g / 255.0), gamma) * 255.0f);
            int new_b = clamp(pow((float) (b / 255.0), gamma) * 255.0f);

            pixel = SDL_MapRGB(surface->format, new_r, new_g, new_b);
            put_pixel(surface, x, y, pixel);
        }
    }
    SDL_UnlockSurface(surface);
}

//sobel filter function
void sobel_filter(SDL_Surface* surface)
{
    int kernel_x[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
    int kernel_y[3][3] = { { -1, -2, -1 }, { 0, 0, 0 }, { 1, 2, 1 } };

    for (int x = 0; x < surface->w; x++)
    {
        for (int y = 0; y < surface->h; y++)
        {
            int sum_x = 0;
            int sum_y = 0;
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    if (x + i >= 0 && x + i < surface->w && y + j >= 0 && y + j < surface->h)
                    {
                        Uint8 r, g, b;
                        SDL_GetRGB(get_pixel(surface, x + i, y + j), surface->format, &r, &g, &b);
                        sum_x += r * kernel_x[i + 1][j + 1];
                        sum_y += r * kernel_y[i + 1][j + 1];
                    }
                }
            }
            int new_r = clamp(sqrt(pow(sum_x, 2) + pow(sum_y, 2)));
            int new_g = clamp(sqrt(pow(sum_x, 2) + pow(sum_y, 2)));
            int new_b = clamp(sqrt(pow(sum_x, 2) + pow(sum_y, 2)));
            int mini = min_color(new_r, new_g, new_b);

            Uint32 pixel = SDL_MapRGB(surface->format, mini, mini, mini);
            put_pixel(surface, x, y, pixel);
        }
    }
    SDL_UnlockSurface(surface);
}
