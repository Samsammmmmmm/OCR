#pragma once
#include <stdio.h>
#include "NeuralNetwork.h"
#include "Load_Save.h"
#include <err.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

double* get_desired_ouputs(int x);

void train();