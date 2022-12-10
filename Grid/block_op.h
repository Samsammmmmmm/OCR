#include <stdio.h>
#include <stdlib.h>
/*#include </opt/homebrew/include/SDL2/SDL.h>
#include </opt/homebrew/Cellar/sdl2_image/2.6.2/include/SDL2/SDL_image.h>*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

void crop_borders(SDL_Surface *block);

void block_resize(SDL_Surface *block);
