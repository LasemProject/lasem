/* gmathmlglyphtableams.c
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

#include <gmathmlglyphtableams.h>
#include <string.h>

const GMathmlOperatorGlyph AMS_table[] = {
	{
		GMATHML_RADICAL_UTF8, /* radical */
		GMATHML_GLYPH_FLAG_STRETCH_VERTICAL |
		GMATHML_GLYPH_FLAG_TYPE_SIZED,
		{GMATHML_FONT_CMEX10,		"t"},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_CMEX10,		"v"},
		{GMATHML_FONT_CMEX10,		"u"},
		4,
		{
			{GMATHML_FONT_CMEX10,		"p"},
			{GMATHML_FONT_CMEX10,		"q"},
			{GMATHML_FONT_CMEX10,		"r"},
			{GMATHML_FONT_CMEX10,		"s"}
		}
	},
	{
		"(",
		GMATHML_GLYPH_FLAG_STRETCH_VERTICAL |
		GMATHML_GLYPH_FLAG_TYPE_SIZED |
		GMATHML_GLYPH_FLAG_ALIGN_AXIS,
		{GMATHML_FONT_CMEX10,		"0"},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_CMEX10,		"@"},
		{GMATHML_FONT_CMEX10,		"B"},
		5,
		{
			{GMATHML_FONT_CMR10,		"("},
			{GMATHML_FONT_CMEX10,		"\xc2\xa1"},
			{GMATHML_FONT_CMEX10,		"\xc2\xb3"},
			{GMATHML_FONT_CMEX10,		"\xc2\xb5"},
			{GMATHML_FONT_CMEX10,		"\xc3\x83"}
		}
	},
	{
		")",
		GMATHML_GLYPH_FLAG_STRETCH_VERTICAL |
		GMATHML_GLYPH_FLAG_TYPE_SIZED |
		GMATHML_GLYPH_FLAG_ALIGN_AXIS,
		{GMATHML_FONT_CMEX10, 		"1"},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_CMEX10,		"A"},
		{GMATHML_FONT_CMEX10,		"C"},
		5,
		{
			{GMATHML_FONT_CMR10, 		")"},
			{GMATHML_FONT_CMEX10,		"\xc2\xa2"},
			{GMATHML_FONT_CMEX10,		"\xc2\xb4"},
			{GMATHML_FONT_CMEX10,		"\xc2\xb6"},
			{GMATHML_FONT_CMEX10,		"!"}
		}
	},
	{
		"{",
		GMATHML_GLYPH_FLAG_STRETCH_VERTICAL |
		GMATHML_GLYPH_FLAG_TYPE_SIZED |
		GMATHML_GLYPH_FLAG_ALIGN_AXIS,
		{GMATHML_FONT_CMEX10, 		"8"},
		{GMATHML_FONT_CMEX10, 		"<"},
		{GMATHML_FONT_CMEX10, 		":"},
		{GMATHML_FONT_CMEX10, 		">"},
		5,
		{
			{GMATHML_FONT_CMSY10, 		"f"},
			{GMATHML_FONT_CMEX10,		"\xc2\xa9"},
			{GMATHML_FONT_CMEX10,		"n"},
			{GMATHML_FONT_CMEX10,		"\xc2\xbd"},
			{GMATHML_FONT_CMEX10,		"("}
		}
	},
	{
		"}",
		GMATHML_GLYPH_FLAG_STRETCH_VERTICAL |
		GMATHML_GLYPH_FLAG_TYPE_SIZED |
		GMATHML_GLYPH_FLAG_ALIGN_AXIS,
		{GMATHML_FONT_CMEX10, 		"9"},
		{GMATHML_FONT_CMEX10, 		"="},
		{GMATHML_FONT_CMEX10, 		";"},
		{GMATHML_FONT_CMEX10, 		">"},
		5,
		{
			{GMATHML_FONT_CMSY10,		"g"},
			{GMATHML_FONT_CMEX10,		"\xc2\xaa"},
			{GMATHML_FONT_CMEX10,		"o"},
			{GMATHML_FONT_CMEX10,		"\xc2\xbe"},
			{GMATHML_FONT_CMEX10,		")"}
		}
	},
	{
		"\xe2\x88\x91" /* ∑ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMEX10,	"P"},
			{GMATHML_FONT_CMEX10,	"X"}
		}
	},
	{
		"\xe2\x88\x8f" /* ∏ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMEX10,	"Q"},
			{GMATHML_FONT_CMEX10,	"Y"}
		}
	},
	{
		"\xe2\x88\x90" /* ∐ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMEX10,	"`"},
			{GMATHML_FONT_CMEX10,	"a"}
		}
	},
	{
		"\xe2\x8b\x81" /* ∨ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMSY10,	"_"},
			{GMATHML_FONT_CMEX10,	"W"}
		}
	},
	{
		"\xe2\x8b\x80" /* ∧ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMSY10,	"^"},
			{GMATHML_FONT_CMEX10,	"V"}
		}
	},
	{
		"\xe2\x88\xa8" /* ∨ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMSY10,	"_"},
			{GMATHML_FONT_CMEX10,	"W"}
		}
	},
	{
		"\xe2\x88\xa7" /* ∧ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMSY10,	"^"},
			{GMATHML_FONT_CMEX10,	"V"}
		}
	},
	{
		"\xe2\x88\xa9" /* ⋂ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMSY10,	"\\"},
			{GMATHML_FONT_CMEX10,	"\\"}
		}
	},
	{
		"\xe2\x88\xaa" /* ⋃ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMSY10,	"["},
			{GMATHML_FONT_CMEX10,	"["}
		}
	},
	{
		"\xe2\x8b\x82" /* ⋂ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMSY10,	"\\"},
			{GMATHML_FONT_CMEX10,	"\\"}
		}
	},
	{
		"\xe2\x8b\x80" /* ⋃ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMSY10,	"]"},
			{GMATHML_FONT_CMEX10,	"]"}
		}
	},
	{
		"\xe2\x88\xab" /* ∫ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMEX10,	"R"},
			{GMATHML_FONT_CMEX10,	"Z"}
		}
	},
	{
		"\xe2\x88\xac" /* ∬ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMEX10,	"RR"},
			{GMATHML_FONT_CMEX10,	"ZZ"}
		}
	},
	{
		"\xe2\x88\xad" /* ∭ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMEX10,	"RRR"},
			{GMATHML_FONT_CMEX10,	"ZZZ"}
		}
	},
	{
		"\xe2\x88\xae" /* ∮ */,
		GMATHML_GLYPH_FLAG_ALIGN_AXIS | GMATHML_GLYPH_FLAG_HAS_LARGE_VERSION,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		2,
		{
			{GMATHML_FONT_CMEX10,	"H"},
			{GMATHML_FONT_CMEX10,	"I"}
		}
	},
	{
		"\xe2\x85\x86" /* d */,
		0,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		1,
		{
			{GMATHML_FONT_CMI10,	"d"}
		}
	},
	{
		"\xe2\x85\x88" /* i */,
		0,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		1,
		{
			{GMATHML_FONT_CMI10,	"i"}
		}
	},
	{
		"\xf0\x9d\x95\x9b" /* j */,
		0,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		1,
		{
			{GMATHML_FONT_CMI10,	"j"}
		}
	},
	{
		"\xcc\xb2" /* _ */,
		GMATHML_GLYPH_FLAG_STRETCH_HORIZONTAL,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		1,
		{
			{GMATHML_FONT_DEFAULT,	"\xcc\xb2"}
		}
	},
	{
		"\xc2\xaf" /* hor */,
		GMATHML_GLYPH_FLAG_STRETCH_HORIZONTAL,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		1,
		{
			{GMATHML_FONT_DEFAULT,	"\xc2\xaf"}
		}
	},
	{
		"-",
		0,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		1,
		{
			{GMATHML_FONT_DEFAULT,	"\xe2\x88\x92"}
		}
	},
	{
		"|",
		GMATHML_GLYPH_FLAG_STRETCH_VERTICAL,
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		{GMATHML_FONT_ERROR,		""},
		1,
		{
			{GMATHML_FONT_DEFAULT,	"|"}
		}
	}
};

const GMathmlOperatorGlyph *
gmathml_glyph_table_find_operator_glyph (const char *text)
{
	unsigned int i;

	for (i = 0; i < G_N_ELEMENTS (AMS_table); i++)
		if (strcmp (text, AMS_table[i].utf8) == 0) {
			g_message ("[GMathmlView::find_strecthy_glyph] found %s", text);
			return &AMS_table[i];
		}

	return NULL;
}
