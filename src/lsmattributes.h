/*
 * Copyright © 2007-2009 Emmanuel Pacaud
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

#ifndef LSM_ATTRIBUTES_H
#define LSM_ATTRIBUTES_H

#include <lsm.h>
#include <lsmtraits.h>

G_BEGIN_DECLS

typedef struct {
	char *value;
} LsmAttribute;

gboolean	lsm_attribute_is_defined			(const LsmAttribute *attribute);

typedef struct {
	char const *		name;
	int 			attribute_offset;
	const LsmTraitClass *	trait_class;
	const void *		trait_default;
} LsmAttributeInfos;

typedef struct _LsmAttributeManager LsmAttributeManager;

LsmAttributeManager *	lsm_attribute_manager_new 		(unsigned int n_attributes,
								 const LsmAttributeInfos *attribute_infos);
void			lsm_attribute_manager_free		(LsmAttributeManager *manager);
void 			lsm_attribute_manager_add_attributes 	(LsmAttributeManager *manager,
								 unsigned int n_attributes,
								 const LsmAttributeInfos *attribute_infos);
LsmAttributeManager * 	lsm_attribute_manager_duplicate 	(const LsmAttributeManager *origin);

gboolean	lsm_attribute_manager_set_attribute		(LsmAttributeManager *manager,
								 void *instance,
								 char const *name,
								 char const *value);
char const *	lsm_attribute_manager_get_attribute		(LsmAttributeManager *manager,
								 void *instance,
								 char const *name);
void		lsm_attribute_manager_clean_attributes 		(LsmAttributeManager *manager,
								 void *instance);
char *		lsm_attribute_manager_serialize			(LsmAttributeManager *manager,
								 void *instance);

G_END_DECLS

#endif
