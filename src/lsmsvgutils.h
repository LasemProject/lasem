/*
 * Copyright © 2009 Emmanuel Pacaud
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */


#ifndef GSGV_UTILS_H
#define GSGV_UTILS_H

#include <lsmsvg.h>

G_BEGIN_DECLS

gboolean lsm_svg_str_parse_double 	(char **str, double *x);
gboolean lsm_svg_str_parse_double_list 	(char **str, unsigned int n_values, double *values);

static inline void
lsm_svg_str_skip_spaces (char **str)
{
	while (g_ascii_isspace (**str))
		(*str)++;
}

static inline void
lsm_svg_str_skip_char (char **str, char c)
{
	while (**str == c)
		(*str)++;
}

static inline void
lsm_svg_str_skip_comma_and_spaces (char **str)
{
	while (g_ascii_isspace (**str) || **str == ',')
		(*str)++;
}

static inline void
lsm_svg_str_skip_semicolon_and_spaces (char **str)
{
	while (g_ascii_isspace (**str) || **str == ';')
		(*str)++;
}

static inline void
lsm_svg_str_skip_colon_and_spaces (char **str)
{
	while (g_ascii_isspace (**str) || **str == ':')
		(*str)++;
}

G_END_DECLS

#endif
