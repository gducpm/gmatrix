/*
 * GMatrix - Modern Matrix Rain for your Terminal
 * Copyright (C) 2024 GDucpm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "def.h"

rgb_color fade_from = {0, 255, 0, false};
rgb_color fade_to = {0, 0, 0, false};
rgb_color bg_color = {0, 0, 0, true};
bool no_bg = false;

/* Convert an ANSI (0-15) or 256-color index to its approximate RGB values */
static rgb_color index_to_rgb(int index) {
	rgb_color c = {0, 0, 0, true};
	if (index < 16) {
		/* Standard 16 ANSI colors */
		static const int ansi_colors[16][3] = {
			{0,0,0}, {128,0,0}, {0,128,0}, {128,128,0},
			{0,0,128}, {128,0,128}, {0,128,128}, {192,192,192},
			{128,128,128}, {255,0,0}, {0,255,0}, {255,255,0},
			{0,0,255}, {255,0,255}, {0,255,255}, {255,255,255}
		};
		c.r = ansi_colors[index][0];
		c.g = ansi_colors[index][1];
		c.b = ansi_colors[index][2];
	} else if (index < 232) {
		/* 6x6x6 color cube for 256-color mode */
		int i = index - 16;
		int r = (i / 36) % 6;
		int g = (i / 6) % 6;
		int b = i % 6;
		c.r = r ? (r * 40 + 55) : 0;
		c.g = g ? (g * 40 + 55) : 0;
		c.b = b ? (b * 40 + 55) : 0;
	} else if (index < 256) {
		/* Grayscale ramp for 256-color mode */
		int i = index - 232;
		int v = i * 10 + 8;
		c.r = c.g = c.b = v;
	}
	return c;
}

/* Parse a color argument: supports #RRGGBB, R,G,B, or color index */
rgb_color parse_rgb(const char *arg) {
	rgb_color color = {0, 0, 0, true};
	if (arg[0] == '#') {
		/* Format: #RRGGBB */
		if (strlen(arg) == 7) {
			unsigned int r, g, b;
			sscanf(arg + 1, "%02x%02x%02x", &r, &g, &b);
			color.r = (int)r;
			color.g = (int)g;
			color.b = (int)b;
		}
	} else if (strchr(arg, ',')) {
		/* Format: R,G,B */
		sscanf(arg, "%d,%d,%d", &color.r, &color.g, &color.b);
	} else {
		/* Format: ANSI or 256-color index */
		char *endptr;
		long val = strtol(arg, &endptr, 10);
		if (endptr != arg && val >= 0 && val <= 255) {
			color = index_to_rgb((int)val);
		}
	}
	return color;
}
