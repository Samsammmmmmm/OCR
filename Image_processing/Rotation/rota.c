#include "rota.h"

#define PI M_PI


// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture, double angle)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_Rect dstrect = { 220, 100, 200, 200};
  SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, angle, NULL, SDL_FLIP_NONE);
  SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop(SDL_Renderer* renderer, SDL_Texture* texture, double angle_para)
{
    SDL_Event event;

    double angle = angle_para;
    /*SDL_StartTextInput();
      char text[] = "";*/
    
    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // TODO
	  // If the "quit" button is pushed, ends the event loop.
	case SDL_QUIT:
	  return;

	  /*case SDLK_RETURN:
	    return;*/
	    // If the window is resized, updates and redraws the diagonals.
	case SDL_WINDOWEVENT:
	  if (event.window.event == SDL_WINDOWEVENT_RESIZED)
	    {
	      draw(renderer, texture, angle);
	    }
	  break;
	case SDL_KEYDOWN:
	  switch(event.key.keysym.sym )
	    {
	      /*case SDLK_BACKSPACE:
	      if (strlen(text) > 0)
	      text = text.substr(0, strlen(text) - 1);*/
	    case SDLK_RIGHT:
	      angle++;
	      draw(renderer, texture, angle);
	      break;
	    case SDLK_LEFT:
	      angle--;
	      draw(renderer, texture, angle);
	      break;
	    }
	  /*case SDL_TEXTINPUT:
	    strcat(text, event.text.text);*/
        }
    }
    /*angle = atof(text);
      draw(renderer, texture, angle);*/
}

// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.

SDL_Surface *manual_rota(SDL_Surface *surface, int angle)
{
    angle = angle % (angle > 0 ? 360 : -360);
    double radian = PI * angle / 180;
    int height = abs(surface->h * cos(radian) + surface->w * sin(radian));
    int width = abs(surface->h * sin(radian) + surface->w * cos(radian));;
    SDL_Surface *rotated = SDL_CreateRGBSurface(0,width,height,32,0,0,0,0);
    int axis_y = height / 2;
    int axis_x = width / 2;
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            int h = (int) (y - axis_y) * sin(radian)
                + (x - axis_x) * cos(radian) + surface->w / 2;
            int v =(int) (y - axis_y) * cos(radian)
                + (x - axis_x) * -sin(radian) + surface->h / 2;
            if(0 <= x && x < surface->w && 0 <= y && y < surface->h)
                put_pixel(rotated, x, y, get_pixel(surface, h, v));
            else
                put_pixel(rotated, x, y, 0x00000000);
        }
    }
    save_image(rotated,"rotated.png");
    return rotated;
}

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2 && argc != 3)
        errx(EXIT_FAILURE, "Usage: image-file angle-rotation");

    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("display", 0, 0, 640, 400,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* surface = load_image(argv[1]);
    if (surface == NULL)
      errx(EXIT_FAILURE, "%s", SDL_GetError());

    /*int w = surface->w;
    int h = surface->h;
    int new_dim = (int)sqrt(w*w+h*h);
    
    SDL_SetWindowSize(window, new_dim, new_dim);*/
    
    SDL_Texture* texture =  SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
      errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Dispatches the events.
    if (argc==3)
    {
      event_loop(renderer, texture, atof(argv[2]));
      manual_rota(surface,atof(argv[2]));
    }
    else
      event_loop(renderer, texture, 0);

    // Destroys the objects.;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
