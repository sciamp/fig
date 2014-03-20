/* fig-target-type.h
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

#ifndef FIG_TARGET_TYPE_H
#define FIG_TARGET_TYPE_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum
{
   FIG_TARGET_LIBRARY = 1,
   FIG_TARGET_PROGRAM = 2,
} FigTargetType;

GType fig_target_type_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* FIG_TARGET_TYPE_H */
