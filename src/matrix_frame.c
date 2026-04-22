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

/* Helper to set a random character value based on the selected charset */
static void set_gmchar_val(gmchar *g) {
	if (custom_charset_chars && custom_charset_len > 0) {
		/* Custom character set */
		int idx = rand() % custom_charset_len;
		strcpy(g->val, custom_charset_chars[idx]);
	} else if (binary_charset) {
		/* Binary characters (0 and 1) */
		sprintf(g->val, "%c", (rand() % 2) + 48);
	} else if (katakana_mode) {
		/* Half-width Katakana range: U+FF66 to U+FF9D */
		/* UTF-8 for U+FF66 is EF BD A6 */
		int code = 0xFF66 + (rand() % (0xFF9D - 0xFF66 + 1));
		g->val[0] = (char)(0xE0 | ((code >> 12) & 0x0F));
		g->val[1] = (char)(0x80 | ((code >> 6) & 0x3F));
		g->val[2] = (char)(0x80 | (code & 0x3F));
		g->val[3] = '\0';
	} else {
		/* Standard ASCII printable characters */
		sprintf(g->val, "%c", (rand() % 94) + 33);
	}
}

/* Update logic for an individual character in the matrix */
void process_gmchar(const int r, const int c) {
	/* Only update at the specified frequency for the column */
	if (frame % col_update[c] != 0) return;

	if (buffer[r][c].is_head) {
		/* If it's a head, spawn a new head character in the row below */
		if (r < rows - 1) {
			gmchar next;
			set_gmchar_val(&next);
			next.is_active = true;
			next.expiry_in = buffer[r][c].expiry_in;
			next.is_head = true;
			buffer[r+1][c] = next;
		} else if (consistent_first_wave && first_wave_seen[c]) {
			/* Mark that the very first consistent wave reached the bottom */
			first_wave_bottomed[c] = true;
		}
		
		/* The current head character becomes part of the tail */
		buffer[r][c].expiry_in--;
		buffer[r][c].is_head = false;
	} else if (buffer[r][c].is_active) {
		/* Fade out the character over time */
		if (--buffer[r][c].expiry_in < 1) {
			strcpy(buffer[r][c].val, " ");
			buffer[r][c].is_active = false;
		}
	}
}

/* Manage head spawning and frequency randomization for each column */
void process_column(const int c) {
	if (frame % col_update[c] != 0) return;

	/* Handle the initial spawn delay */
	if (col_cooldown[c] > 0) {
		col_cooldown[c]--;
		return;
	}

	if (!buffer[0][c].is_active && (waves_left[c] > 0 || number_waves == -1)) {
		/* Prevent overlapping waves during the first consistent drop */
		if (consistent_first_wave && !first_wave_fully_done) {
			if (first_wave_seen[c]) return;
		}

		/* Staggered transition from consistent wave to randomized async frequencies */
		if (consistent_first_wave && first_wave_fully_done && !no_async && !freq_randomized[c]) {
			col_update[c] = (rand() % 2) + 1;
			col_cooldown[c] = rand() % 20; /* Subtle stagger for the second wave */
			freq_randomized[c] = true;
			return;
		}

		/* Spawn a new head character at the top of the column */
		gmchar head;
		set_gmchar_val(&head);
		head.is_active = true;
		
		int expiry;
		if (consistent_first_wave && !first_wave_seen[c]) {
			expiry = 20; /* Fixed length for the consistent first wave */
			first_wave_seen[c] = true;
		} else {
			expiry = (rand() % 15) + 15;
		}
		head.expiry_in = expiry;
		head.is_head = true;

		buffer[0][c] = head;
		
		/* Calculate cooldown until the next drop can start */
		col_cooldown[c] = head.expiry_in + (rand() % 25) + 10;
		if (number_waves != -1) waves_left[c]--;
	}
}
