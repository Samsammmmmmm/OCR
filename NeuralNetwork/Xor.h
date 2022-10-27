#pragma once
#include <stdio.h>
#include "NeuralNetwork.h"

void XOR(network _network, double inputs[]);

void train(network _network, int epochs);

int main();