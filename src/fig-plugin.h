/* fig-plugin.h
 *
 * Copyright (C) 2014 Christian Hergert <christian@hergert.me>
 *
 * This file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FIG_PLUGIN_H
#define FIG_PLUGIN_H

#include <glib-object.h>

G_BEGIN_DECLS

#define FIG_TYPE_PLUGIN            (fig_plugin_get_type())
#define FIG_PLUGIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_PLUGIN, FigPlugin))
#define FIG_PLUGIN_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), FIG_TYPE_PLUGIN, FigPlugin const))
#define FIG_PLUGIN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  FIG_TYPE_PLUGIN, FigPluginClass))
#define FIG_IS_PLUGIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FIG_TYPE_PLUGIN))
#define FIG_IS_PLUGIN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  FIG_TYPE_PLUGIN))
#define FIG_PLUGIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  FIG_TYPE_PLUGIN, FigPluginClass))

typedef struct _FigPlugin        FigPlugin;
typedef struct _FigPluginClass   FigPluginClass;
typedef struct _FigPluginPrivate FigPluginPrivate;

struct _FigPlugin
{
   GObject parent;

   /*< private >*/
   FigPluginPrivate *priv;
};

struct _FigPluginClass
{
   GObjectClass parent_class;
};

GType      fig_plugin_get_type     (void) G_GNUC_CONST;
FigPlugin *fig_plugin_new_for_path (const gchar  *path);
gboolean   fig_plugin_load         (FigPlugin    *plugin,
                                    GError      **error);
gboolean   fig_plugin_unload       (FigPlugin    *plugin,
                                    GError      **error);

G_END_DECLS

#endif /* FIG_PLUGIN_H */
