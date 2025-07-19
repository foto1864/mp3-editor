#include <gtk/gtk.h>
// #include <stdlib.h>
#include <stdbool.h>

#define COVER_WIDTH 300
#define COVER_HEIGHT 300

bool folder_selected = false;

GtkWidget* create_window(GtkApplication* app, const gchar* title, gint width, gint height) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), title);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    return window;
}

GtkWidget* load_image(const char* img_filename, int width, int height) {
    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale(img_filename, width, height, TRUE, &error);
    if (!pixbuf) {
        g_printerr("Failed to load image: %s\n", error->message);
        g_error_free(error);
        exit(1);
    }
    GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
    g_object_unref(pixbuf);    
    return image;
}

void add_cover_image(GtkWidget *widget, gpointer data) {
    GtkWidget *label = GTK_WIDGET(data);
    if (!folder_selected) {
        gtk_label_set_text(GTK_LABEL(label), "You ain't selected no folder");
    }
    else {
        gtk_label_set_text(GTK_LABEL(label), "You have selected a folder");
    }
    return;
}

static void activate(GtkApplication *app, gpointer user_data) {

    // Create the window
    GtkWidget *window = create_window(app, "Mp3 Editor", 800, 400);

    // Create main box
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Create box for section 1 : Album cover and text
    GtkWidget *section_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(main_box), section_box, FALSE, FALSE, 0);

    // Load image
    GtkWidget *image = load_image("cover.png", COVER_WIDTH, COVER_HEIGHT);
    gtk_widget_set_halign(image, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(section_box), image, FALSE, FALSE, 0);

    // Create button for adding cover
    GtkWidget *add_cover = gtk_button_new_with_label("Set Album Cover");
    gtk_widget_set_size_request(add_cover, COVER_WIDTH, -1);
    gtk_widget_set_halign(add_cover, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(section_box), add_cover, FALSE, FALSE, 0);

    // Connect button to a function
    // The function writes to response label this time, and prints to the screen
    GtkWidget *response_label = gtk_label_new("");
    gtk_widget_set_size_request(response_label, COVER_WIDTH, -1);
    gtk_widget_set_halign(response_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(section_box), response_label, FALSE, FALSE, 0);
    g_signal_connect(add_cover, "clicked", G_CALLBACK(add_cover_image), response_label);

    // Create the rest of the dummy buttons (dummy for the time being)
    GtkWidget *button_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(main_box), button_row, FALSE, FALSE, 0);

    GtkWidget *button1 = gtk_button_new_with_label("Button1");
    GtkWidget *button2 = gtk_button_new_with_label("Button2");
    GtkWidget *button3 = gtk_button_new_with_label("Button3");

    gtk_box_pack_start(GTK_BOX(button_row), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_row), button2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_row), button3, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.Mp3Editor", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
