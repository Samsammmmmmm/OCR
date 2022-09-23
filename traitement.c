#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// dowload the picture
int download(char image[])
{
    SDL_Surface* img = IMG_Load(image);
    if(!img)
    {
        printf("Erreur de chargement de l'image : %s",SDL_GetError());
        return -1;
    }
    else
    {
        printf("Image loaded\n");
        return 0;
     }
}

// print the picture
void print_pic(char image[])
{
    SDL_Event event;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Window *window = NULL;

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(500, 500, 0, &window, &renderer);
    IMG_Init(IMG_INIT_PNG);
    texture = IMG_LoadTexture(renderer, image);
    while (1)
    {
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyTexture(texture);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

//

    //take pixel image and put it in shades of grey

    //gaussian filter : smooth the picture

    //pass the picture only in black and white

