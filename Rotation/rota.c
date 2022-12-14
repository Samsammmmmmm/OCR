#include "rota.h"

#define PI M_PI
/*
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


// - Save the surface into a PNG file.
void save_image(SDL_Surface *image, char *path)
{
    int saved = IMG_SavePNG(image, path);
    if (saved != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
}


// - get the pixel color at (x, y)
Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
    Uint8 bytePerPixel = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bytePerPixel;

    switch(bytePerPixel){
    case 1:
        return *p;
    case 2:
        return *(Uint16 *)p;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32 *)p;
    default:
        return 0;
    }
}


// - set the pixel color at (x, y)
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch (bpp)
    {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(Uint16 *)p = pixel;
        break;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
*/
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

int IsPixelInImage(SDL_Surface *surface, int x, int y){
    return 0 <= x && x < surface->w && 0 <= y && y < surface->h;
}
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.

SDL_Surface *manual_rota(SDL_Surface *surface, int angle)
{
    angle = angle % (angle > 0 ? 360 : -360);
    double radian = PI * angle / 180;
    int height = abs((int)(surface->h * cos(radian) + surface->w * sin(radian)));
    int width = abs((int)(surface->h * sin(radian) + surface->w * cos(radian)));;
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
            if(IsPixelInImage(surface, h, v))
                put_pixel(rotated, x, y, get_pixel(surface, h, v));
            else
                put_pixel(rotated, x, y, 0x00000000);
        }
    }
    save_image(rotated,"rotated.png");
    return rotated;
}

int main2(int argc, char** argv)
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

