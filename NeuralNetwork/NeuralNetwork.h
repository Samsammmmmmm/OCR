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

double relu(double k);

double relu_prime(double k)

network create_network(int size_input, int size_hidden, int size_output, 
    int number_layers);

void initialize_weights(network *_network);

void set_inputs(network *_network, double inputs[]);

void forward_prop(network *_network, double inputs[]);

void back_prop(network *_network, double desired_ouputs[]);

void gradient_descent(network *_network, double eta);