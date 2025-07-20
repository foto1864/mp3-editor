#include "ui.h"
#include "mp3_handler.h"

static GtkWidget *image_widget;

gchar *cover_path = NULL;
GtkListStore *mp3_store = NULL;

void on_folder_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Select Folder",
        GTK_WINDOW(user_data),
        GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Open", GTK_RESPONSE_ACCEPT,
        NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *folder = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        GList *mp3s = load_mp3_files_from_folder(folder);

        gtk_list_store_clear(mp3_store);
        for (GList *l = mp3s; l != NULL; l = l->next) {
            MP3Metadata *meta = l->data;
            GtkTreeIter iter;
            gtk_list_store_append(mp3_store, &iter);
            gtk_list_store_set(mp3_store, &iter,
                0, meta->title,
                1, meta->artist,
                2, meta->album,
                3, meta->filepath,  // hidden full path
                -1);
        }

        free_mp3_metadata_list(mp3s);
        g_free(folder);
    }

    gtk_widget_destroy(dialog);
}

void on_cover_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Select Cover Image",
        GTK_WINDOW(user_data),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Open", GTK_RESPONSE_ACCEPT,
        NULL);

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pixbuf_formats(filter);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    char *filename = NULL;
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        if (filename && g_file_test(filename, G_FILE_TEST_EXISTS)) {
            if (cover_path) {
                g_free(cover_path);
                cover_path = NULL;
            }

            cover_path = g_strdup(filename);

            GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size(filename, 200, 200, NULL);
            if (pixbuf) {
                gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), pixbuf);
                g_object_unref(pixbuf);
            }
        }

        g_free(filename);  // now safe to free
    }

    gtk_widget_destroy(dialog);
}


#include <stdio.h>

#define EYED3_PATH "/home/foto1864/Desktop/Militos/Code-Snippets/myenv/bin/eyeD3"

static void write_metadata_with_eyed3(const gchar *filepath, const gchar *title, const gchar *artist, const gchar *album, const gchar *cover) {
    gchar *escaped_title = g_shell_quote(title);
    gchar *escaped_artist = g_shell_quote(artist);
    gchar *escaped_album = g_shell_quote(album);
    gchar *escaped_path = g_shell_quote(filepath);
    gchar *escaped_cover = cover ? g_shell_quote(cover) : NULL;

    gchar *cmd;
    if (cover) {
        cmd = g_strdup_printf("%s --title=%s --artist=%s --album=%s --add-image=%s:FRONT_COVER %s",
                              EYED3_PATH, escaped_title, escaped_artist, escaped_album, escaped_cover, escaped_path);
    } else {
        cmd = g_strdup_printf("%s --title=%s --artist=%s --album=%s %s",
                              EYED3_PATH, escaped_title, escaped_artist, escaped_album, escaped_path);
    }

    g_print("Running command: %s\n", cmd);
    system(cmd);

    g_free(escaped_title);
    g_free(escaped_artist);
    g_free(escaped_album);
    g_free(escaped_path);
    if (escaped_cover) g_free(escaped_cover);
    g_free(cmd);
}

void on_apply_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget **entries = (GtkWidget **)user_data;
    GtkEntry *artist_entry = GTK_ENTRY(entries[0]);
    GtkEntry *album_entry = GTK_ENTRY(entries[1]);

    const gchar *artist = gtk_entry_get_text(artist_entry);
    const gchar *album = gtk_entry_get_text(album_entry);

    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(mp3_store), &iter);

    while (valid) {
        gchar *title;
        gchar *filepath;

        gtk_tree_model_get(GTK_TREE_MODEL(mp3_store), &iter,
                           0, &title,
                           -1);

        // We reconstruct the file path from the title (BAD idea unless we track full paths)
        // TODO: Replace this with real path tracking
        gchar *cwd = g_get_current_dir();
        gtk_tree_model_get(GTK_TREE_MODEL(mp3_store), &iter, 3, &filepath, -1);
        g_free(cwd);

        write_metadata_with_eyed3(filepath, title, artist, album, cover_path);

        g_free(title);
        g_free(filepath);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(mp3_store), &iter);
    }

    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                               "Metadata updated for all MP3s!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}


void on_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "MP3 Metadata Editor");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 650);
    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){0.18, 0.18, 0.18, 1});

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Left pane
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(main_box), left_box, FALSE, FALSE, 0);

    GtkWidget *folder_btn = gtk_button_new_with_label("📁 Select Folder");
    g_signal_connect(folder_btn, "clicked", G_CALLBACK(on_folder_clicked), window);
    gtk_box_pack_start(GTK_BOX(left_box), folder_btn, FALSE, FALSE, 0);

    GtkWidget *cover_btn = gtk_button_new_with_label("🖼️ Select Cover");
    g_signal_connect(cover_btn, "clicked", G_CALLBACK(on_cover_clicked), window);
    gtk_box_pack_start(GTK_BOX(left_box), cover_btn, FALSE, FALSE, 0);

    image_widget = gtk_image_new();
    gtk_widget_set_size_request(image_widget, 200, 200);
    gtk_box_pack_start(GTK_BOX(left_box), image_widget, FALSE, FALSE, 0);

    // Right pane (table + entries)
    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(main_box), right_box, TRUE, TRUE, 0);

    GtkWidget *artist_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(artist_entry), "Artist");
    gtk_box_pack_start(GTK_BOX(right_box), artist_entry, FALSE, FALSE, 0);

    GtkWidget *album_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(album_entry), "Album");
    gtk_box_pack_start(GTK_BOX(right_box), album_entry, FALSE, FALSE, 0);

    // MP3 list view
    mp3_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    // title, artist, album, full path

    GtkWidget *treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(mp3_store));

    const char *titles[] = {"Title", "Artist", "Album"};
    for (int i = 0; i < 3; ++i) {
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(titles[i], renderer, "text", i, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }

    gtk_box_pack_start(GTK_BOX(right_box), treeview, TRUE, TRUE, 0);

    // Allocate entry array to pass both artist and album fields
    GtkWidget **entry_array = g_new(GtkWidget *, 2);
    entry_array[0] = artist_entry;
    entry_array[1] = album_entry;

    // Create the Apply button
    GtkWidget *apply_button = gtk_button_new_with_label("💾 Apply to All MP3s");
    gtk_widget_set_name(apply_button, "apply_button");

    // Green text on dark mode (optional style)
    GtkStyleContext *context = gtk_widget_get_style_context(apply_button);
    gtk_style_context_add_class(context, "suggested-action");

    // Connect signal
    g_signal_connect(apply_button, "clicked", G_CALLBACK(on_apply_clicked), entry_array);

    // Pack into the right_box (or wherever your entries are)
    gtk_box_pack_start(GTK_BOX(right_box), apply_button, FALSE, FALSE, 10);


    gtk_widget_show_all(window);
}
