/* fig-command-info.c
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

#include "fig-command.h"
#include "fig-command-info.h"

G_DEFINE_TYPE (FigCommandInfo, fig_command_info, G_TYPE_INITIALLY_UNOWNED)

struct _FigCommandInfoPrivate
{
   FigCommandFactory  factory;
   gpointer           factory_data;
   GType              command_type;
   gchar             *description;
   gchar             *name;
};

enum
{
   PROP_0,
   PROP_COMMAND_TYPE,
   PROP_DESCRIPTION,
   PROP_NAME,
   LAST_PROP
};

static GHashTable *gCommandInfos;
static GParamSpec *gParamSpecs[LAST_PROP];

static FigCommand *
fig_command_info_real_create (FigCommandInfo *info)
{
   FigCommandInfoPrivate *priv;
   FigCommand *ret = NULL;

   g_return_val_if_fail (FIG_IS_COMMAND_INFO (info), NULL);

   priv = info->priv;

   if (priv->factory) {
      ret = priv->factory (info, priv->factory_data);
   }

   if (!ret && g_type_is_a (info->priv->command_type, FIG_TYPE_COMMAND)) {
      ret = g_object_new (info->priv->command_type, NULL);
   }

   return ret;
}

FigCommand *
fig_command_info_create (FigCommandInfo *info)
{
   g_return_val_if_fail (FIG_IS_COMMAND_INFO (info), NULL);

   return FIG_COMMAND_INFO_GET_CLASS (info)->create (info);
}

void
fig_command_info_set_factory (FigCommandInfo    *info,
                              FigCommandFactory  factory,
                              gpointer           user_data)
{
   g_return_if_fail (FIG_IS_COMMAND_INFO (info));

   info->priv->factory = factory;
   info->priv->factory_data = user_data;
}

const gchar *
fig_command_info_get_name (FigCommandInfo *info)
{
   g_return_val_if_fail (FIG_IS_COMMAND_INFO (info), NULL);
   return info->priv->name;
}

const gchar *
fig_command_info_get_description (FigCommandInfo *info)
{
   g_return_val_if_fail (FIG_IS_COMMAND_INFO (info), NULL);
   return info->priv->description;
}

GType
fig_command_info_get_command_type (FigCommandInfo *info)
{
   g_return_val_if_fail (FIG_IS_COMMAND_INFO (info), 0);
   return info->priv->command_type;
}

void
fig_command_info_register (FigCommandInfo *info)
{
   const gchar *name;

   g_return_if_fail (FIG_IS_COMMAND_INFO (info));

   name = fig_command_info_get_name (info);
   g_hash_table_insert (gCommandInfos, g_strdup (name), g_object_ref_sink (info));
}

FigCommandInfo *
fig_command_info_lookup (const gchar *name)
{
   g_return_val_if_fail (name, NULL);

   if (gCommandInfos) {
      return g_hash_table_lookup (gCommandInfos, name);
   }

   return NULL;
}

static void
fig_command_info_finalize (GObject *object)
{
   FigCommandInfoPrivate *priv;

   priv = FIG_COMMAND_INFO(object)->priv;

   g_clear_pointer (&priv->description, g_free);
   g_clear_pointer (&priv->name, g_free);

   G_OBJECT_CLASS (fig_command_info_parent_class)->finalize (object);
}

static void
fig_command_info_get_property (GObject    *object,
                               guint       prop_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
   FigCommandInfo *info = FIG_COMMAND_INFO(object);

   switch (prop_id) {
   case PROP_COMMAND_TYPE:
      g_value_set_gtype (value, info->priv->command_type);
      break;
   case PROP_DESCRIPTION:
      g_value_set_string (value, info->priv->description);
      break;
   case PROP_NAME:
      g_value_set_string (value, info->priv->name);
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

static void
fig_command_info_set_property (GObject      *object,
                               guint         prop_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
   FigCommandInfo *info = FIG_COMMAND_INFO(object);

   switch (prop_id) {
   case PROP_COMMAND_TYPE:
      info->priv->command_type = g_value_get_gtype (value);
      g_object_notify_by_pspec (object, pspec);
      break;
   case PROP_DESCRIPTION:
      g_free (info->priv->description);
      info->priv->description = g_value_dup_string (value);
      g_object_notify_by_pspec (object, pspec);
      break;
   case PROP_NAME:
      g_free (info->priv->name);
      info->priv->name = g_value_dup_string (value);
      g_object_notify_by_pspec (object, pspec);
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
   }
}

static void
fig_command_info_class_init (FigCommandInfoClass *klass)
{
   GObjectClass *object_class;

   object_class = G_OBJECT_CLASS(klass);
   object_class->finalize = fig_command_info_finalize;
   object_class->get_property = fig_command_info_get_property;
   object_class->set_property = fig_command_info_set_property;
   g_type_class_add_private(object_class, sizeof(FigCommandInfoPrivate));

   klass->create = fig_command_info_real_create;

   gParamSpecs [PROP_COMMAND_TYPE] =
      g_param_spec_gtype ("command-type",
                          _("Command Type"),
                          _("The GType of the command."),
                          FIG_TYPE_COMMAND,
                          (G_PARAM_READWRITE |
                           G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_COMMAND_TYPE,
                                    gParamSpecs [PROP_COMMAND_TYPE]);

   gParamSpecs [PROP_DESCRIPTION] =
      g_param_spec_string ("description",
                           _("Description"),
                           _("The command description."),
                           NULL,
                           (G_PARAM_READWRITE |
                            G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_DESCRIPTION,
                                    gParamSpecs [PROP_DESCRIPTION]);

   gParamSpecs [PROP_NAME] =
      g_param_spec_string ("name",
                           _("Name"),
                           _("The command name."),
                           NULL,
                           (G_PARAM_READWRITE |
                            G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_NAME,
                                    gParamSpecs [PROP_NAME]);

   gCommandInfos = g_hash_table_new_full (g_str_hash,
                                          g_str_equal,
                                          g_free,
                                          g_object_unref);
}

static void
fig_command_info_init (FigCommandInfo *info)
{
   info->priv = G_TYPE_INSTANCE_GET_PRIVATE (info,
                                             FIG_TYPE_COMMAND_INFO,
                                             FigCommandInfoPrivate);
}
