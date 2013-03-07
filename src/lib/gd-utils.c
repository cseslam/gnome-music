/*
 * Copyright (c) 2011, 2012 Red Hat, Inc.
 *
 * Gnome Documents is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * Gnome Documents is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Gnome Documents; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Author: Cosimo Cecchi <cosimoc@redhat.com>
 *
 */

#include "gd-utils.h"

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string.h>
#include <math.h>

#define GNOME_DESKTOP_USE_UNSTABLE_API
#include <libgnome-desktop/gnome-desktop-thumbnail.h>

#define ATTRIBUTES_FOR_THUMBNAIL \
  G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE"," \
  G_FILE_ATTRIBUTE_TIME_MODIFIED

static gboolean
create_thumbnail (GIOSchedulerJob *job,
                  GCancellable *cancellable,
                  gpointer user_data)
{
  GSimpleAsyncResult *result = user_data;
  GFile *file = G_FILE (g_async_result_get_source_object (G_ASYNC_RESULT (result)));
  GnomeDesktopThumbnailFactory *factory;
  GFileInfo *info;
  gchar *uri;
  GdkPixbuf *pixbuf;
  guint64 mtime;

  uri = g_file_get_uri (file);
  info = g_file_query_info (file, ATTRIBUTES_FOR_THUMBNAIL,
                            G_FILE_QUERY_INFO_NONE,
                            NULL, NULL);

  /* we don't care about reporting errors here, just fail the
   * thumbnail.
   */
  if (info == NULL)
    {
      g_simple_async_result_set_op_res_gboolean (result, FALSE);
      goto out;
    }

  mtime = g_file_info_get_attribute_uint64 (info, G_FILE_ATTRIBUTE_TIME_MODIFIED);

  factory = gnome_desktop_thumbnail_factory_new (GNOME_DESKTOP_THUMBNAIL_SIZE_NORMAL);
  pixbuf = gnome_desktop_thumbnail_factory_generate_thumbnail
    (factory, 
     uri, g_file_info_get_content_type (info));

  if (pixbuf != NULL)
    {
      gnome_desktop_thumbnail_factory_save_thumbnail (factory, pixbuf,
                                                      uri, (time_t) mtime);
      g_simple_async_result_set_op_res_gboolean (result, TRUE);
    }
  else
    {
      g_simple_async_result_set_op_res_gboolean (result, FALSE);
    }

  g_object_unref (info);
  g_object_unref (file);
  g_object_unref (factory);
  g_clear_object (&pixbuf);

 out:
  g_simple_async_result_complete_in_idle (result);
  g_object_unref (result);

  return FALSE;
}

void
gd_queue_thumbnail_job_for_file_async (GFile *file,
                                       GAsyncReadyCallback callback,
                                       gpointer user_data)
{
  GSimpleAsyncResult *result;

  result = g_simple_async_result_new (G_OBJECT (file),
                                      callback, user_data, 
                                      gd_queue_thumbnail_job_for_file_async);

  g_io_scheduler_push_job (create_thumbnail,
                           result, NULL,
                           G_PRIORITY_DEFAULT, NULL);
}

gboolean
gd_queue_thumbnail_job_for_file_finish (GAsyncResult *res)
{
  GSimpleAsyncResult *simple = G_SIMPLE_ASYNC_RESULT (res);

  return g_simple_async_result_get_op_res_gboolean (simple);
}

/* taken from eel/eel-gtk-extensions.c */
static gboolean 
tree_view_button_press_callback (GtkWidget *tree_view,
				 GdkEventButton *event,
				 gpointer data)
{
	GtkTreePath *path;
	GtkTreeViewColumn *column;

	if (event->button == 1 && event->type == GDK_BUTTON_PRESS) {
		if (gtk_tree_view_get_path_at_pos (GTK_TREE_VIEW (tree_view),
						   event->x, event->y,
						   &path,
						   &column,
						   NULL, 
						   NULL)) {
			gtk_tree_view_row_activated
				(GTK_TREE_VIEW (tree_view), path, column);
		}
	}

	return FALSE;
}

void
gd_gtk_tree_view_set_activate_on_single_click (GtkTreeView *tree_view,
                                               gboolean should_activate)
{
	guint button_press_id;

	button_press_id = GPOINTER_TO_UINT 
		(g_object_get_data (G_OBJECT (tree_view), 
				    "gd-tree-view-activate"));

	if (button_press_id && !should_activate) {
		g_signal_handler_disconnect (tree_view, button_press_id);
		g_object_set_data (G_OBJECT (tree_view), 
				   "gd-tree-view-activate", 
				   NULL);
	} else if (!button_press_id && should_activate) {
		button_press_id = g_signal_connect 
			(tree_view,
			 "button_press_event",
			 G_CALLBACK  (tree_view_button_press_callback),
			 NULL);
		g_object_set_data (G_OBJECT (tree_view), 
				   "gd-tree-view-activate", 
				   GUINT_TO_POINTER (button_press_id));
	}
}

