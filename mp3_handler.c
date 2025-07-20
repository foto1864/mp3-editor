#include "mp3_handler.h"
#include <taglib/tag_c.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

static gboolean has_mp3_extension(const gchar *filename) {
    const gchar *dot = strrchr(filename, '.');
    return dot && g_ascii_strcasecmp(dot, ".mp3") == 0;
}

GList *load_mp3_files_from_folder(const gchar *folder_path) {
    GList *metadata_list = NULL;
    DIR *dir = opendir(folder_path);
    if (!dir) return NULL;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_REG) continue;
        if (!has_mp3_extension(entry->d_name)) continue;

        gchar *full_path = g_build_filename(folder_path, entry->d_name, NULL);
        TagLib_File *file = taglib_file_new(full_path);
        if (!file) {
            g_free(full_path);
            continue;
        }

        const TagLib_Tag *tag = taglib_file_tag(file);
        MP3Metadata *meta = g_malloc(sizeof(MP3Metadata));
        meta->filepath = g_strdup(full_path);
        meta->title  = g_strdup(taglib_tag_title(tag));
        meta->artist = g_strdup(taglib_tag_artist(tag));
        meta->album  = g_strdup(taglib_tag_album(tag));

        if (!meta->title || !*meta->title) meta->title = g_strdup(entry->d_name);
        if (!meta->artist) meta->artist = g_strdup("");
        if (!meta->album) meta->album = g_strdup("");

        metadata_list = g_list_append(metadata_list, meta);
        taglib_file_free(file);
        g_free(full_path);
    }

    closedir(dir);
    return metadata_list;
}

void free_mp3_metadata_list(GList *list) {
    for (GList *l = list; l != NULL; l = l->next) {
        MP3Metadata *meta = l->data;
        g_free(meta->title);
        g_free(meta->artist);
        g_free(meta->album);
        g_free(meta->filepath);
        g_free(meta);
    }
    g_list_free(list);
}
