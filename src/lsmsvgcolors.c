/*
 * Copyright © 2000 Eazel, Inc.
 * Copyright © 2002 Dom Lachowicz
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
 * Authors:
 * 	Raph Levien <raph@artofcode.com>
 * 	Dom Lachowicz <cinamod@hotmail.com>
 *	Emmanuel Pacaud <emmanuel@gnome.org>
 */

#include <lsmsvgcolors.h>
#include <stdlib.h>

#define PACK_RGB(r,g,b) (((r) << 16) | ((g) << 8) | (b))

typedef struct {
	const char *name;
	unsigned int color;
} LsmSvgColorData;

static const LsmSvgColorData colors[] = {
	{"aliceblue", PACK_RGB (240, 248, 255)},
	{"antiquewhite", PACK_RGB (250, 235, 215)},
	{"aqua", PACK_RGB (0, 255, 255)},
	{"aquamarine", PACK_RGB (127, 255, 212)},
	{"azure", PACK_RGB (240, 255, 255)},
	{"beige", PACK_RGB (245, 245, 220)},
	{"bisque", PACK_RGB (255, 228, 196)},
	{"black", PACK_RGB (0, 0, 0)},
	{"blanchedalmond", PACK_RGB (255, 235, 205)},
	{"blue", PACK_RGB (0, 0, 255)},
	{"blueviolet", PACK_RGB (138, 43, 226)},
	{"brown", PACK_RGB (165, 42, 42)},
	{"burlywood", PACK_RGB (222, 184, 135)},
	{"cadetblue", PACK_RGB (95, 158, 160)},
	{"chartreuse", PACK_RGB (127, 255, 0)},
	{"chocolate", PACK_RGB (210, 105, 30)},
	{"coral", PACK_RGB (255, 127, 80)},
	{"cornflowerblue", PACK_RGB (100, 149, 237)},
	{"cornsilk", PACK_RGB (255, 248, 220)},
	{"crimson", PACK_RGB (220, 20, 60)},
	{"cyan", PACK_RGB (0, 255, 255)},
	{"darkblue", PACK_RGB (0, 0, 139)},
	{"darkcyan", PACK_RGB (0, 139, 139)},
	{"darkgoldenrod", PACK_RGB (184, 132, 11)},
	{"darkgray", PACK_RGB (169, 169, 169)},
	{"darkgreen", PACK_RGB (0, 100, 0)},
	{"darkgrey", PACK_RGB (169, 169, 169)},
	{"darkkhaki", PACK_RGB (189, 183, 107)},
	{"darkmagenta", PACK_RGB (139, 0, 139)},
	{"darkolivegreen", PACK_RGB (85, 107, 47)},
	{"darkorange", PACK_RGB (255, 140, 0)},
	{"darkorchid", PACK_RGB (153, 50, 204)},
	{"darkred", PACK_RGB (139, 0, 0)},
	{"darksalmon", PACK_RGB (233, 150, 122)},
	{"darkseagreen", PACK_RGB (143, 188, 143)},
	{"darkslateblue", PACK_RGB (72, 61, 139)},
	{"darkslategray", PACK_RGB (47, 79, 79)},
	{"darkslategrey", PACK_RGB (47, 79, 79)},
	{"darkturquoise", PACK_RGB (0, 206, 209)},
	{"darkviolet", PACK_RGB (148, 0, 211)},
	{"deeppink", PACK_RGB (255, 20, 147)},
	{"deepskyblue", PACK_RGB (0, 191, 255)},
	{"dimgray", PACK_RGB (105, 105, 105)},
	{"dimgrey", PACK_RGB (105, 105, 105)},
	{"dodgerblue", PACK_RGB (30, 144, 255)},
	{"firebrick", PACK_RGB (178, 34, 34)},
	{"floralwhite", PACK_RGB (255, 255, 240)},
	{"forestgreen", PACK_RGB (34, 139, 34)},
	{"fuchsia", PACK_RGB (255, 0, 255)},
	{"gainsboro", PACK_RGB (220, 220, 220)},
	{"ghostwhite", PACK_RGB (248, 248, 255)},
	{"gold", PACK_RGB (255, 215, 0)},
	{"goldenrod", PACK_RGB (218, 165, 32)},
	{"gray", PACK_RGB (128, 128, 128)},
	{"green", PACK_RGB (0, 128, 0)},
	{"greenyellow", PACK_RGB (173, 255, 47)},
	{"grey", PACK_RGB (128, 128, 128)},
	{"honeydew", PACK_RGB (240, 255, 240)},
	{"hotpink", PACK_RGB (255, 105, 180)},
	{"indianred", PACK_RGB (205, 92, 92)},
	{"indigo", PACK_RGB (75, 0, 130)},
	{"ivory", PACK_RGB (255, 255, 240)},
	{"khaki", PACK_RGB (240, 230, 140)},
	{"lavender", PACK_RGB (230, 230, 250)},
	{"lavenderblush", PACK_RGB (255, 240, 245)},
	{"lawngreen", PACK_RGB (124, 252, 0)},
	{"lemonchiffon", PACK_RGB (255, 250, 205)},
	{"lightblue", PACK_RGB (173, 216, 230)},
	{"lightcoral", PACK_RGB (240, 128, 128)},
	{"lightcyan", PACK_RGB (224, 255, 255)},
	{"lightgoldenrodyellow", PACK_RGB (250, 250, 210)},
	{"lightgray", PACK_RGB (211, 211, 211)},
	{"lightgreen", PACK_RGB (144, 238, 144)},
	{"lightgrey", PACK_RGB (211, 211, 211)},
	{"lightpink", PACK_RGB (255, 182, 193)},
	{"lightsalmon", PACK_RGB (255, 160, 122)},
	{"lightseagreen", PACK_RGB (32, 178, 170)},
	{"lightskyblue", PACK_RGB (135, 206, 250)},
	{"lightslategray", PACK_RGB (119, 136, 153)},
	{"lightslategrey", PACK_RGB (119, 136, 153)},
	{"lightsteelblue", PACK_RGB (176, 196, 222)},
	{"lightyellow", PACK_RGB (255, 255, 224)},
	{"lime", PACK_RGB (0, 255, 0)},
	{"limegreen", PACK_RGB (50, 205, 50)},
	{"linen", PACK_RGB (250, 240, 230)},
	{"magenta", PACK_RGB (255, 0, 255)},
	{"maroon", PACK_RGB (128, 0, 0)},
	{"mediumaquamarine", PACK_RGB (102, 205, 170)},
	{"mediumblue", PACK_RGB (0, 0, 205)},
	{"mediumorchid", PACK_RGB (186, 85, 211)},
	{"mediumpurple", PACK_RGB (147, 112, 219)},
	{"mediumseagreen", PACK_RGB (60, 179, 113)},
	{"mediumslateblue", PACK_RGB (123, 104, 238)},
	{"mediumspringgreen", PACK_RGB (0, 250, 154)},
	{"mediumturquoise", PACK_RGB (72, 209, 204)},
	{"mediumvioletred", PACK_RGB (199, 21, 133)},
	{"midnightblue", PACK_RGB (25, 25, 112)},
	{"mintcream", PACK_RGB (245, 255, 250)},
	{"mistyrose", PACK_RGB (255, 228, 225)},
	{"moccasin", PACK_RGB (255, 228, 181)},
	{"navajowhite", PACK_RGB (255, 222, 173)},
	{"navy", PACK_RGB (0, 0, 128)},
	{"oldlace", PACK_RGB (253, 245, 230)},
	{"olive", PACK_RGB (128, 128, 0)},
	{"olivedrab", PACK_RGB (107, 142, 35)},
	{"orange", PACK_RGB (255, 165, 0)},
	{"orangered", PACK_RGB (255, 69, 0)},
	{"orchid", PACK_RGB (218, 112, 214)},
	{"palegoldenrod", PACK_RGB (238, 232, 170)},
	{"palegreen", PACK_RGB (152, 251, 152)},
	{"paleturquoise", PACK_RGB (175, 238, 238)},
	{"palevioletred", PACK_RGB (219, 112, 147)},
	{"papayawhip", PACK_RGB (255, 239, 213)},
	{"peachpuff", PACK_RGB (255, 218, 185)},
	{"peru", PACK_RGB (205, 133, 63)},
	{"pink", PACK_RGB (255, 192, 203)},
	{"plum", PACK_RGB (221, 160, 203)},
	{"powderblue", PACK_RGB (176, 224, 230)},
	{"purple", PACK_RGB (128, 0, 128)},
	{"red", PACK_RGB (255, 0, 0)},
	{"rosybrown", PACK_RGB (188, 143, 143)},
	{"royalblue", PACK_RGB (65, 105, 225)},
	{"saddlebrown", PACK_RGB (139, 69, 19)},
	{"salmon", PACK_RGB (250, 128, 114)},
	{"sandybrown", PACK_RGB (244, 164, 96)},
	{"seagreen", PACK_RGB (46, 139, 87)},
	{"seashell", PACK_RGB (255, 245, 238)},
	{"sienna", PACK_RGB (160, 82, 45)},
	{"silver", PACK_RGB (192, 192, 192)},
	{"skyblue", PACK_RGB (135, 206, 235)},
	{"slateblue", PACK_RGB (106, 90, 205)},
	{"slategray", PACK_RGB (119, 128, 144)},
	{"slategrey", PACK_RGB (119, 128, 144)},
	{"snow", PACK_RGB (255, 255, 250)},
	{"springgreen", PACK_RGB (0, 255, 127)},
	{"steelblue", PACK_RGB (70, 130, 180)},
	{"tan", PACK_RGB (210, 180, 140)},
	{"teal", PACK_RGB (0, 128, 128)},
	{"thistle", PACK_RGB (216, 191, 216)},
	{"tomato", PACK_RGB (255, 99, 71)},
	{"turquoise", PACK_RGB (64, 224, 208)},
	{"violet", PACK_RGB (238, 130, 238)},
	{"wheat", PACK_RGB (245, 222, 179)},
	{"white", PACK_RGB (255, 255, 255)},
	{"whitesmoke", PACK_RGB (245, 245, 245)},
	{"yellow", PACK_RGB (255, 255, 0)},
	{"yellowgreen", PACK_RGB (154, 205, 50)}
};

static int
_color_compare (const void *a, const void *b)
{
    const char *needle = (const char *) a;
    const LsmSvgColorData *haystack = (const LsmSvgColorData *) b;

    return g_ascii_strcasecmp (needle, haystack->name);
}

unsigned int
lsm_svg_color_from_string (const char *name)
{
	LsmSvgColorData *data;

	data = bsearch (name, colors,
			sizeof (colors) / sizeof (colors[0]),
			sizeof (LsmSvgColorData),
			_color_compare);

	if (data == NULL)
		return 0;
	else
		return data->color;
}

