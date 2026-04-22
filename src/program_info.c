#include "def.h"

void print_help(char *program_name) {
	printf(
	"GMatrix - Modern Matrix Rain for your Terminal\n\n"
	"USAGE:\n"
	"  %s [OPTIONS]\n\n"
	"GENERAL OPTIONS:\n"
	"  -h, --help                Prints this help text\n"
	"  -v, --version             Prints program versioning and licensing info\n"
	"  -u, --update-delay VAL    Sets falling speed (default: 80ms)\n"
	"  -n, --number-waves VAL    Limit waves per column and exit when done\n"
	"  -A, --no-async            Disable asynchronous column updates\n"
	"  -S, --consistent          First wave drops simultaneously with uniform length\n\n"
	"APPEARANCE OPTIONS:\n"
	"  -F, --no-tail-fade        Disable tail fading\n"
	"  -B, --no-bold             Disable bold text globally\n"
	"  -b, --all-bold            Apply bold text to all characters\n"
	"  -d, --distinct-head       Make the head character distinct (bright white)\n"
	"  -C, --no-color            Disable colors (classic mode)\n"
	"  -c, --force-color         Force color mode on\n\n"
	"CHARACTER SETS:\n"
	"  -s, --binary-charset      Use '0' and '1' characters\n"
	"  -k, --katakana            Use half-width Katakana characters (Matrix style)\n"
	"  -K, --custom-charset STR  Use a custom set of characters\n\n"
	"TRUECOLOR SETTINGS (24-bit):\n"
	"  -t, --force-truecolor     Force truecolor mode (supports RGB)\n"
	"  -T, --disable-truecolor   Disable truecolor mode\n"
	"  --fade-from COLOR         Start color for fade (hex \"#RRGGBB\" or \"R,G,B\")\n"
	"  --fade-to COLOR           End color for fade (hex \"#RRGGBB\" or \"R,G,B\")\n"
	"  --bg-color COLOR          Background color (hex \"#RRGGBB\" or \"R,G,B\")\n"
	"  --no-bg                   Disable background color rendering\n\n",
	program_name);
}

void print_version(void) {
	printf(
	"GMatrix v" VERSION "\n"
	"Copyright (C) 2024 GDucpm\n\n"
	"License: GNU General Public License v3.0\n"
	"This is free software: you are free to change and redistribute it.\n"
	"There is NO WARRANTY, to the extent permitted by law.\n\n"
	"Written by GDucpm\n");
}
