#include <gtk/gtk.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <signal.h>


// Makes the widgets global
GtkBuilder* builder;
GtkWindow* window;
GtkFixed* fixed;
GtkButton* button_parameters;
GtkButton* button_start;
GtkButton* button_save;
GtkLabel* label_step;
GtkFileChooserButton* button_file_chooser;
GtkImage* grid;
gchar *file_name = NULL;

// function that recreate the new image selected
void change_image(GtkContainer* fixed, gchar* file_name, GtkWidget* grid)
{
    int hor = 270, ver = 100; // place the image manually **to modify**
    gtk_container_remove(fixed, grid); //remove the old grid
    grid = gtk_image_new_from_file (file_name);
    gtk_widget_show(grid);
    gtk_container_add(fixed, grid);
    gtk_fixed_move((GtkFixed*) fixed, grid, hor, ver);
}

//event handler for the "on_file_set" signal. Allows to choose the starting grid
void on_file_set(GtkFileChooserButton *f)
{
    file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(f)); //remember to free
    printf("file name: %s\n", file_name);
    printf("folder name: %s\n", gtk_file_chooser_get_current_folder (GTK_FILE_CHOOSER(f)));
    //charge the image
    change_image((GtkContainer*) fixed, file_name, (GtkWidget*) grid);
    //change the step label to charging the picture
    gtk_label_set_label((GtkLabel*)label_step, "step in progress : charging the image\n");
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
    button_start = GTK_BUTTON(gtk_builder_get_object(builder, "button_start"));
    button_save = GTK_BUTTON(gtk_builder_get_object(builder, "button_save"));
    label_step = GTK_LABEL(gtk_builder_get_object(builder, "label_step"));
    button_file_chooser = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "button_file_chooser"));
    grid = GTK_IMAGE(gtk_builder_get_object(builder, "grid"));
    //grid = NULL;

    // Connects event handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    //g_signal_connect(button_parameters, "clicked", G_CALLBACK(on_parameters), button_parameters);
    //g_signal_connect(stop_button, "clicked", G_CALLBACK(on_stop), &game);
    g_signal_connect(button_file_chooser, "file-set", G_CALLBACK(on_file_set), NULL);
    // g_signal_connect(window, "key_release_event", G_CALLBACK(on_key_release), &game);
    //g_signal_connect(area, "configure-event", G_CALLBACK(on_configure), &game);
    //g_signal_connect(training_cb, "toggled", G_CALLBACK(on_training), &game);

     // Runs the main loop
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show((GtkWidget*) window);// display the window
    gtk_main();

    // Exits.
    return 0;
}
