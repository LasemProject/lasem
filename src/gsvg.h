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

#ifndef GSVG_H
#define GSVG_H

#include <gdom.h>

G_BEGIN_DECLS

typedef struct _GSvgDocument GSvgDocument;
typedef struct _GSvgElement GSvgElement;
typedef struct _GSvgGraphic GSvgGraphic;
typedef struct _GSvgSvgElement GSvgSvgElement;
typedef struct _GSvgGElement GSvgGElement;
typedef struct _GSvgRectElement GSvgRectElement;
typedef struct _GSvgCircleElement GSvgCircleElement;
typedef struct _GSvgEllipseElement GSvgEllipseElement;
typedef struct _GSvgPathElement GSvgPathElement;

typedef struct _GSvgView GSvgView;
typedef struct _GSvgStyle GSvgStyle;

G_END_DECLS

#endif
