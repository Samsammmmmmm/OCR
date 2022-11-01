#ifndef TRAITEMENT_H
#define TRAITEMENT_H

SDL_Surface* load_image(const char* path);
void print_pic(SDL_Renderer* renderer,SDL_Texture* texture );
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
void surface_to_grayscale(SDL_Surface* surface);
void surface_to_gamma(SDL_Surface* surface);
void surface_to_contrast(SDL_Surface* surface);


#endif
