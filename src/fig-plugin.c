/* fig-plugin.c
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

#include <gio/gio.h>
#include <glib/gi18n.h>
#include <gmodule.h>

#include "fig-plugin.h"

G_DEFINE_TYPE (FigPlugin, fig_plugin, G_TYPE_OBJECT)

struct _FigPluginPrivate
{
   gchar   *path;
   GModule *module;
};

enum
{
   PROP_0,
   PROP_PATH,
   LAST_PROP
};

static GParamSpec *gParamSpecs [LAST_PROP];

FigPlugin *
fig_plugin_new_for_path (const gchar *path)
{
   return g_object_new (FIG_TYPE_PLUGIN, "path", path, NULL);
}

const gchar *
fig_plugin_get_path (FigPlugin *plugin)
{
   g_return_val_if_fail (plugin, NULL);

   return plugin->priv->path;
}

void
fig_plugin_set_path (FigPlugin   *plugin,
                     const gchar *path)
{
   g_return_if_fail (FIG_IS_PLUGIN (plugin));

   g_free (plugin->priv->path);
   plugin->priv->path = g_strdup (path);
   g_object_notify_by_pspec (G_OBJECT (plugin), gParamSpecs [PROP_PATH]);
}

gboolean
fig_plugin_load (FigPlugin  *plugin,
                 GError    **error)
{
   FigPluginPrivate *priv;
   void (*load) (void) = NULL;

   g_return_val_if_fail (FIG_IS_PLUGIN (plugin), FALSE);

   priv = plugin->priv;

   if (!priv->path) {
      g_set_error (error,
                   G_IO_ERROR,
                   G_IO_ERROR_INVALID_FILENAME,
                   "Must set :path property before loading.");
      return FALSE;
   }

   priv->module = g_module_open (priv->path, G_MODULE_BIND_LOCAL);
   if (!priv->module) {
      g_set_error (error,
                   G_IO_ERROR,
                   G_IO_ERROR_FAILED,
                   "Failed to open module: %s",
                   g_module_error ());
      return FALSE;
   }

#define FIG_PLUGIN_SYMBOL "fig_plugin_init"

   if (!g_module_symbol (priv->module, FIG_PLUGIN_SYMBOL, (gpointer *)&load)) {
      g_module_close (priv->module);
      priv->module = NULL;
      g_set_error (error,
                   G_IO_ERROR,
                   G_IO_ERROR_FAILED,
                   "Failed to locate symbol " FIG_PLUGIN_SYMBOL ": %s",
                   priv->path);
      return FALSE;
   }

   load ();

   return TRUE;
}

gboolean
fig_plugin_unload (FigPlugin  *plugin,
                   GError    **error)
{
   FigPluginPrivate *priv;
   gboolean ret = TRUE;

   g_return_val_if_fail (FIG_IS_PLUGIN (plugin), FALSE);

   priv = plugin->priv;

   if (priv->module) {
      if (!g_module_close (priv->module)) {
         g_set_error (error,
                      G_IO_ERROR,
                      G_IO_ERROR_FAILED,
                      "Failed to unload module: %s",
                      g_module_error ());
         ret = FALSE;
      }
      priv->module = NULL;
   }

   return ret;
}

static void
fig_plugin_finalize (GObject *object)
{
   FigPluginPrivate *priv;

   priv = FIG_PLUGIN(object)->priv;

   if (priv->module) {
      if (!g_module_close (priv->module)) {
         g_warning ("Failed to close module: %s",
                    g_module_name (priv->module));
      }
   }

   G_OBJECT_CLASS(fig_plugin_parent_class)->finalize(object);
}

static void
fig_plugin_get_property (GObject    *object,
                         guint       prop_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
   FigPlugin *plugin = FIG_PLUGIN(object);

   switch (prop_id) {
   case PROP_PATH:
      g_value_set_string (value, fig_plugin_get_path (plugin));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

static void
fig_plugin_set_property (GObject      *object,
                         guint         prop_id,
                         const GValue *value,
                         GParamSpec   *pspec)
{
   FigPlugin *plugin = FIG_PLUGIN(object);

   switch (prop_id) {
   case PROP_PATH:
      fig_plugin_set_path (plugin, g_value_get_string (value));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

static void
fig_plugin_class_init (FigPluginClass *klass)
{
   GObjectClass *object_class;

   object_class = G_OBJECT_CLASS (klass);
   object_class->finalize = fig_plugin_finalize;
   object_class->get_property = fig_plugin_get_property;
   object_class->set_property = fig_plugin_set_property;
   g_type_class_add_private (object_class, sizeof (FigPluginPrivate));

   gParamSpecs [PROP_PATH] =
      g_param_spec_string ("path",
                           _("Path"),
                           _("The path the module."),
                           NULL,
                           (G_PARAM_READWRITE |
                            G_PARAM_CONSTRUCT_ONLY |
                            G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_PATH,
                                    gParamSpecs [PROP_PATH]);
}

static void
fig_plugin_init (FigPlugin *plugin)
{
   plugin->priv = G_TYPE_INSTANCE_GET_PRIVATE (plugin,
                                               FIG_TYPE_PLUGIN,
                                               FigPluginPrivate);
}
