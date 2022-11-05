#pragma once
#include <string.h>
#include <err.h>
#include "NeuralNetwork.h"

void save(network *_network);

network load(char* path);

void printNetwork(network *_network);