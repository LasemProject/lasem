/* Lasem
 *
 * Copyright © 2007-2010 Emmanuel Pacaud
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

/**
 * SECTION:lsmcss
 * @short_description: CSS support class
 */

#include <lsmcss.h>
#include <lsmstr.h>
#include <gio/gio.h>
#include <stdio.h>

static GObjectClass *parent_class = NULL;

enum {
	LSM_CSS_PROPERTY_0,
	LSM_CSS_PROPERTY_DATA
} LsmCssProperties;

struct _LsmCssPrivate {
	int dummy;
};

/* LsmCss implementation */

typedef struct {
	char *name;
	char *value;
} LsmCssAttribute;

typedef struct {
	GSList *terms;
} LsmCssSelector;

typedef struct {
	GSList *selectors;
	GSList *attributes;
} LsmCssRule;

LsmCssAttribute *	lsm_css_attribute_new 		(const char *name, const char *value);
void 			lsm_css_attribute_free 		(LsmCssAttribute *attribute);
LsmCssSelector * 	lsm_css_selector_new 		(void);
void 			lsm_css_selector_free 		(LsmCssSelector *selector);
LsmCssRule * 		lsm_css_rule_new 		(void);
void 			lsm_css_rule_free 		(LsmCssRule *rule);
void 			lsm_css_rule_add_selector 	(LsmCssRule *rule, LsmCssSelector *selector);
void 			lsm_css_rule_add_attribute 	(LsmCssRule *rule, const char *name, const char *value);

LsmCssAttribute *
lsm_css_attribute_new (const char *name, const char *value)
{
	LsmCssAttribute *attribute;

	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (value != NULL, NULL);

	attribute = g_new0 (LsmCssAttribute, 1);
	attribute->name = g_strdup (name);
	attribute->value = g_strdup (value);

	return attribute;
}

void
lsm_css_attribute_free (LsmCssAttribute *attribute)
{
	g_return_if_fail (attribute != NULL);

	g_free (attribute->name);
	g_free (attribute->value);
	g_free (attribute);
}

LsmCssSelector *
lsm_css_selector_new (void)
{
	LsmCssSelector *selector;

	selector = g_new0 (LsmCssSelector, 1);
	return selector;
}

void
lsm_css_selector_free (LsmCssSelector *selector)
{
	g_return_if_fail (selector != NULL);

	g_slist_free_full (selector->terms, g_free);
	g_free (selector);
}

LsmCssRule *
lsm_css_rule_new (void)
{
	LsmCssRule *rule;

	rule = g_new0 (LsmCssRule, 1);
	
	return rule;
}

void
lsm_css_rule_free (LsmCssRule *rule)
{
	g_return_if_fail (rule != NULL);

	g_slist_free_full (rule->selectors, (GDestroyNotify) lsm_css_selector_free);
	g_slist_free_full (rule->attributes, (GDestroyNotify) lsm_css_attribute_free);
	g_free (rule);
}

void
lsm_css_rule_add_selector (LsmCssRule *rule, LsmCssSelector *selector)
{
	g_return_if_fail (rule != NULL);
	g_return_if_fail (selector != NULL);

	rule->selectors = g_slist_prepend (rule->selectors, selector);
}

void
lsm_css_rule_add_attribute (LsmCssRule *rule, const char *name, const char *value)
{
	LsmCssAttribute *attribute;

	g_return_if_fail (rule != NULL);

	attribute = lsm_css_attribute_new (name, value);
	g_return_if_fail (attribute != NULL);

	rule->attributes = g_slist_prepend (rule->attributes, attribute);
}

#define LSM_CSS_PARSE_ERROR lsm_css_parse_error_quark ()

static GQuark
lsm_css_parse_error_quark (void)
{
	return g_quark_from_static_string ("lsm-css-parse-error-quark");
}

static void
_skip_comment_space_line (char **str)
{
	char *ptr;

	do {
		ptr = *str;

		lsm_str_skip_comment (str);
		lsm_str_skip_spaces (str);
		lsm_str_skip_line_separators (str);
	} while (ptr != *str);
}

static char *
_parse_element (char **str)
{
	char *start = *str;

	if (**str == '.' ||
	    **str == '#' ||
	    g_ascii_isalnum (**str))
		(*str)++;

	while (g_ascii_isalnum (**str) ||
	       **str == '-' ||
	       **str == '_' ||
	       **str == ':')
		(*str)++;

	if (*str == start)
		return NULL;

	return g_strndup (start, *str - start);
}

static char *
_parse_attribute_name (char **str)
{
	char *start = *str;

	if (g_ascii_isalnum (**str))
		(*str)++;

	while (g_ascii_isalnum (**str) ||
	       **str == '-' ||
	       **str == '_')
		(*str)++;

	if (*str == start)
		return NULL;

	return g_strndup (start, *str - start);
}

static char *
_parse_attribute_value (char **str)
{
	char *start = *str;
	char *value;

	while (**str != ';' &&
	       **str != '\0')
		(*str)++;

	if (*str == start)
		return NULL;

	value = g_strndup (start, *str - start);

	if (**str == ';')
		(*str)++;

	return value;
}

