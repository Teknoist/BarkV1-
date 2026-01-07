#ifndef CHAPTERS_DIALOG_H
#define CHAPTERS_DIALOG_H

#include <gtk/gtk.h>
#include "music_backend.h"
#include <string>

void show_chapters_dialog(GtkWindow *parent, MusicBackend *backend, const std::string &current_file);

#endif // CHAPTERS_DIALOG_H
