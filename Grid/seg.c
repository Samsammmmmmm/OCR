#include "seg.h"

void init_sdl()
{
	// Init only the video part.
	// If it fails, die with an error message.
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
		errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image2(char *path)
{
	SDL_Surface *img;

	// Load an image using SDL_image with format detection.
	// If it fails, die with an error message.
	img = IMG_Load(path);
	if (!img)
		errx(3, "can't load %s: %s", path, IMG_GetError());

	return img;
}

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
	//recupere les pixels
	int bpp = surf->format->BytesPerPixel;
	return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel2(SDL_Surface *surface, unsigned x, unsigned y)
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

void put_pixel2(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
	Uint8 *p = pixel_ref(surface, x, y);

	switch(surface->format->BytesPerPixel)
	{
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
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

SDL_Surface* block_resize(SDL_Surface *block, int pix_w, int pix_h)
{
  // Create a new surface with the desired dimensions
  SDL_Surface* wanted_surf_dim = SDL_CreateRGBSurface(0, pix_w, pix_h, 32, 0, 0, 0, 0);
  
  // Copy the pixels from the original surface and scale them to the new size
  SDL_BlitScaled(block, NULL, wanted_surf_dim, NULL);

  return wanted_surf_dim;
}

void save_resize(SDL_Surface *block, int pix_w, int pix_h)
{
    IMG_SavePNG(block_resize(block, pix_w, pix_h), "BMP/detection_resize.png");
}

void setlines(SDL_Surface *image, int *x, int *y)
{
	Uint32 pixel = SDL_MapRGB(image->format, 255, 0, 0);
	for(int i = 0; i < 10; i++)
	{
		int xx = y[i];
		for(int j = 0; j < image->w;j++)
		{
		  for(int k = i; k<i+3 && k < image->w; k++)
		    {put_pixel2(image,j,xx+k,pixel);}
		}
	}
	for(int i = 0; i < 10; i++)
	{
		int xx = x[i];
		for(int j = 0; j < image->h;j++)
		{
		  for(int l = i; l<i+3 && l < image->h; l++)
		    {put_pixel2(image,xx+l,j,pixel);}
		}
	}
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void getlines(SDL_Surface *image, int *x, int *y)
{
	int width = image->w;
	int height = image->h;
	int *linesx = malloc(height * sizeof(int));
	int *linesy = malloc(width * sizeof(int));
	int nbpixelx = 0;
	int nbpixely = 0;

	for(int y = 0; y < height ; y++) //x axis
	{
		for(int x = 0; x < width ; x++)
		{
			Uint32 pixel = get_pixel2(image,x,y);
			Uint8 r,g,b;
			SDL_GetRGB(pixel, image->format, &(r), &(g), &(b));
			if((r<150)&(g<150)&(b<150))
				nbpixelx += 1;
		}
		linesx[y] = nbpixelx;
		nbpixelx = 0;
	}

	for(int x = 0; x < width; x++) // y axis
	{
		for(int y = 0; y < height; y++)
		{
			Uint32 pixel = get_pixel2(image,x,y);
			Uint8 r,g,b;
			SDL_GetRGB(pixel, image->format, &(r), &(g), &(b));
			if((r<150)&(g<150)&(b<150))
				nbpixely += 1;
		}
		linesy[x] = nbpixely;
		nbpixely = 0;
	}

	for(int i = 0; i < 10;i++)
	{
		int ind = 0;
		int max = 0;
		for(int x = 0; x < height; x++)
		{
			if(linesx[x] > max)
			{
				max = linesx[x];
				ind = x;
			}
		}
		for(int j = ind - 15; j < ind + 15; j++)
		{
			if(j > -1 && j < height)
				linesx[j] = 0;
		}
		y[i] = ind;
	}


	for(int i = 0; i < 10;i++)
	{
		int ind = 0;
		int max = 0;
		for(int y = 0; y < width; y++)
		{
			if(linesy[y] > max)
			{
				max = linesy[y];
				ind = y;
			}
		}
		for(int j = ind - 15; j < ind + 15; j++)
		{
			if(j > - 1 && j < width)
				linesy[j] = 0;
		}
		x[i] = ind;
	}

	free(linesx);
	free(linesy);
}

int compare_function(const void *a,const void *b) 
{
	int *x = (int *) a;
	int *y = (int *) b;
	return *x - *y;
}

void segmentation(SDL_Surface *image, SDL_Surface* image2)
{
	int *x = malloc(10 * sizeof(int));
	int *y = malloc(10 * sizeof(int));

	getlines(image,x,y);

	qsort(x,10,sizeof(int),compare_function);
	qsort(y,10,sizeof(int),compare_function);
	//setlines(image2,x,y);
	/*char buffer[100];
	  snprintf(buffer, sizeof(buffer), "../BMP/test.bmp");
	if (SDL_SaveBMP(image2, buffer) != 0)
	  {
	    printf("SDL_SaveBMP failed: %s\n", SDL_GetError());

	    }*/
	//IMG_SavePNG(image2, "BMP/detection.png");
	int cpt = 1;
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			SDL_Rect rectangle;
			rectangle.x = x[j]+10;
			rectangle.y = y[i]+10;
			rectangle.w = x[j+1] - (x[j]);
			rectangle.h = y[i+1] - (y[i]);

			SDL_Surface *croped =
			  SDL_CreateRGBSurface(0,
					       rectangle.w-20,
					       rectangle.h-20, 32, 0, 0, 0, 0);
			SDL_BlitSurface(image2,&rectangle,croped,NULL);
			/*SDL_Surface *croped2 =
			  SDL_CreateRGBSurface(0,
					       croped->w - 20,
					       20, 32, 0, 0, 0, 0);
					       SDL_BlitSurface(image2,&rectangle,croped,NULL);*/

			char buffer[100];
			snprintf(buffer, sizeof(buffer), "BMP/%d.bmp", cpt);
			cpt++;

			if(SDL_SaveBMP(block_resize(croped, 28, 28)/*croped*/, buffer) != 0)
			{
				printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
			}

			SDL_FreeSurface(croped);
		}
    }
    setlines(image2,x,y);
    IMG_SavePNG(image2, "BMP/detection.png");
    save_resize(image2, 500, 500);
	free(x);
	free(y);
    //SDL_FreeSurface(image);
}

/*
int main(int argc, char **argv)
{
	if (argc != 3){
		errx(1,"the segmentation function takes 3 parameters");
		return EXIT_FAILURE;
	}

	init_sdl();
	
	// Creates a window.
	SDL_Window* window = SDL_CreateWindow("Hough lines", 0, 0, 640, 400,
					      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL)
	  errx(EXIT_FAILURE, "%s", SDL_GetError());
	
	// Creates a renderer.
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
	
	SDL_Surface* image_surface = load_image2(argv[1]);
	SDL_Surface* image_filtered = load_image2(argv[2]);
	
	segmentation(image_surface, image_filtered);

	

	SDL_Surface * block1 = SDL_LoadBMP("../BMP/1.bmp");
	char buffer[100];
	snprintf(buffer, sizeof(buffer), "../BMP/1_28.bmp");
	if(SDL_SaveBMP(block_resize(block1, 28, 28), buffer) != 0)
	  {
	    printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
	  }
	

	

	//SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image_surface);
	SDL_Surface * image = SDL_LoadBMP("../BMP/test.bmp");
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_Event event;
	int bool = 1;
	while (bool)
	  {
	    SDL_WaitEvent(&event);
	    
	    switch (event.type)
	      {
	      case SDL_QUIT:
		bool = 0;
		break;
	      }
	    SDL_RenderCopy(renderer, texture, NULL, NULL);
	    SDL_RenderPresent(renderer);
	  }
	
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image_surface);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return EXIT_SUCCESS;
}
*/
