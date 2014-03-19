/* fig-cli.c
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

#include "fig-cli.h"

struct _FigCliPrivate
{
   GOutputStream *stderr_stream;
   GOutputStream *stdout_stream;
};

enum
{
   PROP_0,
   PROP_STDERR_STREAM,
   PROP_STDOUT_STREAM,
   LAST_PROP
};

G_DEFINE_TYPE_WITH_PRIVATE (FigCli, fig_cli, G_TYPE_OBJECT)

static GParamSpec *gParamSpecs [LAST_PROP];

GOutputStream *
fig_cli_get_stderr_stream (FigCli *cli)
{
   g_return_val_if_fail (FIG_IS_CLI (cli), NULL);

   return cli->priv->stderr_stream;
}

void
fig_cli_set_stderr_stream (FigCli        *cli,
                           GOutputStream *stderr_stream)
{
   FigCliPrivate *priv;

   g_return_if_fail (FIG_IS_CLI (cli));
   g_return_if_fail (!stderr_stream || G_IS_OUTPUT_STREAM (stderr_stream));

   priv = cli->priv;

   if (stderr_stream != priv->stderr_stream) {
      g_clear_object (&priv->stderr_stream);
      if (stderr_stream) {
         priv->stderr_stream = g_object_ref (stderr_stream);
      }
      g_object_notify_by_pspec (G_OBJECT (cli),
                                gParamSpecs [PROP_STDERR_STREAM]);
   }
}

GOutputStream *
fig_cli_get_stdout_stream (FigCli *cli)
{
   g_return_val_if_fail (FIG_IS_CLI (cli), NULL);

   return cli->priv->stderr_stream;
}

void
fig_cli_set_stdout_stream (FigCli        *cli,
                           GOutputStream *stdout_stream)
{
   FigCliPrivate *priv;

   g_return_if_fail (FIG_IS_CLI (cli));
   g_return_if_fail (!stdout_stream || G_IS_OUTPUT_STREAM (stdout_stream));

   priv = cli->priv;

   if (stdout_stream != priv->stdout_stream) {
      g_clear_object (&priv->stdout_stream);
      if (stdout_stream) {
         priv->stdout_stream = g_object_ref (stdout_stream);
      }
      g_object_notify_by_pspec (G_OBJECT (cli),
                                gParamSpecs [PROP_STDOUT_STREAM]);
   }
}

static void
fig_cli_finalize (GObject *object)
{
   FigCliPrivate *priv;

   priv = FIG_CLI (object)->priv;

   g_clear_object (&priv->stderr_stream);
   g_clear_object (&priv->stdout_stream);

   G_OBJECT_CLASS (fig_cli_parent_class)->finalize (object);
}

static void
fig_cli_get_property (GObject    *object,
                      guint       prop_id,
                      GValue     *value,
                      GParamSpec *pspec)
{
   FigCli *cli = FIG_CLI (object);

   switch (prop_id) {
   case PROP_STDERR_STREAM:
      g_value_set_object (value, fig_cli_get_stderr_stream (cli));
      break;
   case PROP_STDOUT_STREAM:
      g_value_set_object (value, fig_cli_get_stdout_stream (cli));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
   }
}

static void
fig_cli_set_property (GObject      *object,
                      guint         prop_id,
                      const GValue *value,
                      GParamSpec   *pspec)
{
   FigCli *cli = FIG_CLI (object);

   switch (prop_id) {
   case PROP_STDERR_STREAM:
      fig_cli_set_stderr_stream (cli, g_value_get_object (value));
      break;
   case PROP_STDOUT_STREAM:
      fig_cli_set_stdout_stream (cli, g_value_get_object (value));
      break;
   default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
   }
}

static void
fig_cli_class_init (FigCliClass *klass)
{
   GObjectClass *object_class;

   object_class = G_OBJECT_CLASS (klass);
   object_class->finalize = fig_cli_finalize;
   object_class->get_property = fig_cli_get_property;
   object_class->set_property = fig_cli_set_property;

   gParamSpecs [PROP_STDERR_STREAM] =
      g_param_spec_object ("stderr-stream",
                           _("Stderr Stream"),
                           _("The stream to write stderr content to."),
                           G_TYPE_OUTPUT_STREAM,
                           (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_STDERR_STREAM,
                                    gParamSpecs [PROP_STDERR_STREAM]);

   gParamSpecs [PROP_STDOUT_STREAM] =
      g_param_spec_object ("stdout-stream",
                           _("Stdout Stream"),
                           _("The stream to write stdout content to."),
                           G_TYPE_OUTPUT_STREAM,
                           (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
   g_object_class_install_property (object_class, PROP_STDOUT_STREAM,
                                    gParamSpecs [PROP_STDOUT_STREAM]);
}

static void
fig_cli_init (FigCli *cli)
{
   cli->priv = fig_cli_get_instance_private (cli);
}
