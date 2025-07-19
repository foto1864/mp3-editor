#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Playground");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    label1 = gtk_label_new("Hello, world!");
    gtk_widget_set_margin_top(label1, 20);
    gtk_widget_set_margin_start(label1, 20);
    gtk_box_pack_start(GTK_BOX(vbox), label1, TRUE, TRUE, 0);
    gtk_widget_set_halign(label1, GTK_ALIGN_START);

    label2 = gtk_label_new("Welcome to the bruv-zone.");
    gtk_box_pack_start(GTK_BOX(vbox), label2, TRUE, TRUE, 0);

    button = gtk_button_new_with_label("Click Me");
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 20);

    GtkWidget *btn1 = gtk_button_new_with_label("Top Button");
    GtkWidget *btn2 = gtk_button_new_with_label("Bottom Button");

    gtk_widget_set_hexpand(btn1, TRUE);
    gtk_widget_set_vexpand(btn1, TRUE);

    gtk_widget_set_hexpand(btn2, TRUE);
    gtk_widget_set_vexpand(btn2, TRUE);

    gtk_box_pack_start(GTK_BOX(vbox), btn1, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), btn2, TRUE, TRUE, 5);


    gtk_widget_show_all(window);
}


int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.HelloGTK", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
