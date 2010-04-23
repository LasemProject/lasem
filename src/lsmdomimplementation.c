/*
 * Copyright © 2007-2009 Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author:
 * 	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmdomimplementation.h>
#include <string.h>

LsmDomDocument *
lsm_dom_implementation_create_document (const char *qualified_name)
{
	g_return_val_if_fail (qualified_name != NULL, NULL);

	if (strcmp (qualified_name, "svg") == 0)
		return LSM_DOM_DOCUMENT (lsm_svg_document_new ());
	else if (strcmp (qualified_name, "math") == 0)
		return LSM_DOM_DOCUMENT (lsm_mathml_document_new ());

	return NULL;
}
