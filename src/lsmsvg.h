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

#ifndef LSM_SVG_H
#define LSM_SVG_H

#include <lsmdom.h>

G_BEGIN_DECLS

typedef struct _GSvgDocument LsmSvgDocument;
typedef struct _GSvgElement LsmSvgElement;
typedef struct _GSvgGraphic LsmSvgGraphic;
typedef struct _GSvgSvgElement LsmSvgSvgElement;
typedef struct _GSvgGElement LsmSvgGElement;
typedef struct _GSvgDefsElement LsmSvgDefsElement;
typedef struct _GSvgUseElement LsmSvgUseElement;
typedef struct _GSvgSymbolElement LsmSvgSymbolElement;
typedef struct _GSvgRectElement LsmSvgRectElement;
typedef struct _GSvgCircleElement LsmSvgCircleElement;
typedef struct _GSvgEllipseElement LsmSvgEllipseElement;
typedef struct _GSvgLineElement LsmSvgLineElement;
typedef struct _GSvgPolylineElement LsmSvgPolylineElement;
typedef struct _GSvgPolygonElement LsmSvgPolygonElement;
typedef struct _GSvgPathElement LsmSvgPathElement;
typedef struct _GSvgTextElement LsmSvgTextElement;
typedef struct _GSvgGradientElement LsmSvgGradientElement;
typedef struct _GSvgLinearGradientElement LsmSvgLinearGradientElement;
typedef struct _GSvgRadialGradientElement LsmSvgRadialGradientElement;
typedef struct _GSvgStopElement LsmSvgStopElement;
typedef struct _GSvgPatternElement LsmSvgPatternElement;

typedef struct _GSvgView LsmSvgView;
typedef struct _GSvgStyle LsmSvgStyle;

G_END_DECLS

#endif