const char *
gd_filename_get_extension_offset (const char *filename)
{
	char *end, *end2;

	end = strrchr (filename, '.');

	if (end && end != filename) {
		if (strcmp (end, ".gz") == 0 ||
		    strcmp (end, ".bz2") == 0 ||
		    strcmp (end, ".sit") == 0 ||
		    strcmp (end, ".Z") == 0) {
			end2 = end - 1;
			while (end2 > filename &&
			       *end2 != '.') {
				end2--;
			}
			if (end2 != filename) {
				end = end2;
			}
		}
	}

	return end;
}

/**
 * gd_filename_strip_extension:
 * @filename_with_extension: (allow-none):
 *
 * Returns: (transfer full):
 */
char *
gd_filename_strip_extension (const char * filename_with_extension)
{
	char *filename, *end;

	if (filename_with_extension == NULL) {
		return NULL;
	}

	filename = g_strdup (filename_with_extension);
	end = (gchar *) gd_filename_get_extension_offset (filename);

	if (end && end != filename) {
		*end = '\0';
	}

	return filename;
}

/**
 * gd_filename_to_mime_type:
 * @filename_with_extension:
 *
 * Returns: (transfer none):
 */
const char *
gd_filename_to_mime_type (const gchar *filename_with_extension)
{
  const gchar *extension;
  const gchar *type = NULL;

  g_return_val_if_fail (filename_with_extension != NULL, NULL);

  extension = gd_filename_get_extension_offset (filename_with_extension);

  if (g_strcmp0 (extension, ".pdf") == 0)
    type = "application/pdf";

  return type;
}

/**
 * gd_filename_to_rdf_type:
 * @filename_with_extension:
 *
 * Returns: (transfer none):
 */
const char *
gd_filename_to_rdf_type (const gchar *filename_with_extension)
{
  const gchar *extension;
  const gchar *type = NULL;

  g_return_val_if_fail (filename_with_extension != NULL, NULL);

  extension = gd_filename_get_extension_offset (filename_with_extension);

  if (g_strcmp0 (extension, ".txt") == 0)
    type = "nfo:HtmlDocument";

  else if (g_strcmp0 (extension, ".doc") == 0
      || g_strcmp0 (extension, ".docm") == 0
      || g_strcmp0 (extension, ".docx") == 0
      || g_strcmp0 (extension, ".dot") == 0
      || g_strcmp0 (extension, ".dotx") == 0
      || g_strcmp0 (extension, ".epub") == 0
      || g_strcmp0 (extension, ".pdf") == 0)
    type = "nfo:PaginatedTextDocument";

  else if (g_strcmp0 (extension, ".pot") == 0
           || g_strcmp0 (extension, ".potm") == 0
           || g_strcmp0 (extension, ".potx") == 0
           || g_strcmp0 (extension, ".pps") == 0
           || g_strcmp0 (extension, ".ppsm") == 0
           || g_strcmp0 (extension, ".ppsx") == 0
           || g_strcmp0 (extension, ".ppt") == 0
           || g_strcmp0 (extension, ".pptm") == 0
           || g_strcmp0 (extension, ".pptx") == 0)
    type = "nfo:Presentation";

  else if (g_strcmp0 (extension, ".txt") == 0)
    type = "nfo:PlainTextDocument";

  else if (g_strcmp0 (extension, ".xls") == 0
           || g_strcmp0 (extension, ".xlsb") == 0
           || g_strcmp0 (extension, ".xlsm") == 0
           || g_strcmp0 (extension, ".xlsx") == 0)
    type = "nfo:Spreadsheet";

  return type;
}

/**
 * gd_iso8601_from_timestamp:
 * @timestamp:
 *
 * Returns: (transfer full):
 */
gchar *
gd_iso8601_from_timestamp (gint64 timestamp)
{
  GTimeVal tv;

  tv.tv_sec = timestamp;
  tv.tv_usec = 0;
  return g_time_val_to_iso8601 (&tv);
}

/**
 * gd_create_collection_icon:
 * @base_size:
 * @pixbufs: (element-type GdkPixbuf):
 *
 * Returns: (transfer full):
 */
