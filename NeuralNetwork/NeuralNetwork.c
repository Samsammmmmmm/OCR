#include "NeuralNetwork.h"

double relu(double k)
{
    if (k > 0)
    {
        return k;
    }
    else
    {
        return 0;
    }
}

double relu_prime(double k)
{
    if (k > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
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

void set_inputs(network *_network, double inputs[])
{
    //initialize the first layer with inputs
    for (int neurons_nb = 0; neurons_nb < _network->layers[0].number_neurons;  
        neurons_nb++)
    {
        _network->layers[0].neurons[neurons_nb].activation = inputs[neurons_nb];
    }
}

void forward_prop(network *_network, double inputs[])
{
    set_inputs(_network, inputs);

    for (int layers_nb = 1; layers_nb < _network->number_layers; layers_nb++)
    {
        for (int neurons_nb = 0; neurons_nb < 
            _network->layers[layers_nb].number_neurons; neurons_nb++)
        {
            _network->layers[layers_nb].neurons[neurons_nb].z = 
                _network->layers[layers_nb].neurons[neurons_nb].bias;

            for (int prevlayerNeurons_nb = 0; prevlayerNeurons_nb <
                _network->layers[layers_nb - 1].number_neurons; 
                prevlayerNeurons_nb++)
            {
                //multiplies the activation of the previous neuron by 
                //the weight between the previous neuron and the current neuron
                _network->layers[layers_nb].neurons[neurons_nb].z += 
                    _network->layers[layers_nb - 1].
                    neurons[prevlayerNeurons_nb].activation * 
                    _network->layers[layers_nb].neurons[neurons_nb].
                    weights[prevlayerNeurons_nb];
            }
            //Relu for hidden layers
            if (layers_nb < _network->number_layers - 1)
            {
                _network->layers[layers_nb].neurons[neurons_nb].activation =
                    relu(_network->layers[layers_nb].neurons[neurons_nb].z);
                //if (_network->layers[layers_nb].neurons[neurons_nb].z > 0)
                //{
                //    _network->layers[layers_nb].neurons[neurons_nb].activation =
                //        _network->layers[layers_nb].neurons[neurons_nb].z;
                //}
                //else
                //{
                //    _network->layers[layers_nb].neurons[neurons_nb].activation =
                //        0;
                //}
            }
            //else
            //{
            //    _network->layers[layers_nb].neurons[neurons_nb].activation = 
            //        _network->layers[layers_nb].neurons[neurons_nb].z;
            //}
        }
    }
    
    //Softmax for output layer
    layer outputLayer = _network->layers[_network->number_layers - 1];
    double total = 0;
    for (int neurons_nb = 0; neurons_nb < outputLayer.number_neurons; 
        neurons_nb++)
    {
        double value = exp(outputLayer.neurons[neurons_nb].z);
        outputLayer.neurons[neurons_nb].activation = value;
        total += value;
    }
    for (int neurons_nb = 0; neurons_nb < outputLayer.number_neurons; 
        neurons_nb++)
    {
        outputLayer.neurons[neurons_nb].activation /= total;
    }
}

void back_prop(network *_network, double desired_ouputs[])
{
    //output layer
    for(int lastlayerNeurons_nb = 0; lastlayerNeurons_nb < 
        _network->layers[_network->number_layers - 1].number_neurons; 
        lastlayerNeurons_nb++)
    {
        _network->layers[_network->number_layers - 1].
            neurons[lastlayerNeurons_nb].error = 
            relu_prime(_network->layers[_network->number_layers - 1].
            neurons[lastlayerNeurons_nb].activation) * 
            (desired_ouputs[lastlayerNeurons_nb] - 
            _network->layers[_network->number_layers - 1].
            neurons[lastlayerNeurons_nb].activation);
    }

    //hidden layers
    for (int layers_nb = _network->number_layers - 2; layers_nb > 0;
        layers_nb--)
    {
        for (int neurons_nb = 0; neurons_nb < _network->layers[layers_nb].
            number_neurons; neurons_nb++)
        {
            double total = 0;
            for (int nextlayerNeurons_nb = 0; nextlayerNeurons_nb < _network->
                layers[layers_nb + 1].number_neurons; nextlayerNeurons_nb++)
            {
                total += _network->layers[layers_nb + 1].
                    neurons[nextlayerNeurons_nb].error * 
                    _network->layers[layers_nb + 1].
                    neurons[nextlayerNeurons_nb].weights[neurons_nb];
            }
            _network->layers[layers_nb].neurons[neurons_nb].error =
                relu_prime(_network->layers[layers_nb].neurons[neurons_nb].
                    activation) * total;
        }
    }
}

void gradient_descent(network *_network, double eta)
{
    for (int layers_nb = 1; layers_nb < _network->number_layers; layers_nb++)
    {
        for (int neurons_nb = 0; neurons_nb < _network->layers[layers_nb].
            number_neurons; neurons_nb++)
        {
            for (int prevlayerNeurons_nb = 0; prevlayerNeurons_nb < 
                _network->layers[layers_nb - 1].number_neurons; 
                prevlayerNeurons_nb++)
            {
                _network->layers[layers_nb].neurons[neurons_nb].
                    weights[prevlayerNeurons_nb] += eta *
                    _network->layers[layers_nb].neurons[neurons_nb].error *
                    _network->layers[layers_nb - 1].
                    neurons[prevlayerNeurons_nb].activation;
                _network->layers[layers_nb].neurons[neurons_nb].bias =
                    eta * 
                    _network->layers[layers_nb].neurons[neurons_nb].error *
                    _network->layers[layers_nb - 1].
                    neurons[prevlayerNeurons_nb].activation;
            }
        }
    }
}