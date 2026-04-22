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

bool using_truecolor = false;
volatile sig_atomic_t resized = 0;
volatile sig_atomic_t running = 1;

gmchar **buffer;
int frame = 0;
int *col_update;
int *col_cooldown;
int *waves_left;
bool *first_wave_seen;
bool *first_wave_bottomed;
bool *freq_randomized;
bool first_wave_fully_done = false;
int rows, cols;
bool paused = false;

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, ""); /* Enable UTF-8 support */
	parse_opts(argc, argv);
	automated_setup();
	
	#ifndef _WIN32
		signal(SIGWINCH, handle_resize);
		signal(SIGINT, handle_exit);
	#endif

	/* Main application loop */
	for (;;) {
		if (!running) break;
		
		/* Handle terminal resize */
		if (resized) {
			clear();
			buffer_cleanup();
			endwin();
			refresh();
			automated_setup();
			resized = 0;
			first_wave_fully_done = false;
		}
		
		int press = getch();

		if (press == (int)'q') break;
		if (press == (int)'p') paused = !paused;

		if (!paused) {
			frame++;

			bool any_active = false;
			bool first_wave_active = false;
			
			/* Process each column of the matrix */
			for (int c = 0; c < cols; c++) {
				process_column(c);
				for (int r = rows - 1; r >= 0; r--) {
					process_gmchar(r, c);
					if (buffer[r][c].is_active) {
						any_active = true;
					}
				}
				
				/* Logic to track the status of the first consistent wave */
				if (consistent_first_wave && !first_wave_fully_done) {
					if (!first_wave_bottomed[c]) first_wave_active = true;
					else {
						/* Column bottomed out, but check if characters are still fading */
						for (int r = 0; r < rows; r++) {
							if (buffer[r][c].is_active) {
								first_wave_active = true;
								break;
							}
						}
					}
				}
			}

			/* If all columns finished the first wave, we transition to random mode */
			if (consistent_first_wave && !first_wave_fully_done && !first_wave_active) {
				first_wave_fully_done = true;
			}
			
			buffer_print();

			/* Termination logic for --number-waves */
			if (number_waves != -1) {
				bool all_done = true;
				for (int c = 0; c < cols; c++) {
					if (waves_left[c] > 0) {
						all_done = false;
						break;
					}
				}
				if (all_done && !any_active) break;
			}
		}

		napms(update_delay);
	}

	buffer_cleanup();
	endwin();
	return 0;
}

/* Initialize ncurses and program state */
void automated_setup(void) {
	initscr();
	noecho();
	cbreak();    /* Immediate input, but allows signals like Ctrl+C */
	curs_set(0); /* Hide the cursor */
	nodelay(stdscr, TRUE);
    
	if ((has_colors() && !no_color) || force_color) {
		start_color();
		init_pair(11, 15, 0); /* Head color pair */
		
		using_truecolor = ((supports_truecolor() && !no_truecolor) || force_truecolor);
		if (using_truecolor) {
			/* Clear screen using ANSI escape sequence for a clean truecolor start */
			printf("\033[2J\033[H");
			fflush(stdout);
			
			/* Initialize some basic pairs for the tail fading in non-RGB mode */
			for (short i = 0; i < 10; i++) {
				init_color(i + 16, 0, i * 100, 0);
				init_pair(i + 1, i + 16, 0);
			}
		} else {
			/* Classic 16-color mode tail pairs */
			for (short i = 0; i < 10; i++) {
				init_pair(i + 1, 2, 0);
			}
		}
	}

	srand((unsigned int)time(NULL));
	buffer_setup();
	col_data_setup();
}

void handle_resize(int sig) {
	(void)sig;
	resized = 1;
}

void handle_exit(int sig) {
	(void)sig;
	running = 0;
}

/* Check environment variables for truecolor support */
bool supports_truecolor(void) {
	char *colorterm = getenv("COLORTERM");
	if (colorterm != NULL && (strcmp(colorterm, "truecolor") == 0 || strcmp(colorterm, "24bit") == 0)) {
		return true;
	}
	return false;
}