static gboolean
lsm_css_parse_buffer (LsmCss *css, const char *buffer, gsize size, GError **error)
{
	char *str = (char *) buffer;
	GString *selector;
	GError *local_error = NULL;

	g_return_val_if_fail (LSM_IS_CSS (css), FALSE);

	if (buffer == NULL)
		return TRUE;

	selector = g_string_new ("");

	do {
		do {
			g_string_assign (selector, "");

			do {
				char *element;

				_skip_comment_space_line (&str);
				element = _parse_element (&str);
				_skip_comment_space_line (&str);

				if (element != NULL)
					g_string_append (selector, element);

				g_free (element);
			} while (*str != ',' &&
				 *str != '{' &&
				 *str != '\0' &&
				 local_error == NULL);

			if (selector->len == 0 && *str != '\0') {
				local_error = g_error_new (LSM_CSS_PARSE_ERROR, 0, "Expected selector definition");
			} else if (*str != '\0')
				printf ("selector = %s\n", selector->str);

		} while (*str != '{' && 
			 *str != '\0' &&
			 local_error == NULL);

		if (*str == '{') {
			str++;

			do {
				char *attribute_name;

				_skip_comment_space_line (&str);
				attribute_name = _parse_attribute_name (&str);
				_skip_comment_space_line (&str);

				printf ("attr_name = '%s'\n", attribute_name);

				if (*str != ':') {
					local_error = g_error_new (LSM_CSS_PARSE_ERROR, 0, "Missing colon");
				} else {
					char *attribute_value;

					str++;
					_skip_comment_space_line (&str);
					attribute_value = _parse_attribute_value (&str);
					_skip_comment_space_line (&str);

					printf ("attr_value = '%s'\n", attribute_value);

					g_free (attribute_value);
				}

				g_free (attribute_name);
			} while (*str != '}' &&
				 *str != '\0' &&
				 local_error == NULL);

			if (*str != '}') {
				local_error = g_error_new (LSM_CSS_PARSE_ERROR, 0, "Missing closing curly brackets");
			} else
				str++;
		}

	} while (*str != '\0' && local_error == NULL);

	g_string_free (selector, TRUE);

	if (local_error != NULL) {
		printf ("error: %s\n", local_error->message);
		g_propagate_error (error, local_error);
	}

	return local_error == NULL;
}

LsmCss *
lsm_css_new_from_memory  (const void *buffer, gssize size, GError **error)
{
	LsmCss *css;

	css = g_object_new (LSM_TYPE_CSS, "data", buffer, NULL);

	return css;
}

static LsmCss *
lsm_css_new_from_file  (GFile *file, GError **error)
{
	LsmCss *css;
	gsize size = 0;
	char *contents = NULL;

	if (!g_file_load_contents (file, NULL, &contents, &size, NULL, error))
		return NULL;

	css = lsm_css_new_from_memory (contents, size, error);

	g_free (contents);

	return css;
}

LsmCss *
lsm_css_new_from_path (const char *path, GError **error)
{
	LsmCss *css;
	GFile *file;

	g_return_val_if_fail (path != NULL, NULL);

	file = g_file_new_for_path (path);

	css = lsm_css_new_from_file (file, error);

	g_object_unref (file);

	return css;
}

LsmCss *
lsm_css_new_from_url (const char *url, GError **error)
{
	LsmCss *css;
	GFile *file;

	g_return_val_if_fail (url != NULL, NULL);

	file = g_file_new_for_uri (url);

	css = lsm_css_new_from_file (file, error);

	g_object_unref (file);

	return css;
}

static void
_set_property (GObject * object, guint prop_id,
	       const GValue * value, GParamSpec * pspec)
{
	LsmCss *css = LSM_CSS (object);

	switch (prop_id) {
		case LSM_CSS_PROPERTY_DATA:
			lsm_css_parse_buffer (css, g_value_get_string (value), -1, NULL);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
_get_property (GObject * object, guint prop_id,
	       GValue * value, GParamSpec * pspec)
{
	switch (prop_id) {
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
lsm_css_init (LsmCss *css)
{
	css->priv = G_TYPE_INSTANCE_GET_PRIVATE (css, LSM_TYPE_CSS, LsmCssPrivate);
}

static void
_finalize (GObject *object)
{
	parent_class->finalize (object);
}

/* LsmCss class */

static void
lsm_css_class_init (LsmCssClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (LsmCssPrivate));

	parent_class = g_type_class_peek_parent (klass);

	object_class->finalize = _finalize;
	object_class->set_property = _set_property;
	object_class->get_property = _get_property;

	g_object_class_install_property (
		object_class, LSM_CSS_PROPERTY_DATA,
		g_param_spec_string ("data", "Data",
				     "CSS data", "",
				     G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS)
		);
}

G_DEFINE_TYPE (LsmCss, lsm_css, G_TYPE_OBJECT)

