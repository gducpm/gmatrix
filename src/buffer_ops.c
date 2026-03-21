#include "def.h"

void buffer_setup(void) {
	getmaxyx(stdscr, rows, cols);
	cols /= 2;

	buffer = malloc((size_t)rows * sizeof(gmchar *));
	col_update = malloc((size_t)cols * sizeof(int));
	col_cooldown = malloc((size_t)cols * sizeof(int));
	if (!buffer || !col_update || !col_cooldown) {
		perror("Cannot allocate matrix array, update array or cooldown array!");
		exit(42);
	}

	for (int r = 0; r < rows; r++) {
		buffer[r] = malloc((size_t)cols * sizeof(gmchar));
		if (!buffer[r]) {
			perror("Cannot allocate matrix array row!");
			exit(42);
		}

		for (int c = 0; c < cols; c++) {
			buffer[r][c] = (gmchar){' ', false, 0, false};
		}
	}
}

void col_data_setup(void) {
	for (int c = 0; c < cols; c++) {
		if (no_async) {
			col_update[c] = 1;
			col_cooldown[c] = rand() % 50;
		} else {
			col_update[c] = (rand() % 2) + 1;
			col_cooldown[c] = rand() % 50;
		}
	}
}

void buffer_print(void) {
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			if (buffer[r][c].is_head && distinct_head) {
				if (no_bold) attron(COLOR_PAIR(11));
				else attron(COLOR_PAIR(11) | A_BOLD);
				mvaddch(r, c*2, (unsigned char)buffer[r][c].val);
				if (no_bold) attroff(COLOR_PAIR(11));
				else attroff(COLOR_PAIR(11) | A_BOLD);
			} else if (buffer[r][c].is_active) {
				int pair_value;
				if (no_tail_fade) pair_value = 10;
				else pair_value = MIN(10, buffer[r][c].expiry_in);
				if (all_bold) attron(COLOR_PAIR(pair_value) | A_BOLD);
				else attron(COLOR_PAIR(pair_value));
				mvaddch(r, c*2, (unsigned char)buffer[r][c].val);
				if (all_bold) attroff(COLOR_PAIR(pair_value) | A_BOLD);
				else attroff(COLOR_PAIR(pair_value));
			} else {
				mvaddch(r, c*2, ' ');
			}
		}
	}
	refresh();
}

void buffer_cleanup(void) {
	if (buffer) {
		for (int r = 0; r < rows; r++) {
			free(buffer[r]);
		}
		free(buffer);
	}
	if (col_update) free(col_update);
	if (col_cooldown) free(col_cooldown);
}
