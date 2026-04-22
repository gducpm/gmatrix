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

/* Allocate and initialize the matrix character buffer and metadata arrays */
void buffer_setup(void) {
	getmaxyx(stdscr, rows, cols);
	cols /= 2; /* Using double columns for spacing */

	buffer = malloc((size_t)rows * sizeof(gmchar *));
	col_update = malloc((size_t)cols * sizeof(int));
	col_cooldown = malloc((size_t)cols * sizeof(int));
	waves_left = malloc((size_t)cols * sizeof(int));
	first_wave_seen = calloc((size_t)cols, sizeof(bool));
	first_wave_bottomed = calloc((size_t)cols, sizeof(bool));
	freq_randomized = calloc((size_t)cols, sizeof(bool));
	
	if (!buffer || !col_update || !col_cooldown || !waves_left || 
	    !first_wave_seen || !first_wave_bottomed || !freq_randomized) {
		perror("Cannot allocate matrix arrays!");
		exit(42);
	}

	for (int r = 0; r < rows; r++) {
		buffer[r] = malloc((size_t)cols * sizeof(gmchar));
		if (!buffer[r]) {
			perror("Cannot allocate matrix array row!");
			exit(42);
		}

		/* Initialize each cell as empty */
		for (int c = 0; c < cols; c++) {
			strcpy(buffer[r][c].val, " ");
			buffer[r][c].is_active = false;
			buffer[r][c].expiry_in = 0;
			buffer[r][c].is_head = false;
		}
	}
}

/* Initialize starting frequencies and cooldowns for each column */
void col_data_setup(void) {
	for (int c = 0; c < cols; c++) {
		waves_left[c] = number_waves;
		if (no_async) {
			col_update[c] = 1;
			col_cooldown[c] = consistent_first_wave ? 0 : rand() % 50;
		} else {
			/* Force freq of 1 for the --consistent first wave */
			col_update[c] = consistent_first_wave ? 1 : (rand() % 2) + 1;
			col_cooldown[c] = consistent_first_wave ? 0 : rand() % 50;
		}
	}
}

/* Render the entire matrix buffer to the terminal */
void buffer_print(void) {
	for (int r = 0; r < rows; r++) {
		if (using_truecolor) {
			/* Set cursor position and background color for the row once */
			printf("\033[%d;1H", r + 1);
			if (!no_bg) printf("\033[48;2;%d;%d;%dm", bg_color.r, bg_color.g, bg_color.b);
		} else {
			/* Reset attributes for ncurses before moving */
			attrset(0);
			move(r, 0);
		}
		
		for (int c = 0; c < cols; c++) {
			if (buffer[r][c].is_head && distinct_head) {
				/* Draw bright head character */
				if (using_truecolor) {
					printf("\033[%s38;2;255;255;255m%s \033[22;39m", no_bold ? "" : "1;", buffer[r][c].val);
				} else {
					attrset(0);
					if (no_bold) attron(COLOR_PAIR(11));
					else attron(COLOR_PAIR(11) | A_BOLD);
					mvaddstr(r, c*2, buffer[r][c].val);
					addch(' ');
					attrset(0);
				}
			} else if (buffer[r][c].is_active) {
				/* Draw fading tail character */
				int pair_value;
				if (no_tail_fade) pair_value = 10;
				else pair_value = XPLAT_MIN(10, buffer[r][c].expiry_in);
				
				if (using_truecolor) {
					/* Interpolate color between from and to values */
					double ratio = (double)pair_value / 10.0;
					int r_val = (int)(fade_to.r + ratio * (fade_from.r - fade_to.r));
					int g_val = (int)(fade_to.g + ratio * (fade_from.g - fade_to.g));
					int b_val = (int)(fade_to.b + ratio * (fade_from.b - fade_to.b));
					printf("\033[%s38;2;%d;%d;%dm%s \033[22;39m", all_bold ? "1;" : "", r_val, g_val, b_val, buffer[r][c].val);
				} else {
					attrset(0);
					if (all_bold) attron(COLOR_PAIR(pair_value) | A_BOLD);
					else attron(COLOR_PAIR(pair_value));
					mvaddstr(r, c*2, buffer[r][c].val);
					addch(' ');
					attrset(0);
				}
			} else {
				/* Draw empty space */
				if (using_truecolor) {
					printf("  ");
				} else {
					attrset(0);
					mvaddstr(r, c*2, "  ");
				}
			}
		}
		
		/* Clear to end of line to handle padding/excess terminal width */
		if (using_truecolor) printf("\033[K\033[0m");
		else {
			attrset(0);
			clrtoeol();
		}
	}
	
	if (using_truecolor) fflush(stdout);
	else refresh();
}

/* Free all allocated memory */
void buffer_cleanup(void) {
	if (buffer) {
		for (int r = 0; r < rows; r++) {
			free(buffer[r]);
		}
		free(buffer);
	}
	if (col_update) free(col_update);
	if (col_cooldown) free(col_cooldown);
	if (waves_left) free(waves_left);
	if (first_wave_seen) free(first_wave_seen);
	if (first_wave_bottomed) free(first_wave_bottomed);
	if (freq_randomized) free(freq_randomized);
	if (custom_charset) free(custom_charset);
	if (custom_charset_chars) {
		for (int i = 0; i < custom_charset_len; i++) {
			free(custom_charset_chars[i]);
		}
		free(custom_charset_chars);
		custom_charset_chars = NULL;
	}
}
