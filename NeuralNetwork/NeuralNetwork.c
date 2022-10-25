#include "NeuralNetwork.h"

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
    for (int i = 1; i < number_layers - 1; i++)
    {
        if (i == 1)
            //input layer to hidden layer
        {
            _network.layers[i] = create_layer(size_hidden, size_input);
        }
        else
            //hidden layer to hidden layer
        {
            _network.layers[i] = create_layer(size_hidden, size_hidden);
        }
    }

    //ouput layer
    _network.layers[number_layers - 1] = create_layer(size_output, size_hidden);
    
    return _network;
}

void initialize_weights(network *_network)
{
    for (int layers_nb = 0; layers_nb < _network->number_layers; layers_nb++)
    {
        for (int neurons_nb = 0; neurons_nb < 
            _network->layers[layers_nb].number_neurons; neurons_nb++)
        {
            for (int weights_nb = 0; weights_nb < _network->layers[layers_nb].
                neurons[neurons_nb].number_weights; weights_nb++)
            {
                _network->layers[layers_nb].neurons[neurons_nb].
                    weights[weights_nb] = 
                    ((double)rand() / (double)RAND_MAX);
            }

            _network->layers[layers_nb].neurons[neurons_nb].bias = 
                ((double)rand() / (double)RAND_MAX);
        }
    }
}

void set_inputs(network *_network, int inputs[])
{
    //initialize the first layer with inputs
    for (int neurons_nb = 0; neurons_nb < _network->layers[0].number_neurons;  
        neurons_nb++)
    {
        _network->layers[0].neurons[neurons_nb].activation = inputs[neurons_nb];
    }
}

void forward_prop(network *_network, int inputs[])
{
    set_inputs(_network, inputs);

    double total = 0;
    for (int layers_nb = 1; layers_nb < _network->number_layers; layers_nb++)
    {
        for (int neurons_nb = 0; neurons_nb < 
            _network->layers[layers_nb].number_neurons; neurons_nb++)
        {
            total = _network->layers[layers_nb].neurons[neurons_nb].bias;

            for (int prevlayerNeurons_nb = 0; prevlayerNeurons_nb <
                _network->layers[layers_nb - 1].number_neurons; 
                prevlayerNeurons_nb++)
            {
                //multiplies the activation of the previous neuron by 
                //the weight between the previous neuron and the current neuron
                total += _network->layers[layers_nb - 1].
                    neurons[prevlayerNeurons_nb].activation * 
                    _network->layers[layers_nb].neurons[neurons_nb].
                    weights[prevlayerNeurons_nb];
            }
            //Relu for hidden layers
            if (layers_nb < _network->number_layers - 1)
            {
                if (total > 0)
                {
                    _network->layers[layers_nb].neurons[neurons_nb].activation =
                        total;
                }
                else
                {
                    _network->layers[layers_nb].neurons[neurons_nb].activation =
                        0;
                }
            }
            else
            {
                _network->layers[layers_nb].neurons[neurons_nb].activation = 
                    total;
            }
        }
    }
    
    //Softmax for output layer
    layer outputLayer = _network->layers[_network->number_layers - 1];
    total = 0;
    for (int neurons_nb = 0; neurons_nb < outputLayer.number_neurons; 
        neurons_nb++)
    {
        double value = exp(outputLayer.neurons[neurons_nb].activation);
        outputLayer.neurons[neurons_nb].activation = value;
        total += value;
    }
    for (int neurons_nb = 0; neurons_nb < outputLayer.number_neurons; 
        neurons_nb++)
    {
        outputLayer.neurons[neurons_nb].activation /= total;
    }
}

void back_prop(network *_network, int k)
{
    
}