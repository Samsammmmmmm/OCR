#include "layer.h"

layer create_layer(int number_neurons, int number_neurons_previousLayer)
{
    layer _layer;
    _layer.number_neurons = number_neurons;
    _layer.neurons = malloc(number_neurons * sizeof(struct neuron));
    for (int k = 0; k < number_neurons; k++)
    {
        _layer.neurons[k] = create_neuron(number_neurons_previousLayer);
    }
    return _layer;
}