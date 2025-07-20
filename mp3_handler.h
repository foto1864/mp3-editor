#ifndef MP3_HANDLER_H
#define MP3_HANDLER_H

#include <gtk/gtk.h>

typedef struct {
    gchar *title;
    gchar *artist;
    gchar *album;
    gchar *filepath;
} MP3Metadata;

GList *load_mp3_files_from_folder(const gchar *folder_path);
void free_mp3_metadata_list(GList *list);

#endif
