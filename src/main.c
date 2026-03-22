#include "def.h"

volatile sig_atomic_t resized = 0;

gmchar **buffer;
int frame = 0;
int *col_update;
int *col_cooldown;
int rows, cols;
bool paused = false;

int main(int argc, char *argv[]) {
	parse_opts(argc, argv);
	automated_setup();
	#ifndef _WIN32
		signal(SIGWINCH, handle_resize);
	#endif
	for (;;) {
		if (resized) {
			clear();
			buffer_cleanup();
			endwin();
			refresh();
			automated_setup();
			resized = 0;
		}
		
		int press = getch();

		if (press == (int)'q') break;
		if (press == (int)'p') paused = !paused;

		if (!paused) {
			frame++;

			for (int c = 0; c < cols; c++) {
				process_column(c);
				for (int r = rows - 1; r >= 0; r--) {
					process_gmchar(r, c);
				}
			}
			
			buffer_print();
		}

		napms(update_delay);
	}

	buffer_cleanup();
	endwin();
	return 0;
}

void automated_setup(void) {
	initscr();
	noecho();
	raw();
	curs_set(0);
	nodelay(stdscr, TRUE);
    
	if ((has_colors() && !no_color) || force_color) {
		start_color();
		init_pair(11, 15, 0);
		if ((supports_truecolor() && !no_truecolor) || force_truecolor) {
			for (short i = 0; i < 10; i++) {
				init_color(i + 16, 0, i * 100, 0);
				init_pair(i + 1, i + 16, 0);
			}
		} else {
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
bool supports_truecolor(void) {
	char *colorterm = getenv("COLORTERM");
	if (colorterm != NULL && (strcmp(colorterm, "truecolor") == 0 || strcmp(colorterm, "24bit") == 0)) {
		return true;
	}
	return false;
}
