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

#ifndef __GD_MINER_TRACKER_H__
#define __GD_MINER_TRACKER_H__

#include <gio/gio.h>
#include <libtracker-sparql/tracker-sparql.h>

G_BEGIN_DECLS

gchar *gd_miner_tracker_sparql_connection_ensure_resource (TrackerSparqlConnection *connection,
                                                           GCancellable *cancellable,
                                                           GError **error,
                                                           gboolean *resource_exists,
                                                           const gchar *graph,
                                                           const gchar *identifier,
                                                           const gchar *class,
                                                           ...);

gboolean gd_miner_tracker_sparql_connection_insert_or_replace_triple (TrackerSparqlConnection *connection,
                                                                      GCancellable *cancellable,
                                                                      GError **error,
                                                                      const gchar *graph,
                                                                      const gchar *resource,
                                                                      const gchar *property_name,
                                                                      const gchar *property_value);

gboolean gd_miner_tracker_sparql_connection_set_triple (TrackerSparqlConnection *connection,
                                                        GCancellable *cancellable,
                                                        GError **error,
                                                        const gchar *graph,
                                                        const gchar *resource,
                                                        const gchar *property_name,
                                                        const gchar *property_value);

gboolean gd_miner_tracker_sparql_connection_toggle_favorite (TrackerSparqlConnection *connection,
                                                             GCancellable *cancellable,
                                                             GError **error,
                                                             const gchar *resource,
                                                             gboolean favorite);

gchar* gd_miner_tracker_utils_ensure_contact_resource (TrackerSparqlConnection *connection,
                                                       GCancellable *cancellable,
                                                       GError **error,
                                                       const gchar *email,
                                                       const gchar *fullname);

void gd_miner_tracker_update_datasource (TrackerSparqlConnection  *connection,
                                         const gchar              *datasource_urn,
                                         gboolean                  resource_exists,
                                         const gchar              *identifier,
                                         const gchar              *resource,
                                         GCancellable             *cancellable,
                                         GError                  **error);
gboolean gd_miner_tracker_update_mtime (TrackerSparqlConnection  *connection,
                                        gint64                    new_mtime,
                                        gboolean                  resource_exists,
                                        const gchar              *identifier,
                                        const gchar              *resource,
                                        GCancellable             *cancellable,
                                        GError                  **error);

G_END_DECLS

#endif /* __GD_MINER_TRACKER_H__ */
