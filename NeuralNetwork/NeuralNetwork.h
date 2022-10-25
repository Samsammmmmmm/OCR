#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "layer.h"

typedef struct network
{
    int size_input;
    int size_hidden;
    int size_output;
    int number_layers;
    struct layer *layers;
} network;

network create_network(int size_input, int size_hidden, int size_output, 
    int number_layers);

void initialize_weights(network *_network);

void set_inputs(network *_network, int inputs[]);

void forward_prop(network *_network, int inputs[]);

void back_prop(network *_network, int k);