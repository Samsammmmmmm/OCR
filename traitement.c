#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//to dowload the picture, put it in black and smooth it
int main(void)
{
    SDL_Surface* image = IMG_Load("image.png");
    if(!image)
    {
        printf("Erreur de chargement de l'image : %s",SDL_GetError());
        return -1;
    }

    //take pixel image and put it in shades of grey
    //voir si ca marche
    SDL_UnlockSurface(image);
    Uint32 pixels;
    size_t i, j;
    image = SDL_CreateRGBSurfaceWithFormat(0, 255, 255, 32, SDL_PIXELFORMAT_RGBA8888);
    image = SDL_CreateRGBSurface(0, 200, 200, 32, format->Rmask,
                                   format->Gmask, format->Bmask, format->Amask);
    SDL_LockSurface(image);
    pixels = image->pixels;
    for(i = 0; i < 255; i++)
    {
        for(j = 0; j < 255; j++)
            pixels[i 255 + j] = SDL_MapRGBA(image->format, 0, 0, (Uint8)i, 255);
    }

    //gaussian filter : smooth the picture

    //pass the picture only in black and white
}
