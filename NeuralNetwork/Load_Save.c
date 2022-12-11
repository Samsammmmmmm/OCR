#include "Load_Save.h"
#include <time.h>

char* save(network *_network)
{
    char filename[255];
    struct tm* tm;
    time_t now;
    now = time(0); // get current time
    tm = localtime(&now); // get structure
    sprintf(filename, "NeuralNetwork/Weights/weights_%02d%02d%04d_%02d%02d%02d.txt", tm->tm_mon+1,
            tm->tm_mday, tm->tm_year+1900, tm->tm_hour, tm->tm_min, tm->tm_sec);

    FILE *file = fopen(filename, "w+");
    
    fprintf(file, "%d\n", _network->size_input);
    fprintf(file, "%d\n", _network->size_hidden);
    fprintf(file, "%d\n", _network->size_output);
    fprintf(file, "%d\n", _network->number_layers);

    for (int layers_nb = 0; layers_nb < _network->number_layers; layers_nb++)
    {
        fprintf(file, "%d\n", _network->layers[layers_nb].number_neurons);
        for (int neurons_nb = 0; neurons_nb < _network->layers[layers_nb].
            number_neurons; neurons_nb++)
        {
            fprintf(file, "%f\n", _network->layers[layers_nb].
                neurons[neurons_nb].bias);
            fprintf(file, "%d\n", _network->layers[layers_nb].
                neurons[neurons_nb].number_weights);

            for (int weight_nb = 0; weight_nb < _network->layers[layers_nb].
                neurons[neurons_nb].number_weights; weight_nb++)
            {
                fprintf(file, "%f\n", _network->layers[layers_nb].
                neurons[neurons_nb].weights[weight_nb]);
            }
        }
    }


    fclose(file);
    char* new_path = malloc(50 * sizeof(char));
    for (size_t i = 0; i < 50; i++)
    {
        new_path[i] = filename[i];
    }
    return new_path;
}

network load(char *path)
{
    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        errx(EXIT_FAILURE, "weight file does not exist");
    }
    

    network _neural;
    network *_network = &_neural;

    char *line = NULL;
    size_t length = 0;

    getline(&line, &length, file);
    line = strtok(line, "\n");
    int size_input = strtol(line, NULL, 10);
    getline(&line, &length, file);
    line = strtok(line, "\n");
    int size_hidden = strtol(line, NULL, 10);
    getline(&line, &length, file);
    line = strtok(line, "\n");
    int size_output = strtol(line, NULL, 10);
    getline(&line, &length, file);
    line = strtok(line, "\n");
    int number_layers = strtol(line, NULL, 10);

    *_network = create_network(size_input, size_hidden, size_output, 
        number_layers);

    
    for (int layers_nb = 0; layers_nb < number_layers; layers_nb++)
    {
        getline(&line, &length, file);
        int neurons_total = strtol(line, NULL, 10);
        for (int neurons_nb = 0; neurons_nb < neurons_total; neurons_nb++)
        {
            getline(&line, &length, file);
            _network->layers[layers_nb].neurons[neurons_nb].bias = 
                strtod(line, NULL);
            
            getline(&line, &length, file);
            int number_weights = strtod(line, NULL);
            _network->layers[layers_nb].neurons[neurons_nb].number_weights = 
                number_weights;
            for (int weights_nb = 0; weights_nb < number_weights; weights_nb++)
            {
                getline(&line, &length, file);
                _network->layers[layers_nb].neurons[neurons_nb].
                    weights[weights_nb] = strtod(line, NULL);
            }
            
        }
    }

    fclose(file);
    return _neural;
}

void printNetwork(network *_network)
{
    printf("size input: %d\n", _network->size_input);
    printf("size hidden: %d\n", _network->size_hidden);
    printf("size output: %d\n", _network->size_output);
    printf("number layers: %d\n", _network->number_layers);

    printf("\n");

    for (int layers_nb = 0; layers_nb < _network->number_layers; layers_nb++)
    {
        printf("layer n°%d\n",layers_nb);
        for (int neurons_nb = 0; neurons_nb < _network->layers[layers_nb].
            number_neurons; neurons_nb++)
        {
            printf("    neuron n°%d\n", neurons_nb);
            printf("        bias: %f\n", _network->layers[layers_nb].
                neurons[neurons_nb].bias);

            for (int weight_nb = 0; weight_nb < _network->layers[layers_nb].
                neurons[neurons_nb].number_weights; weight_nb++)
            {
                printf("        weight n°%d: %f\n", weight_nb, _network->
                layers[layers_nb].neurons[neurons_nb].weights[weight_nb]);
            }
        }
    }
}