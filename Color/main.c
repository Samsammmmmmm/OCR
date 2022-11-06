#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "basics.h"
#include "filter_funct.h"

int main(int argc, char** argv)
{
     // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Dynamic Fractal Canopy", 0, 0, 640, 400,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
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

    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window, surface->w, surface->h);

<<<<<<< HEAD
    // - Convert the surface into grayscale.
    //filter_median(surface);
    //save_image(surface, "median.png");
    sauvola_tresholding(surface, 0.5, 128);
    save_image(surface, "ref.png");
=======
    // - Convert the surface into black and white
    //filter_median(surface);
    //save_image(surface, "median.png");
    //filter_contrast(surface,128);
    //save_image(surface, "contrast.png");
    sauvola_tresholding(surface);
    save_image(surface, "result.png");
>>>>>>> 7340b3e7c42286ffa36bc40b14f74f1cec2d4107

    // - Create a new texture from the grayscale surface.
    SDL_Texture* result=SDL_CreateTextureFromSurface(renderer, surface);
    
    // - Free the surface.
    SDL_FreeSurface(surface);
    
    // - Dispatch the events.
    print_pic(renderer, result);

    // - Destroy the objects.
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(result);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
