#pragma once

void update_image(GtkContainer* fixed, gchar* filename, GtkWidget* grid);
void on_file_set(GtkFileChooserButton *f);
void on_button_solve_clicked(GtkButton *button);
void on_button_save_clicked(GtkButton *button);
void on_button_parameters_clicked(GtkButton *button);
void on_button_train_clicked(GtkButton *button);
void on_dataset_set(GtkFileChooserButton *f);
void on_config_set(GtkFileChooserButton *f);
