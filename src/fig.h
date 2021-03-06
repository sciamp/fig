/* fig.h
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

#ifndef FIG_H
#define FIG_H

#include <glib.h>

G_BEGIN_DECLS

#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "fig"

#define FIG_INSIDE
#include "fig-cli.h"
#include "fig-command.h"
#include "fig-command-info.h"
#include "fig-command-manager.h"
#include "fig-init-command.h"
#include "fig-update-authors-command.h"
#include "fig-template.h"
#undef FIG_INSIDE

G_END_DECLS

#endif /* FIG_H */
