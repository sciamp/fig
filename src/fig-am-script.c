/* fig-am-script.c
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

#include <glib/gi18n.h>

#include "fig-am-script.h"

struct _FigAmScriptPrivate
{
   GFile *file;
};

enum
{
   PROP_0,
   PROP_FILE,
   LAST_PROP
};

G_DEFINE_TYPE_WITH_PRIVATE (FigAmScript, fig_am_script, G_TYPE_OBJECT)

static GParamSpec *gParamSpecs [LAST_PROP];

FigAmScript *
fig_am_script_new (GFile *file)
{
   g_return_val_if_fail (G_IS_FILE (file), NULL);

   return g_object_new (FIG_TYPE_AM_SCRIPT, "file", file, NULL);
}

GFile *
fig_am_script_get_file (FigAmScript *script)
{
   g_return_val_if_fail (FIG_IS_AM_SCRIPT (script), NULL);
   return script->priv->file;
}

void
fig_am_script_set_file (FigAmScript *script,
                        GFile       *file)
{
   g_return_if_fail (FIG_IS_AM_SCRIPT (script));

   g_clear_object (&script->priv->file);
   script->priv->file = file ? g_object_ref (file) : NULL;
   g_object_notify_by_pspec (G_OBJECT (script), gParamSpecs [PROP_FILE]);
}

static void
fig_am_script_finalize (GObject *object)
{
   FigAmScriptPrivate *priv;

   priv = FIG_AM_SCRIPT (object)->priv;

   g_clear_object (&priv->file);

   G_OBJECT_CLASS (fig_am_script_parent_class)->finalize (object);
}

static void
fig_am_script_get_property (GObject    *object,
                            guint       prop_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
   FigAmScript *script = FIG_AM_SCRIPT (object);

   switch (prop_id) {
   case PROP_FILE:
      g_value_set_object (value, fig_am_script_get_file (script));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
   }
}

static void
fig_am_script_set_property (GObject      *object,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
   FigAmScript *script = FIG_AM_SCRIPT (object);

   switch (prop_id) {
   case PROP_FILE:
      fig_am_script_set_file (script, g_value_get_object (value));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
   }
}

static void
fig_am_script_class_init (FigAmScriptClass *klass)
{
   GObjectClass *object_class;

   object_class = G_OBJECT_CLASS (klass);
   object_class->finalize = fig_am_script_finalize;
   object_class->get_property = fig_am_script_get_property;
   object_class->set_property = fig_am_script_set_property;

   gParamSpecs [PROP_FILE] =
      g_param_spec_object ("file",
                           _("File"),
                           _("File"),
                           G_TYPE_FILE,
                           (G_PARAM_READWRITE |
                            G_PARAM_CONSTRUCT_ONLY |
                            G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_FILE,
                                    gParamSpecs [PROP_FILE]);
}

static void
fig_am_script_init (FigAmScript *script)
{
   script->priv = fig_am_script_get_instance_private (script);
}
