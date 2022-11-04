#ifndef FILTER_FUNCT.H
#define FILTER_FUNCT.H


static void pixel_to_grayscale(Pixel *pixel, int brightness);
static double gamma_filter(double p, int gamma);
static double contrast_filter(double p, int contrast);
static void pixel_to_gamma(Pixel *pixel, int gamma);
static void pixel_to_contrast(Pixel *pixel, int contrast);
void filter_grayscale(Image *image, double brigthness);
void filter gamma_filter(Image *image, double gamma);
void filter_contrast(Image *image, double contrast);

#endif
