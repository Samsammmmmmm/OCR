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

double get_color(Uint32 pixel_color, SDL_PixelFormat* format)
{
    SDL_Color rgb;
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &rgb.r, &rgb.g, &rgb.b);
    int color;
    if(rgb.r == 255)
        color = 1.0;
    else
        color = 0.0;
    return color;
}

double* to_array(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    double *array = malloc((len + 1) * sizeof(double));
    SDL_PixelFormat *format = surface->format;
    if (SDL_LockSurface(surface) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    else
    {
        int cpt = 0;
        for (int i = 0; i < len; i++)
        {
            int color = get_color(pixels[i], format);
            //int color = pixels[i];
            array[i] = color;
            if (color == 1.0)
                cpt++;
        }
        SDL_UnlockSurface(surface);
        printf("%d\n", cpt);
        return array;
    }
}

int is_white(SDL_Surface* surface)
{
    return 0;
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
            //sprintf(path, "../Grid/%zu.bmp", cpt + 1);
            sprintf(path, "../Grid/1.bmp");
            if (j == 3 || j == 6)
                fprintf(to_solve, " ");
            if (is_white(SDL_LoadBMP(path)))
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
    network _neural = load("Weights/weights_12102022_193553.txt");
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
            p[j] = temp == 255 ? 1 : 0;
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

void train(int cpt)
{
    network _neural;
    network *_network = &_neural;
    *_network = create_network(784, 16, 9, 3);
    initialize_weights(_network);

    FILE *file = fopen("Dataset/oui.txt", "r");

    if (file == NULL)
    {
        errx(EXIT_FAILURE, "Dataset file does not exist");
    }
    
    int *desired_ouputs = malloc(cpt * sizeof(int));
    double **inputs = malloc(cpt * sizeof(double*));

    char *line = NULL;
    size_t length = 0;
    for (size_t i = 0; i < cpt; i++)
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
            p[j] = temp == 255 ? 1 : 0;
        }


        desired_ouputs[i] = ouput;
        inputs[i] = p;

        printf("Input n°:%zu\n", i + 1);

    }

    fclose(file);


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

void train2()
{
    /*
    network _neural;
    network *_network = &_neural;
    *_network = create_network(784, 16, 9, 3);
    initialize_weights(_network);
    */

    FILE *file = fopen("Dataset/non.txt", "w+");

    FILE *fp;
    char row[5000];
    char *token;
    fp = fopen("Dataset/TMNIST.csv","r");

    if (fp == NULL)
    {
        errx(EXIT_FAILURE, "Dataset file does not exist");
    }

    int *desired_ouputs = malloc(29900 * sizeof(int));
    double **inputs = malloc(29900 * sizeof(double*));

    int cpt = 0;
    while (feof(fp) != true)
    {
        fgets(row, 5000, fp);

        token = strtok(row, ",");

        double output = atof(token);
        double *p = malloc(784 * sizeof(double));

        int i = 0;
        while (token != NULL)
        {
            double temp = atof(token);
            p[i] = temp == 255 ? 1 : 0;
            token = strtok(NULL, ",");
            if (cpt != 0)
                fprintf(file, "%f\n", temp);
            i++;
        }

        if (cpt != 0)
        {
            desired_ouputs[cpt - 1] = output;
            inputs[cpt - 1] = p;
        }

        cpt++;
    }

    fclose(fp);
    fclose(file);
    /*
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

    free(desired_ouputs);
    free(inputs);
    network_free(_network);
     */
    train(cpt - 1);
}


int main(int argc, char** argv){
    if (argc == 1)
    {
        train2();
    }
    else if (argc == 2)
    {
        test();
    }
    else if (argc == 3)
    {
        if (*argv[1] == '1')
        {
            train(29900);
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