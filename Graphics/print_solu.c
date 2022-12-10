#include "print_solu.h"

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

void image_result(SDL_Surface* grid, char file01[], char file02[])
{
    FILE *file1 = fopen(file01, "r");
    if (file1 == NULL)
    {
        printf("Grid not found !");
        return;
    }
    char grid_to_solve[200] = "";
    int i = 0;
    int c;
    if (file1)
    {
        while ((c = getc(file1)) != EOF)
        {
            if (c != '\n' && c != ' ')
            {
                grid_to_solve[i] = c;
                i++;
            }
        }
        fclose(file1);
    }
    FILE *file2 = fopen(file02, "r");
    if (file2 == NULL)
    {
        printf("Result grid not found!");
        return;
    }
    char grid_solved[200] = "";
    i = 0;
    int c2;
    if (file2)
    {
        while ((c2 = getc(file2)) != EOF)
        {
            if (c2 != '\n' && c2 != (char) 32)
            {
                grid_solved[i] = c2;
                i++;
            }
        }
        fclose(file2);
    }
    int array1[9] = {6+7,60+7,116+7,173+7,227+7,282+7,340+7,393+7,448+7};
    int array2[9] = {6+7,59+7,116+7,172+7,226+7,281+7,339+7,393+7,447+7};
    int xbis = 0;
    int ybis = 0;
    for (int y = 0 ; y < 9 ; y ++)
    {
        for (int x = 0 ; x < 9 ; x ++)
        {
            SDL_Surface* Case;
            switch(grid_solved[ybis])
            {
            case ('1'):
                if (grid_to_solve[xbis] == '.')
                    Case = load_image("number_1_orange.png");
                else
                    Case = load_image("number_1_black.png");
                break;
            case ('2'):
                if (grid_to_solve[xbis] == '.')
                    Case =load_image("number_2_orange.png");
                else
                    Case =load_image("number_2_black.png");
                break;
            case ('3'):
                if (grid_to_solve[xbis] == '.')
                    Case =  load_image("number_3_orange.png");
                else
                    Case =  load_image("number_3_black.png");
                break;
            case ('4'):
                if (grid_to_solve[xbis] == '.')
                    Case =  load_image("number_4_orange.png");
                else
                    Case =  load_image("number_4_black.png");
                break;
            case ('5'):
                if (grid_to_solve[xbis] == '.')
                    Case =  load_image("number_5_orange.png");
                else
                    Case =  load_image("number_5_black.png");
                break;
            case ('6'):
                if (grid_to_solve[xbis] == '.')
                    Case = load_image("number_6_orange.png");
                else
                    Case = load_image("number_6_black.png");
                break;
            case ('7'):
                if (grid_to_solve[xbis] == '.')
                    Case = load_image("number_7_orange.png");
                else
                    Case = load_image("number_7_black.png");
                break;
            case ('8'):
                if (grid_to_solve[xbis] == '.')
                    Case =  load_image("number_8_orange.png");
                else
                    Case =  load_image("number_8_black.png");
                break;
            default:
                if (grid_to_solve[xbis] == '.')
                    Case =  load_image("number_9_orange.png");
                else
                    Case = load_image("number_9_black.png");
                break;
            }
            for (int x2 = 0 ; x2 < Case->w ; x2 ++)
            {
                for (int y2 = 0 ; y2 < Case->h ; y2 ++)
                {
                    Uint8 r, g, b;
                    Uint32 pixel = get_pixel(Case,x2,y2);
                    SDL_GetRGB(pixel, Case->format, &r, &g, &b);
                    if(!(r==0 && g==0 && b==0))
                        put_pixel(grid,array1[x] + x2,array2[y] + y2,pixel);
                }
            }
            SDL_FreeSurface(Case);
            xbis++;
            ybis++;
        }
    }
}

void image_result(char file01[], char file02[])
{
    SDL_Surface* load=load_image("empty_grid.png");
    image_result(load, file01, file02);
    save_image(load, "load.png");
}
