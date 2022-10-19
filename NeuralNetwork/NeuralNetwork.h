#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct neuron
{
    double activation;
    double bias;
    int number_weights;
    double *weights;
} neuron;

typedef struct layer
{
    int number_neurons;
    struct neuron *neurons;
} layer;

typedef struct network
{
    int size_input;
    int size_hidden;
    int size_output;
    int number_layers;
    struct layer *layers;
} network;

double relu(double x);

layer create_layer(int number_neurons, int number_neurons_previous);

neuron create_neuron(int number_weights);

network create_network(int size_input, int size_hidden, int size_output, 
    int number_layers);

void initialize_weights(network *_network)
