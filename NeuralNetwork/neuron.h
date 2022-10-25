#pragma once
#include <stdlib.h>

typedef struct neuron
{
    double activation;
    double bias;
    int number_weights;
    double *weights;
} neuron;

neuron create_neuron(int number_weights);