#ifndef DEF_H
#define DEF_H

#ifdef _WIN32
	#include <windows.h>
	#include <curses.h>
	#define XPLAT_SLEEP(ms) napms(ms)
#else
	#include <ncurses.h>
	#define XPLAT_SLEEP(ms) napms(ms)
#endif
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/param.h>
#include <getopt.h>

#include "version.h"

typedef struct gmchar {
	unsigned char val;
	bool is_active;
	int expiry_in;
	bool is_head;
} gmchar;

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
extern int update_delay;

extern volatile sig_atomic_t resized;
extern gmchar **buffer;
extern int rows;
extern int cols;
extern int frame;
extern int *col_update;
extern int *col_cooldown;
extern bool pause;

void parse_opts(int argc, char *argv[]);
void print_help(char *program_name);
void print_version(void);

void automated_setup(void);
bool supports_truecolor(void);
void handle_resize(int sig);
void buffer_setup(void);
void buffer_cleanup(void);
void col_data_setup(void);
void buffer_print(void);
void process_gmchar(const int r, const int c);
void process_column(const int c);

#endif

