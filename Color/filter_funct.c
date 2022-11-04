#include <stdio.h>
#include <maths.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <pthread.h>
#include "basics.h"


static void pixel_to_grayscale(Pixel *pixel, int brightness)
{
    pixel->r = pixel->g = pixel->b = clamp(
        (0.3 * pixel->r + 0.59 * pixel->g + 0.11 * pixel->b) + brightness, 0,
        255);
}

static double gamma_filter(double p, int gamma)
{
    double c = gamma / 128;
    double factor = pow(p / 255.0, c);
    return clamp(factor * 255.0, 0, 255);
}

static double contrast_filter(double p, int contrast)
{
    double c = contrast / 128;
    double factor = (259 * (c + 255)) / (255 * (259 - c));
    return clamp(factor * (p - 128) + 128, 0, 255);
}


static void pixel_to_gamma(Pixel *pixel, int gamma)
{
    pixel->r = gamma_filter(pixel->r, gamma);
    pixel->g = gamma_filter(pixel->g, gamma);
    pixel->b = gamma_filter(pixel->b, gamma);
}

static void pixel_to_contrast(Pixel *pixel, int contrast)
{
    pixel->r = contrast_filter(pixel->r, contrast);
    pixel->g = contrast_filter(pixel->g, contrast);
    pixel->b = contrast_filter(pixel->b, contrast);
}

void filter_grayscale(Image *image, double brigthness)
{
    image_filter(surface, pixel_to_grayscale, brigthness);
}

void filter gamma_filter(Image *image, double gamma)
{
    image_filter(surface, pixel_to_gamma, gamma);
}

void filter_contrast(Image *image, double contrast)
{
    image_filter(surface, pixel_to_contrast, contrast);
}


