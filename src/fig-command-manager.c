/* fig-command-manager.c
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

#include "fig-command-manager.h"
#include "fig-init-command.h"

G_DEFINE_TYPE (FigCommandManager, fig_command_manager, G_TYPE_OBJECT)

struct _FigCommandManagerPrivate
{
   GHashTable *command_infos;
};

FigCommandManager *
fig_command_manager_get_default (void)
{
   static FigCommandManager *instance;

   if (g_once_init_enter (&instance)) {
      FigCommandManager *tmp = g_object_new (FIG_TYPE_COMMAND_MANAGER, NULL);
      g_once_init_leave (&instance, tmp);
   }

   return instance;
}

/**
 * fig_command_manager_get_commands:
 * @manager: A #FigCommandManager.
 *
 * Fetches a copy of all FigCommandInfo and returns a newly allocated
 * list.
 *
 * Returns: (transfer full) (element-type FigCommandInfo*): A list of
 *    #FigCommandInfo that should be freed by releasing each element
 *    with fig_command_info_free() and then g_list_free().
 */
GList *
fig_command_manager_get_commands (FigCommandManager *manager)
{
   GList *ret;
   GList *iter;

   g_return_val_if_fail (FIG_IS_COMMAND_MANAGER (manager), NULL);

   ret = g_hash_table_get_values (manager->priv->command_infos);

   for (iter = ret; iter; iter = iter->next) {
      iter->data = fig_command_info_copy (iter->data);
   }

   return ret;
}

/**
 * fig_command_manager_lookup:
 * @manager: A #FigCommandManager.
 * @name: The name of the command.
 *
 * Looks up and copies a #FigCommandInfo structure. Free the result
 * with fig_command_info_free().
 *
 * Returns: (transfer full): A #FigCommandInfo.
 */
FigCommandInfo *
fig_command_manager_lookup (FigCommandManager *manager,
                            const gchar       *name)
{
   FigCommandInfo *ret;

   g_return_val_if_fail (FIG_IS_COMMAND_MANAGER (manager), NULL);

   ret = g_hash_table_lookup (manager->priv->command_infos, name);

   return ret ? fig_command_info_copy (ret) : NULL;
}

void
fig_command_manager_register (FigCommandManager    *manager,
                              const FigCommandInfo *command_info)
{
   FigCommandInfo *copy;

   g_return_if_fail (FIG_IS_COMMAND_MANAGER (manager));
   g_return_if_fail (command_info);

   copy = fig_command_info_copy (command_info);
   g_hash_table_insert (manager->priv->command_infos,
                        copy->name, copy);
}

static void
fig_command_manager_register_builtins (FigCommandManager *manager)
{
   const FigCommandInfo builtins [] = {
      { "init", "Initialize a new project.", FIG_TYPE_INIT_COMMAND },
   };
   int i;

   g_assert (FIG_IS_COMMAND_MANAGER (manager));

   for (i = 0; i < G_N_ELEMENTS (builtins); i++) {
      fig_command_manager_register (manager, &builtins [i]);
   }
}

static void
fig_command_manager_finalize (GObject *object)
{
   FigCommandManagerPrivate *priv;

   priv = FIG_COMMAND_MANAGER(object)->priv;

   g_clear_pointer (&priv->command_infos, g_hash_table_unref);

   G_OBJECT_CLASS (fig_command_manager_parent_class)->finalize (object);
}

static void
fig_command_manager_class_init (FigCommandManagerClass *klass)
{
   GObjectClass *object_class;

   object_class = G_OBJECT_CLASS(klass);
   object_class->finalize = fig_command_manager_finalize;
   g_type_class_add_private (object_class, sizeof (FigCommandManagerPrivate));
}

static void
fig_command_manager_init (FigCommandManager *manager)
{

   manager->priv = G_TYPE_INSTANCE_GET_PRIVATE (manager,
                                                FIG_TYPE_COMMAND_MANAGER,
                                                FigCommandManagerPrivate);

   manager->priv->command_infos =
      g_hash_table_new_full (g_str_hash,
                             g_str_equal,
                             NULL,
                             (GDestroyNotify)fig_command_info_free);

   fig_command_manager_register_builtins (manager);
}
