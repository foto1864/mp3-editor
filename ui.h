#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>

extern GtkListStore *mp3_store;
extern gchar *cover_path;

void on_activate(GtkApplication *app, gpointer user_data);

void on_apply_clicked(GtkButton *button, gpointer user_data);

#endif
