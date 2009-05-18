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

typedef struct _LsmSvgDocument LsmSvgDocument;
typedef struct _LsmSvgElement LsmSvgElement;
typedef struct _LsmSvgGraphic LsmSvgGraphic;
typedef struct _LsmSvgClipPathElement LsmSvgClipPathElement;
typedef struct _LsmSvgSvgElement LsmSvgSvgElement;
typedef struct _LsmSvgGElement LsmSvgGElement;
typedef struct _LsmSvgDefsElement LsmSvgDefsElement;
typedef struct _LsmSvgUseElement LsmSvgUseElement;
typedef struct _LsmSvgImageElement LsmSvgImageElement;
typedef struct _LsmSvgSymbolElement LsmSvgSymbolElement;
typedef struct _LsmSvgRectElement LsmSvgRectElement;
typedef struct _LsmSvgCircleElement LsmSvgCircleElement;
typedef struct _LsmSvgEllipseElement LsmSvgEllipseElement;
typedef struct _LsmSvgLineElement LsmSvgLineElement;
typedef struct _LsmSvgPolylineElement LsmSvgPolylineElement;
typedef struct _LsmSvgPolygonElement LsmSvgPolygonElement;
typedef struct _LsmSvgPathElement LsmSvgPathElement;
typedef struct _LsmSvgTextElement LsmSvgTextElement;
typedef struct _LsmSvgGradientElement LsmSvgGradientElement;
typedef struct _LsmSvgLinearGradientElement LsmSvgLinearGradientElement;
typedef struct _LsmSvgRadialGradientElement LsmSvgRadialGradientElement;
typedef struct _LsmSvgStopElement LsmSvgStopElement;
typedef struct _LsmSvgPatternElement LsmSvgPatternElement;

typedef struct _LsmSvgView LsmSvgView;
typedef struct _LsmSvgStyle LsmSvgStyle;

G_END_DECLS

#endif
