#include "NeuralNetwork.h"

double relu(double x)
{
    if(x > 0)
    {
        return x;
    }
    return 0;
};

neuron create_neuron(int number_weights)
{
    neuron _neuron;
    _neuron.activation = 0.0;
    _neuron.bias = 0.0;
    _neuron.number_weights = number_weights;
    _neuron.weights = malloc(number_weights * sizeof(double));
    return _neuron;
};

layer create_layer(int number_neurons, int number_neurons_previous)
{
    layer _layer;
    _layer.number_neurons = number_neurons;
    _layer.neurons = malloc(number_neurons * sizeof(struct neuron));
    for (int k = 0; k < number_neurons; k++)
    {
        _layer.neurons[k] = create_neuron(number_neurons_previous);
    }
    return _layer;
};

network create_network(int size_input, int size_hidden, int size_output, 
int number_layers)
{
    network _network;
    _network.number_layers = number_layers;
    _network.size_input = size_input;
    _network.size_hidden = size_hidden;
    _network.size_output = size_output;
    _network.layers = malloc(number_layers * sizeof(struct layer));

        //input layer
    _network.layers[0] = create_layer(size_input, 0); 
        //hidden layer
    for (int i = 0; i < number_layers - 2; i++)
    {
        _network.layers[i + 1] = create_layer(size_hidden, 
            _network.layers[i - 1].number_neurons);
    }
        //ouput layer
    _network.layers[number_layers - 1] = create_layer(size_output,
        _network.layers[number_layers - 2].number_neurons);
    
    return _network;
}

void initialize_weights(network *_network)
{
    for (int i = 0; i < _network->number_layers; i++)
    {
        layer *_layer = &(_network->layers[i]);
        for (int j = 0; i < _layer->number_neurons; j++)
        {
            neuron *_neuron = &(_layer->neurons[j]);
            for (int k = 0; i < _neuron->number_weights; k++)
            {
                _neuron->weights[k] = ((double)rand() / (double)RAND_MAX);
            }
        }
    }
}