GIcon *
gd_create_collection_icon (gint base_size,
                           GList *pixbufs)
{
  cairo_surface_t *surface;
  GIcon *retval;
  cairo_t *cr;
  GtkStyleContext *context;
  GtkWidgetPath *path;
  gint padding, tile_size, scale_size;
  gint pix_width, pix_height;
  gint idx, cur_x, cur_y;
  GList *l;
  GdkPixbuf *pix;

  /* TODO: do not hardcode 4, but scale to another layout if more
   * pixbufs are provided.
   */

  padding = MAX (floor (base_size / 10), 4);
  tile_size = (base_size - (3 * padding)) / 2;

  context = gtk_style_context_new ();
  gtk_style_context_add_class (context, "documents-collection-icon");

  path = gtk_widget_path_new ();
  gtk_widget_path_append_type (path, GTK_TYPE_ICON_VIEW);
  gtk_style_context_set_path (context, path);
  gtk_widget_path_unref (path);

  surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, base_size, base_size);
  cr = cairo_create (surface);

  gtk_render_background (context, cr,
                         0, 0, base_size, base_size);

  l = pixbufs;
  idx = 0;
  cur_x = padding;
  cur_y = padding;

  while (l != NULL && idx < 4)
    {
      pix = l->data;
      pix_width = gdk_pixbuf_get_width (pix);
      pix_height = gdk_pixbuf_get_height (pix);

      scale_size = MIN (pix_width, pix_height);

      cairo_save (cr);

      cairo_translate (cr, cur_x, cur_y);

      cairo_rectangle (cr, 0, 0,
                       tile_size, tile_size);
      cairo_clip (cr);

      cairo_scale (cr, (gdouble) tile_size / (gdouble) scale_size, (gdouble) tile_size / (gdouble) scale_size);
      gdk_cairo_set_source_pixbuf (cr, pix, 0, 0);

      cairo_paint (cr);
      cairo_restore (cr);

      if ((idx % 2) == 0)
        {
          cur_x += tile_size + padding;
        }
      else
        {
          cur_x = padding;
          cur_y += tile_size + padding;
        }

      idx++;
      l = l->next;
    }

  retval = G_ICON (gdk_pixbuf_get_from_surface (surface, 0, 0, base_size, base_size));

  cairo_surface_destroy (surface);
  cairo_destroy (cr);
  g_object_unref (context);

  return retval;
}

/**
 * gd_create_variant_from_pixbuf:
 * @pixbuf:
 *
 * Returns: (transfer full):
 */
GVariant *
gd_create_variant_from_pixbuf (GdkPixbuf *pixbuf)
{
  GVariant *variant;
  guchar *data;
  guint   length;

  data = gdk_pixbuf_get_pixels_with_length (pixbuf, &length);
  variant = g_variant_new ("(iiibii@ay)",
                           gdk_pixbuf_get_width (pixbuf),
                           gdk_pixbuf_get_height (pixbuf),
                           gdk_pixbuf_get_rowstride (pixbuf),
                           gdk_pixbuf_get_has_alpha (pixbuf),
                           gdk_pixbuf_get_bits_per_sample (pixbuf),
                           gdk_pixbuf_get_n_channels (pixbuf),
                           g_variant_new_from_data (G_VARIANT_TYPE_BYTESTRING,
                                                    data, length, TRUE,
                                                    (GDestroyNotify)g_object_unref,
                                                    g_object_ref (pixbuf)));
  return g_variant_ref_sink (variant);
}

void
gd_ev_view_find_changed (EvView *view,
                         EvJobFind *job,
                         gint page)
{
  ev_view_find_changed (view,
                        ev_job_find_get_results (job),
                        page);
}

static void
miner_manager_index_file_thread (GTask *task,
                                 gpointer source_object,
                                 gpointer task_data,
                                 GCancellable *cancellable)
{
  TrackerMinerManager *manager = source_object;
  GFile *file = task_data;
  GError *error = NULL;

  tracker_miner_manager_index_file (manager, file, &error);
  if (error != NULL)
    g_task_return_error (task, error);
  else
    g_task_return_boolean (task, TRUE);
}

void
gd_tracker_miner_manager_index_file_async (TrackerMinerManager *manager,
                                           GFile *file,
                                           GAsyncReadyCallback callback,
                                           gpointer user_data)
{
  GTask *task = g_task_new (manager, NULL, callback, user_data);
  g_task_set_task_data (task, g_object_ref (file), (GDestroyNotify) g_object_unref);
  g_task_run_in_thread (task, miner_manager_index_file_thread);
  g_object_unref (task);
}

gboolean
gd_tracker_miner_manager_index_file_finish (TrackerMinerManager *manager,
                                            GAsyncResult *result,
                                            GError **error)
{
  return g_task_propagate_boolean (G_TASK (result), error);
}
