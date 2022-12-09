#include <gtk/gtk.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <signal.h>
#include <string.h>
// add the headers of all the parts to call the diffrent functions


// Makes the widgets global
GtkBuilder* builder;
GtkWindow* window;
GtkFixed* fixed;
GtkButton* button_parameters;
GtkButton* button_solve;
GtkButton* button_save;
GtkLabel* label_step;
GtkFileChooserButton* button_file_chooser;
GtkImage* grid;
gchar *filename = NULL;

// function that recreate the new image selected
void update_image(GtkContainer* fixed, gchar* filename, GtkWidget* grid)
{
    int hor = 270, ver = 100; // place the image manually **to modify**
    gtk_container_remove(fixed, grid); //remove the old grid
    grid = gtk_image_new_from_file (filename);
    gtk_widget_show(grid);
    gtk_container_add(fixed, grid);
    gtk_fixed_move((GtkFixed*) fixed, grid, hor, ver);
    //gtk_image_set_from_file(GTK_IMAGE(preview), (const gchar*) filename);
}

//event handler for the "file-set" signal. Allows to choose the starting grid
void on_file_set(GtkFileChooserButton *f)
{
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(f)); //remember to free
    printf("file name: %s\n", filename);
    printf("folder name: %s\n", gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER(f)));
    //charge the image
    update_image((GtkContainer*) fixed, filename, (GtkWidget*) grid);
    //change the step label to charging the picture
    gtk_label_set_label((GtkLabel*)label_step, "step in progress : charging the image\n");
    // activate the button solve
    gtk_widget_set_sensitive ((GtkWidget*) button_solve, TRUE);
    gtk_widget_set_sensitive ((GtkWidget*) button_save, FALSE);
}

// event handler for the solve button. Call all the functions to solve the grid
void on_button_solve_clicked(GtkButton *button)
{
    // put all the functions that solve the grid
    
    //takes the solved grid in the files and displays it
    
    //update_image((GtkContainer*) fixed, filename + .result, (GtkWidget*) grid);
    
    //change the step label
    gtk_label_set_label((GtkLabel*)label_step, "step in progress : grid solved\n");
    // activate the button save
    gtk_widget_set_sensitive ((GtkWidget*) button_save, TRUE);
}

// event handler for the save button --> save the grid displayed into a file
void on_button_save_clicked(GtkButton *button)
{
    printf("save\n");
    /*SDL_Surface* solved_surface;
      solved_surface = load_image*/
}

int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(&argc, &argv);

    // Constructs a GtkBuilder instance.
    GtkBuilder* builder = gtk_builder_new ();

    // Loads the UI description.
    // (Exits if an error occurs.)
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "SudokuUI.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
    fixed = GTK_FIXED(gtk_builder_get_object(builder, "fixed"));
    button_parameters = GTK_BUTTON(gtk_builder_get_object(builder, "button_parameters"));
    button_solve = GTK_BUTTON(gtk_builder_get_object(builder, "button_solve"));
    button_save = GTK_BUTTON(gtk_builder_get_object(builder, "button_save"));
    label_step = GTK_LABEL(gtk_builder_get_object(builder, "label_step"));
    button_file_chooser = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "button_file_chooser"));
    grid = GTK_IMAGE(gtk_builder_get_object(builder, "grid"));

    // Connects event handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    //g_signal_connect(button_parameters, "clicked", G_CALLBACK(on_parameters), button_parameters);
    g_signal_connect(button_solve, "clicked", G_CALLBACK(on_button_solve_clicked), NULL);
    g_signal_connect(button_save, "clicked", G_CALLBACK(on_button_save_clicked), NULL);
    g_signal_connect(button_file_chooser, "file-set", G_CALLBACK(on_file_set), NULL);

    //Initialize the first states
    gtk_widget_set_sensitive ((GtkWidget*) button_solve, FALSE);
    gtk_widget_set_sensitive ((GtkWidget*) button_save, FALSE);

     // Runs the main loop
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show((GtkWidget*) window);// display the window
    gtk_main();

    // Exits.
    return 0;
}
