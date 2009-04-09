/* lsmmathml.h
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

#ifndef LSM_MATHML_H
#define LSM_MATHML_H

#include <lsmdom.h>

G_BEGIN_DECLS

typedef struct _GMathmlDocument LsmMathmlDocument;
typedef struct _GMathmlElement LsmMathmlElement;
typedef struct _GMathmlPhantomElement LsmMathmlPhantomElement;
typedef struct _GMathmlTableElement LsmMathmlTableElement;
typedef struct _GMathmlTableRowElement LsmMathmlTableRowElement;
typedef struct _GMathmlTableCellElement LsmMathmlTableCellElement;
typedef struct _GMathmlSpaceElement LsmMathmlSpaceElement;
typedef struct _GMathmlFractionElement LsmMathmlFractionElement;
typedef struct _GMathmlUnderOverElement LsmMathmlUnderOverElement;
typedef struct _GMathmlMathElement LsmMathmlMathElement;
typedef struct _GMathmlRadicalElement LsmMathmlRadicalElement;
typedef struct _GMathmlScriptElement LsmMathmlScriptElement;
typedef struct _GMathmlPresentationToken LsmMathmlPresentationToken;
typedef struct _GMathmlNumberElement LsmMathmlNumberElement;
typedef struct _GMathmlOperatorElement LsmMathmlOperatorElement;
typedef struct _GMathmlStringElement LsmMathmlStringElement;
typedef struct _GMathmlPresentationContainer LsmMathmlPresentationContainer;
typedef struct _GMathmlStyleElement LsmMathmlStyleElement;
typedef struct _GMathmlRowElement LsmMathmlRowElement;
typedef struct _GMathmlFencedElement LsmMathmlFencedElement;
typedef struct _GMathmlPaddedElement LsmMathmlPaddedElement;
typedef struct _GMathmlErrorElement LsmMathmlErrorElement;
typedef struct _GMathmlActionElement LsmMathmlActionElement;
typedef struct _GMathmlAlignMarkElement LsmMathmlAlignMarkElement;
typedef struct _GMathmlAlignGroupElement LsmMathmlAlignGroupElement;

typedef struct _GMathmlView LsmMathmlView;

typedef struct _GMathmlStyle LsmMathmlStyle;

G_END_DECLS

#endif
