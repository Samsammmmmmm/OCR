#include <err.h>
/*#include </opt/homebrew/include/SDL2/SDL.h>
  #include </opt/homebrew/Cellar/sdl2_image/2.6.2/include/SDL2/SDL_image.h>*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void draw(SDL_Renderer* renderer, SDL_Texture* texture, double angle);

void event_loop(SDL_Renderer* renderer, SDL_Texture* texture, double angle_para);

SDL_Surface* load_image(const char* path);

int main(int argc, char** argv);
