#include "NeuralNetwork.h"

double relu(double x)
{
    if(x > 0)
    {
        return x;
    }
    return 0;
};

layer create_layer(int number_neurons)
{
    layer _layer;
    _layer.number_neurons = number_neurons;
    _layer.neurons = malloc(number_neurons * sizeof(struct neuron));
    return _layer;
};

neuron create_neuron(int number_weights)
{
    neuron _neuron;
    _neuron.activation = 0.0;
    _neuron.bias = 0.0;
    _neuron.weights = malloc(number_weights * sizeof(double));
    return _neuron;
}

network create_network(int size_input, int size_hidden, int size_output, 
int number_layers)
{
    network _network;
    _network.number_layers = number_layers;
    _network.size_input = size_input;
    _network.size_hidden = size_hidden;
    _network.size_output = size_output;
    _network.layers = malloc(number_layers * sizeof(struct layer));
    return _network;
}