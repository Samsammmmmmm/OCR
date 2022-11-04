#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "traitement.h"

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");
    //printf("1\n");

    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    //printf("2\n");

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Dynamic Fractal Canopy", 0, 0, 640,
                                          400,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    //printf("3\n");

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    //printf("4\n");
    // - Create a surface from the colored image.
    SDL_Surface* surface = load_image(argv[1]);
    if (surface == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    //printf("5\n");
    save_image(surface, "ref.png");
    SDL_FreeSurface(surface);
    // Create a texture from the colored surface.
    SDL_Texture* texture = IMG_LoadTexture(renderer, argv[1]);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    //printf("6\n");
    // - Convert the surface into grayscale.
    Image *img= surface_to_image(surface);
    filter_grayscale(&img);
    Surface result= image_to_surface(&img);
    // - Free the surface.
    save_image(result, "grayscale.png");
    SDL_FreeSurface(result);

    //printf("10\n");
    // - Dispatch the events.
    print_pic(renderer, result);
    //printf("11\n");
    // - Destroy the objects.
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(result);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    //printf("12\n");
    return EXIT_SUCCESS;
}
