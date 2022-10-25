#pragma once
#include <stdlib.h>
#include "neuron.h"

typedef struct layer
{
    int number_neurons;
    struct neuron *neurons;
} layer;

layer create_layer(int number_neurons, int number_neurons_previous);