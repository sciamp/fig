/* fig-template.c
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

#include "fig-template.h"

G_DEFINE_TYPE (FigTemplate, fig_template, G_TYPE_OBJECT)

struct _FigTemplatePrivate
{
   gchar *name;
};

enum
{
   PROP_0,
   PROP_NAME,
   LAST_PROP
};

static GParamSpec *gParamSpecs[LAST_PROP];

FigTemplate *
fig_template_new (const gchar *name)
{
   return g_object_new (FIG_TYPE_TEMPLATE, "name", name, NULL);
}

const gchar *
fig_template_get_name (FigTemplate *template)
{
   g_return_val_if_fail (FIG_IS_TEMPLATE (template), FALSE);

   return template->priv->name;
}

static void
fig_template_set_name (FigTemplate *template,
                       const gchar *name)
{
   g_return_if_fail (FIG_IS_TEMPLATE (template));

   g_free (template->priv->name);
   template->priv->name = g_strdup (name);
   g_object_notify_by_pspec (G_OBJECT (template), gParamSpecs [PROP_NAME]);
}

gboolean
fig_template_render (FigTemplate  *template,
                     GFile        *file,
                     GError      **error)
{
   FigTemplatePrivate *priv;
   const gchar *dir = FIG_TEMPLATE_DIR;
   const gchar *str;
   gboolean ret;
   GFile *src;
   gchar *path;

   g_return_val_if_fail (FIG_IS_TEMPLATE (template), FALSE);
   g_return_val_if_fail (G_IS_FILE (file), FALSE);

   priv = template->priv;

   /*
    * Just copy the file now until we get our template stuff figured out.
    */

   if ((str = g_getenv ("FIG_TEMPLATE_DIR"))) {
      if (g_file_test (str, G_FILE_TEST_IS_DIR)) {
         dir = str;
      }
   }

   path = g_build_filename (dir, priv->name, NULL);
   src = g_file_new_for_path (path);
   g_free (path);

   ret = g_file_copy (src, file, G_FILE_COPY_NONE, NULL, NULL, NULL, error);

   g_object_unref (src);

   return ret;
}

static void
fig_template_finalize (GObject *object)
{
   FigTemplatePrivate *priv;

   priv = FIG_TEMPLATE(object)->priv;

   g_clear_pointer (&priv->name, g_free);

   G_OBJECT_CLASS (fig_template_parent_class)->finalize (object);
}

static void
fig_template_get_property (GObject    *object,
                           guint       prop_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
   FigTemplate *template = FIG_TEMPLATE(object);

   switch (prop_id) {
   case PROP_NAME:
      g_value_set_string (value, fig_template_get_name (template));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

static void
fig_template_set_property (GObject      *object,
                           guint         prop_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
   FigTemplate *template = FIG_TEMPLATE(object);

   switch (prop_id) {
   case PROP_NAME:
      fig_template_set_name (template, g_value_get_string (value));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

static void
fig_template_class_init (FigTemplateClass *klass)
{
   GObjectClass *object_class;

   object_class = G_OBJECT_CLASS(klass);
   object_class->finalize = fig_template_finalize;
   object_class->get_property = fig_template_get_property;
   object_class->set_property = fig_template_set_property;
   g_type_class_add_private(object_class, sizeof(FigTemplatePrivate));

   gParamSpecs [PROP_NAME] =
      g_param_spec_string ("name",
                           _("Name"),
                           _("The name of the template."),
                           NULL,
                           (G_PARAM_READWRITE |
                            G_PARAM_CONSTRUCT_ONLY |
                            G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_NAME,
                                    gParamSpecs[PROP_NAME]);
}

static void
fig_template_init (FigTemplate *template)
{
   template->priv = G_TYPE_INSTANCE_GET_PRIVATE (template,
                                                 FIG_TYPE_TEMPLATE,
                                                 FigTemplatePrivate);
}
