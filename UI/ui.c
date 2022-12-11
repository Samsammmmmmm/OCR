#include "ui.h"

// add the headers of all the parts to call the diffrent functions


// Makes the widgets global
GtkBuilder* builder;
GtkWindow* window;
GtkFixed* fixed;
GtkButton* button_solve;
GtkButton* button_save;
GtkButton* button_parameters;
GtkButton* button_train;
/*GtkButton* button_startergrid;
GtkButton* button_pretreatment;
GtkButton* button_lignesdetec;
GtkButton* button_solved;*/
GtkLabel* label_step;
GtkLabel* label_dataset;
GtkLabel* label_config;
GtkFileChooserButton* button_file_chooser;
GtkFileChooserButton* button_dataset_chooser;
GtkFileChooserButton* button_config_chooser;
GtkImage* grid;
gchar *filename = NULL;
//widget from other functions
gchar *dataset = NULL;
gchar *configfile = NULL;

// function that recreate the new image selected
void update_image(GtkContainer* fixed, gchar* filename, GtkWidget* grid)
{
    /*int hor = 270, ver = 100; // place the image manually **to modify**
    gtk_container_remove(fixed, grid); //remove the old grid
    grid = gtk_image_new_from_file (filename);
    gtk_widget_show(grid);
    gtk_container_add(fixed, grid);
    gtk_fixed_move((GtkFixed*) fixed, grid, hor, ver);*/
    gtk_image_set_from_file((GtkImage*) grid, filename);
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
    //img_process(filename);
    //update_image((GtkContainer*) fixed, "BMP/result.png", (GtkWidget*) grid); //image pretreatment
    SDL_Surface* image_surface = load_image2("BMP/ligns1.png");
    SDL_Surface* image_filtered = load_image2("BMP/result1.png");
    segmentation(image_surface, image_filtered);

    update_image((GtkContainer*) fixed, "BMP/detection_resize.png", (GtkWidget*) grid);

    create_file_to_solve("NeuralNetwork/Weights/weights_12112022_104119.txt");
    //takes the solved grid in the files and displays it
    //update_image((GtkContainer*) fixed, "../Graphics/load.png", (GtkWidget*) grid);
    //change the step label
    gtk_label_set_label((GtkLabel*)label_step, "step in progress : grid solved\n");
    // activate the button save
    gtk_widget_set_sensitive ((GtkWidget*) button_save, TRUE);
}

// event handler for the save button --> save the grid displayed into a file
void on_button_save_clicked(GtkButton *button)
{
    SDL_Surface* solved_surface;
    gchar* fileresult = "Graphics/load.png"; // put the name of the solved grid
    solved_surface = IMG_Load(fileresult);
    if (SDL_SaveBMP(solved_surface, "results/result.bmp") != 0)
    {
        printf("Error : %s\n", SDL_GetError());
        }

    //change the step label
    gtk_label_set_label((GtkLabel*)label_step, "step in progress : grid saved in the folder /results\n");
}

void on_button_parameters_clicked(GtkButton *button)
{
    printf("parameters\n");
    if (gtk_widget_get_visible ((GtkWidget*)button_train)){
        gtk_widget_hide((GtkWidget*) label_dataset);// hide the buttons & labels
        gtk_widget_hide((GtkWidget*) label_config);
        gtk_widget_hide((GtkWidget*) button_train);
        gtk_widget_hide((GtkWidget*) button_dataset_chooser);
        gtk_widget_hide((GtkWidget*) button_config_chooser);
    }
    else{
    gtk_widget_show((GtkWidget*) label_dataset);// display the buttons & labels
    gtk_widget_show((GtkWidget*) label_config);
    gtk_widget_show((GtkWidget*) button_train);
    gtk_widget_show((GtkWidget*) button_dataset_chooser);
    gtk_widget_show((GtkWidget*) button_config_chooser);
    }
}
void on_button_train_clicked(GtkButton *button)
{
    printf("train clicked\n");
}
void on_dataset_set(GtkFileChooserButton *f)
{
    printf("dataset clicked\n");
    //filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(f));
}
void on_config_set(GtkFileChooserButton *f)
{
     printf("config clicked\n");
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
    if (gtk_builder_add_from_file(builder, "UI/SudokuUI.glade", &error) == 0)
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
    button_train = GTK_BUTTON(gtk_builder_get_object(builder, "button_train"));
    label_step = GTK_LABEL(gtk_builder_get_object(builder, "label_step"));
    label_dataset = GTK_LABEL(gtk_builder_get_object(builder, "label_dataset"));
    label_config = GTK_LABEL(gtk_builder_get_object(builder, "label_config"));
    button_file_chooser = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "button_file_chooser"));
    button_dataset_chooser = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "button_dataset_chooser"));
    button_config_chooser = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "button_config_chooser"));
    grid = GTK_IMAGE(gtk_builder_get_object(builder, "grid"));

    // Connects event handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(button_parameters, "clicked", G_CALLBACK(on_button_parameters_clicked), &button_parameters);
    g_signal_connect(button_train, "clicked", G_CALLBACK(on_button_train_clicked), NULL);
    g_signal_connect(button_solve, "clicked", G_CALLBACK(on_button_solve_clicked), NULL);
    g_signal_connect(button_save, "clicked", G_CALLBACK(on_button_save_clicked), NULL);
    g_signal_connect(button_file_chooser, "file-set", G_CALLBACK(on_file_set), NULL);
    g_signal_connect(button_dataset_chooser, "file-set", G_CALLBACK(on_dataset_set), NULL);
    g_signal_connect(button_config_chooser, "file-set", G_CALLBACK(on_config_set), NULL);

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
