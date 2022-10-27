#include "neuron.h"

neuron create_neuron(int number_weights)
{
    neuron _neuron;
    _neuron.activation = 0.0;
    _neuron.bias = 0.0;
    _neuron.z = 0.0;
    _neuron.error = 0.0;
    _neuron.number_weights = number_weights;
    _neuron.weights = (double*) malloc(number_weights * sizeof(double));
    return _neuron;
}