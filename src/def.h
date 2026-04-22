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

#ifndef DEF_H
#define DEF_H

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#ifdef _WIN32
	#include <windows.h>
	#include <curses.h>
	#define XPLAT_SLEEP(ms) napms(ms)
	#define XPLAT_MIN(a,b) (((a)<(b))?(a):(b))
	#define XPLAT_MAX(a,b) (((a)>(b))?(a):(b))
#else
	#if __has_include(<ncursesw/ncurses.h>)
		#include <ncursesw/ncurses.h>
	#else
		#include <ncurses.h>
	#endif
	#define XPLAT_SLEEP(ms) napms(ms)
	#include <sys/param.h>
	#define XPLAT_MIN(a,b) MIN(a,b)
	#define XPLAT_MAX(a,b) MAX(a,b)
#endif
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <getopt.h>
#include <locale.h>

#include "version.h"

/* Character representation in the matrix */
typedef struct gmchar {
	char val[5];     /* UTF-8 character string */
	bool is_active;  /* Whether the character is currently visible */
	int expiry_in;   /* Frames until the character disappears */
	bool is_head;    /* Whether this is the leading bright character */
} gmchar;

/* RGB color structure for truecolor mode */
typedef struct rgb_color {
	int r, g, b;
	bool is_set;
} rgb_color;

/* Global configuration flags and settings */
extern bool katakana_mode;
extern rgb_color fade_from;
extern rgb_color fade_to;
extern rgb_color bg_color;
extern bool no_bg;
extern bool using_truecolor;
extern bool no_async;
extern bool no_truecolor;
extern bool force_truecolor;
extern bool no_tail_fade;
extern bool no_color;
extern bool force_color;
extern bool distinct_head;
extern bool no_bold;
extern bool all_bold;
extern bool binary_charset;
extern char *custom_charset;
extern char **custom_charset_chars;
extern int custom_charset_len;
extern bool consistent_first_wave;
extern int update_delay;
extern int number_waves;

/* Runtime state and buffers */
extern volatile sig_atomic_t resized;
extern gmchar **buffer;
extern int rows;
extern int cols;
extern int frame;
extern int *col_update;      /* Frequency of update for each column */
extern int *col_cooldown;    /* Delay until next head spawns in column */
extern int *waves_left;      /* Remaining waves for --number-waves */
extern bool *first_wave_seen;
extern bool *first_wave_bottomed;
extern bool *freq_randomized;
extern bool first_wave_fully_done;
extern bool paused;

/* Function prototypes */
void parse_opts(int argc, char *argv[]);
rgb_color parse_rgb(const char *arg);
void print_help(char *program_name);
void print_version(void);

void automated_setup(void);
bool supports_truecolor(void);
void handle_resize(int sig);
void handle_exit(int sig);
void buffer_setup(void);
void buffer_cleanup(void);
void col_data_setup(void);
void buffer_print(void);
void process_gmchar(const int r, const int c);
void process_column(const int c);

#endif
