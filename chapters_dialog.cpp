#include "chapters_dialog.h"
#include <vector>
#include <sstream>

static std::string seconds_to_hhmmss(gint64 secs) {
    int h = (int)(secs / 3600);
    int m = (int)((secs % 3600) / 60);
    int s = (int)(secs % 60);
    char buf[64];
    if (h > 0)
        snprintf(buf, sizeof(buf), "%d:%02d:%02d", h, m, s);
    else
        snprintf(buf, sizeof(buf), "%d:%02d", m, s);
    return std::string(buf);
}

void show_chapters_dialog(GtkWindow *parent, MusicBackend *backend, const std::string &current_file) {
    if (!backend) return;

    GtkWidget *dialog = gtk_dialog_new_with_buttons("Chapters",
                                                    parent,
                                                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                                    NULL);

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *tree = gtk_tree_view_new();
    GtkListStore *store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT); // Title, Seconds

    for (size_t i = 0; i < backend->chapters.size(); ++i) {
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        const auto &ch = backend->chapters[i];
        std::string title = ch.title.empty() ? ("Chapter " + std::to_string(i+1)) : ch.title;
        gtk_list_store_set(store, &iter, 0, title.c_str(), 1, (int)ch.timestamp, -1);
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(tree), GTK_TREE_MODEL(store));
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *col1 = gtk_tree_view_column_new_with_attributes("Title", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col1);

    GtkCellRenderer *renderer2 = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *col2 = gtk_tree_view_column_new_with_attributes("Time", renderer2, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col2);

    gtk_tree_view_column_set_cell_data_func(col2, renderer2,
        +[](GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data) {
            int seconds;
            gtk_tree_model_get(model, iter, 1, &seconds, -1);
            std::string t = seconds_to_hhmmss(seconds);
            g_object_set(renderer, "text", t.c_str(), NULL);
        }, NULL, NULL);

    gtk_container_add(GTK_CONTAINER(content), tree);
    gtk_widget_set_size_request(tree, 520, 300);
    gtk_widget_show_all(content);

    int resp = gtk_dialog_run(GTK_DIALOG(dialog));
    if (resp == GTK_RESPONSE_ACCEPT) {
        GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
        GtkTreeIter sel_iter;
        GtkTreeModel *model;
        if (gtk_tree_selection_get_selected(sel, &model, &sel_iter)) {
            int seconds;
            gtk_tree_model_get(model, &sel_iter, 1, &seconds, -1);
            for (size_t i = 0; i < backend->chapters.size(); ++i) {
                if ((int)backend->chapters[i].timestamp == seconds) {
                    if (!current_file.empty()) {
                        backend->play_file(current_file.c_str(), seconds);
                    } else {
                        backend->play_chapter(i);
                    }
                    break;
                }
            }
        }
    }

    gtk_widget_destroy(dialog);
    g_object_unref(store);
}
