/* gmathmlenums.c
 *
 * Copyright © 2007-2008  Emmanuel Pacaud
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

#ifndef GSVG_ATTRIBUTES_H
#define GSVG_ATTRIBUTES_H

#include <gdomattributes.h>
#include <gsvg.h>
#include <gsvgenums.h>

G_BEGIN_DECLS

typedef struct {
	double red;
	double green;
	double blue;
	double alpha;
} GSvgColor;

typedef struct {
	double value;
	GSvgLengthType type;
} GSvgLength;

typedef struct {
	GSvgLength base;
	GSvgLength animated;
} GSvgAnimatedLength;

typedef struct {
	GSvgPaintType type;
	char *uri;
	GSvgColor color;
} GSvgPaint;

typedef struct {
	GDomAttribute attr;
	GSvgLength length;
} GSvgLengthAttribute;

typedef struct {
	GDomAttribute attr;
	GSvgAnimatedLength length;
} GSvgAnimatedLengthAttribute;

typedef struct {
	GDomAttribute attr;
	GSvgPaint paint;
} GSvgPaintAttribute;

void		gsvg_length_attribute_parse 		(GSvgLengthAttribute *attribute,
							 GSvgLength *default_value);
void 		gsvg_animated_length_attribute_parse 	(GSvgAnimatedLengthAttribute *attribute,
							 GSvgLength *default_value);

G_END_DECLS

#endif
