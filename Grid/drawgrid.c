#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "hough.h"
// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
	//recupere les pixels
	int bpp = surf->format->BytesPerPixel;
	return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
	//set les pixels a l'aide de pixel_ref
	Uint8 *p = pixel_ref(surface, x, y);

	switch (surface->format->BytesPerPixel)
	{
		case 1:
			return *p;

		case 2:
			return *(Uint16 *)p;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;
	}

	return 0;
}

void hough(SDL_Surface* image_surface, SDL_Renderer* renderer)
{

    unsigned int width = image_surface->w;
    unsigned int height = image_surface->h;
    unsigned int maxval;
    
    unsigned int Rhos, Thetas;
    Rhos = sqrt(width * width + height * height);
    Thetas = 180;

    //initialise accumulator array
    unsigned int acc_array[Rhos][Thetas];
    
    for (size_t i = 0; i < Rhos; i++)
    {
        for (size_t j = 0; j < Thetas; j++)
            acc_array[i][j] = 0;
    }
    Uint8 r, g, b;
    Uint32 pixel;
    //Uint32* pixels = image_surface->pixels;
    /*int len = image_surface->w * image_surface->h;
    SDL_PixelFormat* format = image_surface->format;
    if (format == NULL)
      errx(EXIT_FAILURE, "%s", SDL_GetError());
    SDL_LockSurface(image_surface);
    for (int i = 0; i<len; i++)
      {
	Uint32 pixel = pixels[i];
	SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
	
	//if white
	if (r+g+b > 0)
	  {
	    //p = x*cos(t) + y*sin(t)
	    //check for every t
	    for (int t = 0; t < 180;t++)
	      {
		unsigned int p = x*cos(t*M_PI/180) + y*sin(t*M_PI/180);
		acc_array[p][t]++;
		if (acc_array[p][t]>maxval)
		  {
		    maxval = acc_array[p][t];
		  }
                }
		}
      }
      SDL_UnlockSurface(image_surface);*/
    //go through each pixels
    //int i = 0;
    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
	  pixel = get_pixel(image_surface, x, y);
	  SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
                  
            //if white
            if (r+g+b > 0)
            {
                //p = x*cos(t) + y*sin(t)
                //check for every t
                for (int t = 0; t < 180;t++)
                {
                    unsigned int p = x*cos(t*M_PI/180) + y*sin(t*M_PI/180);
                    acc_array[p][t]++;
		    if (acc_array[p][t]>maxval)
		      {
			maxval = acc_array[p][t];
		      }
                }
            }
        }
	}
    unsigned int threshline = maxval * THRESHOLD;
    for (size_t r = 0; r < Rhos; r++)
    {
      for (size_t t = 0; t < Thetas; t++)
	{
	  if (acc_array[r][t]>=threshline)
	    {
	      // x0 stores the value rcos(theta)
	      int x0 = cos(t*M_PI/180)*r;
	      
	      // y0 stores the value rsin(theta)
	      int y0 = sin(t*M_PI/180)*r;
		
	      // x1 stores the rounded off value of (rcos(theta)-1000sin(theta))
	      int x1 = x0 - 1000*(sin(t*M_PI/180));
		
		// y1 stores the rounded off value of (rsin(theta)+1000cos(theta))
	      int y1 = y0 + 1000*(cos(t*M_PI/180));
		
		// x2 stores the rounded off value of (rcos(theta)+1000sin(theta))
	      int x2 = x0 - 1000*(-(sin(t*M_PI/180)));
		
		// y2 stores the rounded off value of (rsin(theta)-1000cos(theta))
	      int y2 = y0 - 1000*(cos(t*M_PI/180));

	      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	      SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	      SDL_RenderPresent(renderer);
	    }
	}
    }
}

void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop(SDL_Renderer* renderer, SDL_Texture* normaltext, SDL_Texture* houghtext, SDL_Surface* surface)
{
    SDL_Event event;
    SDL_Texture* t = normaltext;

    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // TODO
	  // If the "quit" button is pushed, ends the event loop.
	case SDL_QUIT:
	  return;
	  
	  // If the window is resized, updates and redraws the diagonals.
	case SDL_WINDOWEVENT:
	  if (event.window.event == SDL_WINDOWEVENT_RESIZED)
	    {
	      draw(renderer, t);
	    }
	  break;
	case SDL_KEYDOWN:
	  /*if (t == normaltext)
	    {
	      t=houghtext;
	    }
	  else
	    {
	      t=normaltext;
	      }*/
	  hough(surface, renderer);
	  draw(renderer, t);
        }
    }
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
  SDL_Surface* temp = IMG_Load(path);
  if (temp == NULL)
      errx(EXIT_FAILURE, "%s", SDL_GetError());
  SDL_Surface* surface = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGB888, 0);
  if (surface == NULL)
      errx(EXIT_FAILURE, "%s", SDL_GetError());
  SDL_FreeSurface(temp);
  return surface;
}


int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("display", 0, 0, 640, 400,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* surface = load_image(argv[1]);
    if (surface == NULL)
      errx(EXIT_FAILURE, "%s", SDL_GetError());

    //SDL_SetWindowSize(window, surface->w, surface->h);
    
    SDL_Texture* texture =  SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
      errx(EXIT_FAILURE, "%s", SDL_GetError());

    // hough(surface, renderer);

    SDL_Texture* texturehough =  SDL_CreateTextureFromSurface(renderer, surface);
    if (texturehough == NULL)
      errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // Dispatches the events.
    event_loop(renderer, texture, texturehough, surface);
    SDL_FreeSurface(surface);
    // Destroys the objects.
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texturehough);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
