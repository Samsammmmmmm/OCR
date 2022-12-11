#pragma once

#include <gtk/gtk.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <signal.h>
#include <string.h>
#include "../Image_processing/filter_funct.h"
#include "../Grid/seg.h"
#include "../NeuralNetwork/Sudoku.h"
#include "../Solver/sudokuSolver.h"
#include "../Graphics/print_solu.h"

void update_image(GtkContainer* fixed, gchar* filename, GtkWidget* grid);
void on_file_set(GtkFileChooserButton *f);
void on_button_solve_clicked(GtkButton *button);
void on_button_save_clicked(GtkButton *button);
void on_button_parameters_clicked(GtkButton *button);
void on_button_train_clicked(GtkButton *button);
void on_dataset_set(GtkFileChooserButton *f);
void on_config_set(GtkFileChooserButton *f);
