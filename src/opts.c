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

/* Global configuration defaults */
bool no_async = false;
bool no_truecolor = false;
bool force_truecolor = false;
bool no_tail_fade = false;
bool no_color = false;
bool force_color = false;
bool distinct_head = false;
bool no_bold = false;
bool all_bold = false;
bool binary_charset = false;
bool katakana_mode = false;
char *custom_charset = NULL;
char **custom_charset_chars = NULL;
int custom_charset_len = 0;
bool consistent_first_wave = false;
int update_delay = 80;
int number_waves = -1;

/* Command line options definition */
static struct option long_options[] = {
	{"help", no_argument, 0, 'h'},
	{"version", no_argument, 0, 'v'},
	{"update-delay", required_argument, 0, 'u'},
	{"no-async", no_argument, 0, 'A'},
	{"no-truecolor", no_argument, 0, 'T'},
	{"force-truecolor", no_argument, 0, 't'},
	{"no-tail-fade", no_argument, 0, 'F'},
	{"no-color", no_argument, 0, 'C'},
	{"force-color", no_argument, 0, 'c'},
	{"distinct-head", no_argument, 0, 'd'},
	{"no-bold", no_argument, 0, 'B'},
	{"all-bold", no_argument, 0, 'b'},
	{"binary-charset", no_argument, 0, 's'},
	{"katakana", no_argument, 0, 'k'},
	{"custom-charset", required_argument, 0, 'K'},
	{"consistent", no_argument, 0, 'S'},
	{"number-waves", required_argument, 0, 'n'},
	{"fade-from", required_argument, 0, 1},
	{"fade-to", required_argument, 0, 2},
	{"bg-color", required_argument, 0, 3},
	{"no-bg", no_argument, 0, 4},
	{0, 0, 0, 0}
};

/* Parse the custom charset string into individual UTF-8 characters */
static void parse_custom_charset(void) {
	if (!custom_charset) return;
	
	/* Count UTF-8 characters in the string */
	int len = 0;
	for (int i = 0; custom_charset[i]; ) {
		unsigned char c = (unsigned char)custom_charset[i];
		if (c < 0x80) i += 1;
		else if ((c & 0xE0) == 0xC0) i += 2;
		else if ((c & 0xF0) == 0xE0) i += 3;
		else if ((c & 0xF8) == 0xF0) i += 4;
		else i += 1; /* Handle invalid leading byte by skipping */
		len++;
	}
	
	custom_charset_len = len;
	custom_charset_chars = malloc((size_t)len * sizeof(char *));
	
	/* Extract each character into a new string */
	int char_idx = 0;
	for (int i = 0; custom_charset[i]; ) {
		unsigned char c = (unsigned char)custom_charset[i];
		int char_len = 1;
		if (c < 0x80) char_len = 1;
		else if ((c & 0xE0) == 0xC0) char_len = 2;
		else if ((c & 0xF0) == 0xE0) char_len = 3;
		else if ((c & 0xF8) == 0xF0) char_len = 4;
		
		custom_charset_chars[char_idx] = malloc((size_t)char_len + 1);
		memcpy(custom_charset_chars[char_idx], &custom_charset[i], (size_t)char_len);
		custom_charset_chars[char_idx][char_len] = '\0';
		
		i += char_len;
		char_idx++;
	}
}

/* Parse command line arguments */
void parse_opts(int argc, char *argv[]) {
	int opt;
	while ((opt = getopt_long(argc, argv, "u:hvATtFCcdBbskK:n:S", long_options, NULL)) != -1) {
		switch (opt) {
			case 'h': print_help(argv[0]); exit(0);
			case 'v': print_version(); exit(0);
			case 'u': update_delay = atoi(optarg); break;
			case 'A': no_async = true; break;
			case 'T': no_truecolor = true; break;
			case 't': force_truecolor = true; break;
			case 'F': no_tail_fade = true; break;
			case 'C': no_color = true; break;
			case 'c': force_color = true; break;
			case 'd': distinct_head = true; break;
			case 'B': no_bold = true; break;
			case 'b': all_bold = true; break;
			case 's': binary_charset = true; break;
			case 'k': katakana_mode = true; break;
			case 'K': custom_charset = strdup(optarg); break;
			case 'S': consistent_first_wave = true; break;
			case 'n': number_waves = atoi(optarg); break;
			case 1: fade_from = parse_rgb(optarg); break;
			case 2: fade_to = parse_rgb(optarg); break;
			case 3: bg_color = parse_rgb(optarg); break;
			case 4: no_bg = true; break;
			case '?': fprintf(stderr, "Consult %s --help for more information.\n", argv[0]); exit(42);
            		default: exit(42);
		}
	}
	if (custom_charset) parse_custom_charset();
}
