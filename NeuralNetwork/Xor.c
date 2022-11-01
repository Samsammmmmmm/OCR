#include "Xor.h"

double inputs[4][2] = {{0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}};
double desired_ouputs[4][2] = {{1.0, 0.0}, {0.0, 1.0}, {0.0, 1.0}, {1.0, 0.0}};

void XOR(int epochs)
{
    network _neural;
    network *_network = &_neural;
    *_network = create_network(2, 16, 2, 3);
    initialize_weights(_network);

    for (int epoch = 0; epoch < epochs; epoch++)
    {
        for (int input_nb = 0; input_nb < 4; input_nb++)
        {
            forward_prop(_network, inputs[input_nb]);
            back_prop(_network, desired_ouputs[input_nb]);
            gradient_descent(_network, 0.01);
        }
    }

    for (int input_nb = 0; input_nb < 4; input_nb++)
    {
        forward_prop(_network, inputs[input_nb]);
        printf("%f - %f:\n", inputs[input_nb][0], inputs[input_nb][1]);
        printf("0: %f\n", _network->layers[_network->number_layers - 1].
            neurons[0].activation);
        printf("1: %f\n", _network->layers[_network->number_layers - 1].
            neurons[1].activation);
        printf("\n");
    }
}

int main()
{
    XOR(2000000);

    return 0;
}