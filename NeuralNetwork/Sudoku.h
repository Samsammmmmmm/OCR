#pragma once
#include <stdio.h>
#include "NeuralNetwork.h"
#include "Load_Save.h"
#include <err.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

double* get_desired_ouputs(int x);

Uint32 getpixel(SDL_Surface *surface, int x, int y);

int GetValueOfPixel(int x, int y, SDL_Surface *image);

double* to_array(SDL_Surface* surface);

int is_black(SDL_Surface* surface);

int get_number(network *_network);

void create_file_to_solve(char* weights_path);

void csv_to_txt(char* path, int* cpt);

char* train(char* path);

void test();