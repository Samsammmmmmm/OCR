#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* image = IMG_Load(path);
    if (image == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_Surface result = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGB888, 0);
    if (result == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_FreeSurface(texture);
    return result;
}

// print the picture
void print_pic(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_Event event;
    SDL_Window *window = NULL;

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(500, 500, 0, &window, &renderer);
    IMG_Init(IMG_INIT_PNG);
    texture = IMG_LoadTexture(renderer, texture);
    while (1)
    {
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    IMG_Quit();
}

// Converts a colored pixel into grayscale.
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



    //gaussian filter
    //add min brighness


    //pass the picture only in black and white (after grayscale)
Uint32 black_and_white(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average=1;
    if (r<20)
    {
        average = 0;
    }
    Uint32 color = SDL_MapRGB(format, average, average, average);
    return color;
}

void surface_to_white(SDL_Surface* surface)
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
            pixels[i] = black_and_white(pixels[i], format);
        }
        SDL_UnlockSurface(surface);
    }
}

