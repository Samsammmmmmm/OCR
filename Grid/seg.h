#include <stdio.h>
#include <stdlib.h>
/*#include </opt/homebrew/include/SDL2/SDL.h>
#include </opt/homebrew/Cellar/sdl2_image/2.6.2/include/SDL2/SDL_image.h>*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

void init_sdl();

SDL_Surface* load_image(char *path);

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

void setlines(SDL_Surface *image, int *x, int *y);

void getlines(SDL_Surface *image, int *x, int *y);

int compare_function(const void *a,const void *b) ;

void segmentation(SDL_Surface *image);

int main(int argc, char **argv);