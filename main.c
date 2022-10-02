#include <stdio.h>
#include "traitement.h"

int main(int argc, char ** argv)
{
    int main(int argc, char** argv)
    {
        // Checks the number of arguments.
        if (argc != 2)
            errx(EXIT_FAILURE, "Usage: image-file");

        // Initializes the SDL.
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        // Creates a renderer.
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        // - Create a surface from the colored image.
        SDL_Surface* surface = load_image(argv[1]);
        if (surface == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        // Create a texture from the colored surface.
        SDL_Texture* texture = IMG_LoadTexture(renderer, argv[1]);
        if (texture == NULL)
            errx(EXIT_FAILURE, "%s", SDL_GetError());

        // - Convert the surface into grayscale.
        surface_to_grayscale(surface);

        // - Convert the surface into black and white only
        surface_to_white(surface);

        // - Create a new texture from the grayscale surface.
        SDL_Texture* result = SDL_CreateTextureFromSurface(renderer, surface);

        // - Free the surface.
        SDL_FreeSurface(surface);

        // - Dispatch the events.
        event_loop(renderer, result);

        // - Destroy the objects.
        SDL_DestroyTexture(texture);
        SDL_DestroyTexture(result);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();

        return EXIT_SUCCESS;
    }

}
