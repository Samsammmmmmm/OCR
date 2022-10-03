#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
     //load image
    SDL_Surface* image=IMG_Load(path);
    if (image == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    //create new surface
    SDL_Surface* result =SDL_ConvertSurfaceFormat (image, SDL_PIXELFORMAT_RGB888, 0);
    if (result == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    //free temporary surface
    SDL_FreeSurface(image);
    //return desired image
    return result;
}

// print the picture
void print_pic(SDL_Renderer* renderer,SDL_Texture* texture )
{
    SDL_Event event;

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;

            // If the window is resized, updates and redraws the diagonals.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    SDL_RenderCopy(renderer, texture, NULL, NULL);
                    SDL_RenderPresent(renderer);
                }
                break;

        }
    }
}

// Converts a colored pixel into grayscale tyhen in black and white.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;
    if(average<130)
        average=250;
    else
        average=0;
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

