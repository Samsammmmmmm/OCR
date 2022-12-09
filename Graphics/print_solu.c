#include <stdio.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface* load_image(const char* path)
{
    //load image
    SDL_Surface* image=IMG_Load(path);
    if (image == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    return image;
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
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch (bpp)
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


void resize_surfaceForNN(SDL_Surface* src,SDL_Surface* dest)
{
    int i,j,k;
    double rx,ry;
    rx = dest->w*1.0/src->w;
    ry = dest->h*1.0/src->h;
    for(i=0;i<dest->w;i++)
        for(j=0;j<dest->h;j++)
        {
            Uint32 pix;
            pix = get_pixel(src,(int)(i/rx),(int)(j/ry));
            put_pixel(dest,i,j,pix);
        }
}


void image_result(SDL_Surface* grid)
{
    int c;
    FILE *file;
    file = fopen("grid.txt", "r");
    if (file == NULL)
    {
        printf("Grid not found !");
        return;
    }
    char grid_nsolve[200] = "";
    int i = 0;
    if (file) {
        while ((c = getc(file)) != EOF)
        {
//putchar(c);
            if (c != '\n' && c != ' ')
            {
                grid_nsolve[i] = c;
                i++;
            }
        }
        fclose(file);
    }
    i = 0;
//printf("\n");
    int c2;
    FILE *file2;
    file2 = fopen("solved_grid.txt", "r");
    if (file2 == NULL)
    {
        printf("Result grid not found!");
        return;
    }
    char grid_solve[200] = "";
    i = 0;
    if (file) {
        while ((c2 = getc(file2)) != EOF)
        {
//putchar(c2);
            if (c2 != '\n' && c2 != (char) 32)
            {
                grid_solve[i] = c2;
                i++;
            }
        }
        fclose(file2);
    }
    i = 0;
    int arrayX[9] = {6+7,60+7,116+7,173+7,227+7,282+7,340+7,393+7,448+7};
    int arrayY[9] = {6+7,59+7,116+7,172+7,226+7,281+7,339+7,393+7,447+7};
    int xnSolve = 0;
    int xSolve = 0;

    for (int j = 0 ; j < 9 ; j ++)
    {
        for (int i = 0 ; i < 9 ; i ++)
        {
            SDL_Surface* Case;
            if (grid_nsolve[xnSolve] == '.')
            {
                switch(grid_solve[xSolve])
                {
                case ('1'):
                    Case = load_image("number_1_orange.png");
                    break;
                case ('2'):
                    Case =load_image("number_2_orange.png");
                    break;
                case ('3'):
                    Case =  load_image("number_3_orange.png");
                    break;
                case ('4'):
                    Case =  load_image("number_4_orange.png");
                    break;
                case ('5'):
                    Case =  load_image("number_5_orange.png");
                    break;
                case ('6'):
                    Case = load_image("number_6_orange.png");
                    break;
                case ('7'):
                    Case = load_image("number_7_orange.png");
                    break;
                case ('8'):
                    Case =  load_image("number_8_orange.png");
                    break;
                default:
                    Case =  load_image("number_9_orange.png");
                    break;
                }
            }
            else
            {
                switch(grid_solve[xSolve])
                {
                case ('1'):
                    Case = load_image("number_1_black.png");
                    break;
                case ('2'):
                    Case =load_image("number_2_black.png");
                    break;
                case ('3'):
                    Case =  load_image("number_3_black.png");
                    break;
                case ('4'):
                    Case =  load_image("number_4_black.png");
                    break;
                case ('5'):
                    Case =  load_image("number_5_black.png");
                    break;
                case ('6'):
                    Case = load_image("number_6_black.png");
                    break;
                case ('7'):
                    Case = load_image("number_7_black.png");
                    break;
                case ('8'):
                    Case =  load_image("number_8_black.png");
                    break;
                default:
                    Case = load_image("number_9_black.png");
                    break;

                }
            }

            for (int i2 = 0 ; i2 < Case->w ; i2 ++)
            {
                for (int j2 = 0 ; j2 < Case->h ; j2 ++)
                {
                    Uint32 pixel = get_pixel(Case,i2,j2);
                    put_pixel(grid,arrayX[i] + i2,arrayY[j] + j2,pixel);
                }
            }
            SDL_FreeSurface(Case);
            xnSolve++;
            xSolve++;
        }
    }

}
int main()
{
    SDL_Surface* load=load_image("empty_grid.png");
    image_result(load);
    save_image(load, "load.png");
}

