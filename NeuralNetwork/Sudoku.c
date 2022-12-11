#include "Sudoku.h"

double* get_desired_ouputs(int x)
{
    double *array = calloc(9, sizeof(double));
    switch (x)
    {
    case 1:
        array[0] = 1.0;
        return array;
    case 2:
        array[1] = 1.0;
        return array;
    case 3:
        array[2] = 1.0;
        return array;
    case 4:
        array[3] = 1.0;
        return array;
    case 5:
        array[4] = 1.0;
        return array;
    case 6:
        array[5] = 1.0;
        return array;
    case 7:
        array[6] = 1.0;
        return array;
    case 8:
        array[7] = 1.0;
        return array;
    default:
        array[8] = 1.0;
        return array;
    }
}


Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    Uint8 bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp){
        case 1:
            return *p;
            break;
        case 2:
            return *(Uint16 *)p;
            break;
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
int GetValueOfPixel(int x, int y, SDL_Surface *image)
{
    Uint32 pixel = getpixel(image, x, y);
    Uint8 white;
    SDL_GetRGB(pixel,image->format,&white,&white,&white);
    return white;
}

double* to_array(SDL_Surface* surface)
{
    double *array = malloc(28*28 * sizeof(double));
    int cpt = 0;
    for (int i = 0; i < 28; i++)
    {
        for (int j = 0; j < 28; j++)
        {
            int p = GetValueOfPixel(i, j, surface);
            if (p == 255)
            {
                array[i + p] = 1;
            }
            array[i + j] = 0;
        }
    }
    return array;
}

int is_black(SDL_Surface* surface)
{
    double* color = to_array(surface);
    int cpt = 0;
    for (int i = 0; i < 784; i++)
    {
        double temp = color[i];
        if (temp == 1)
            cpt++;
    }
    return cpt == 0;
}

int get_number(network *_network)
{
    double max = _network->layers[_network->number_layers - 1].neurons[0].activation;
    int max_i = 0;
    for (size_t i = 0; i < 9; i++)
    {
        if (_network->layers[_network->number_layers - 1].neurons[i].activation > max)
        {
            max = _network->layers[_network->number_layers - 1].neurons[i].activation;
            max_i = i + 1;
        }
    }
    return max_i;
}

void create_file_to_solve(char* weights_path)
{
    network _neural = load(weights_path);
    network *_network = &_neural;

    FILE *to_solve = fopen("to_solve.txt", "w");

    size_t cpt = 0;
    for (size_t i = 0; i < 9; i++)
    {
        for (size_t j = 0; j < 9; j++)
        {
            char path[255];
            sprintf(path, "BMP/%zu.bmp", cpt + 1);
            //sprintf(path, "../Grid/white.bmp");
            if (j == 3 || j == 6)
                fprintf(to_solve, " ");
            if (is_black(SDL_LoadBMP(path)))
            {
                fprintf(to_solve, ".");
            }
            else
            {
                double* inputs = to_array(SDL_LoadBMP(path));

                forward_prop(_network, inputs);
                free(inputs);
                int number = get_number(_network);
                char n = number + 48;
                fprintf(to_solve, "%c", n);
            }
            cpt++;
        }
        if (i == 2 || i == 5)
            fprintf(to_solve, "\n");
        fprintf(to_solve, "\n");
    }
    fclose(to_solve);
}

void test()
{
    network _neural = load("Weights/weights_12112022_104119.txt");
    network *_network = &_neural;


    FILE *file = fopen("Dataset/oui.txt", "r");

    if (file == NULL)
    {
        errx(EXIT_FAILURE, "Dataset file does not exist");
    }

    int *desired_ouputs = malloc(29900 * sizeof(int));
    double **inputs = malloc(29900 * sizeof(double*));

    char *line = NULL;
    size_t length = 0;
    for (size_t i = 0; i < 29900; i++)
    {
        double *p = malloc(784 * sizeof(double));

        getline(&line, &length, file);
        line = strtok(line, "\n");
        double ouput = strtol(line, NULL, 10);

        for (size_t j = 0; j < 784; j++)
        {
            getline(&line, &length, file);
            line = strtok(line, "\n");
            double temp = strtol(line, NULL, 10);
            p[j] = temp > 128 ? 1 : 0;
        }


        desired_ouputs[i] = ouput;
        inputs[i] = p;
    }

    fclose(file);

    int ok = 0;
    for (int input_nb = 0; input_nb < 29900; input_nb++)
    {
        forward_prop(_network, inputs[input_nb]);
        if (get_number(_network) == desired_ouputs[input_nb])
            ok++;
    }

    printf("%d on 29900", ok);
}

//Convert a csv file into a txt
void csv_to_txt(char* path, int* cpt)
{
    int MAXCHAR = 5000;
    FILE *fp;
    char row[MAXCHAR];
    char *token;

    fp = fopen("Dataset/TMNIST.csv","r");

    FILE *file;
    file = fopen("Dataset/convert.txt", "w+");

    int first = 0;
    while (feof(fp) != true)
    {
        fgets(row, MAXCHAR, fp);

        token = strtok(row, ",");

        while(token != NULL)
        {
            if (first)
                fprintf(file, "%f\n", atof(token));
            token = strtok(NULL, ",");
        }
        if (!first)
            first++;
        else
            *cpt = *cpt + 1;
    }
    *cpt = *cpt - 1;
    fclose(file);
    fclose(fp);
}

void train(char* path)
{
    int *lines = malloc(sizeof(int));
    *lines = 0;
    csv_to_txt(path, lines);

    //init network
    network _neural;
    network *_network = &_neural;
    *_network = create_network(784, 16, 9, 3);
    initialize_weights(_network);

    FILE *file = fopen("Dataset/convert.txt", "r");

    if (file == NULL)
    {
        errx(EXIT_FAILURE, "Dataset file does not exist");
    }
    /*
    //get number of lines
    int lines = 0;
    while(!feof(file))
    {
        int ch = fgetc(file);
        if(ch == '\n')
        {
            lines++;
        }
    }
    lines = lines / 785;
    fclose(file);

    file = fopen(path, "r"); */
    int *desired_ouputs = malloc(*lines * sizeof(int));
    double **inputs = malloc(*lines * sizeof(double*));

    char *line = NULL;
    size_t length = 0;
    for (size_t i = 0; i < *lines; i++)
    {
        double *p = malloc(784 * sizeof(double));

        getline(&line, &length, file);
        line = strtok(line, "\n");
        double ouput = strtol(line, NULL, 10);

        for (size_t j = 0; j < 784; j++)
        {
            getline(&line, &length, file);
            line = strtok(line, "\n");
            double temp = strtol(line, NULL, 10);
            p[j] = temp > 128 ? 1 : 0;
        }


        desired_ouputs[i] = ouput;
        inputs[i] = p;

        printf("Input n°:%zu\n", i + 1);

    }

    fclose(file);


    //trains the network
   for (int epoch = 0; epoch < 200; epoch++)
    {
        for (int input_nb = 0; input_nb < 29900; input_nb++)
        {
            printf("Epoch n°:%d -- Input n°:%d\n", epoch + 1, input_nb + 1);
            forward_prop(_network, inputs[input_nb]);
            back_prop(_network, get_desired_ouputs(desired_ouputs[input_nb]));
            gradient_descent(_network, 0.01);
        }
    }

    save(_network);
}

/*
int main(int argc, char** argv)
{
    if (argc == 2)
    {
        test();
    }
    else if (argc == 3)
    {
        if (*argv[1] == '1')
        {
            train("Dataset/oui.txt");
        }
        else if (*argv[1] == '0')
        {
            create_file_to_solve(argv[2]);
        }
        else
            errx(EXIT_FAILURE, "Usage:");
    }
    else
        errx(EXIT_FAILURE, "Usage:");
    return 0;
}
*/