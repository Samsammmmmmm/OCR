#pragma once
#include "load_and_save.h"

//mettre le prototype des fonctions
int is_valid(char grid[9][9], int r, int c, char n);
void solver(char grid[9][9]);
int solver_rec(char grid[9][9], int r, int c);
void print_grid(char grid[9][9]);
void solverM(char* path);