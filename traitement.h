#ifndef TRAITEMENT_H
#define TRAITEMENT_H

SDL_Surface* load_image(const char* path);
void print_pic(char image[]);
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
void surface_to_grayscale(SDL_Surface* surface)
Uint32 black_and_white(Uint32 pixel_color, SDL_PixelFormat* format);
void surface_to_white(SDL_Surface* surface);
#endif
