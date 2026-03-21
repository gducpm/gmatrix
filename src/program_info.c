#include "def.h"

void print_help(char *program_name) {
	printf(
	"GMatrix: modern Matrix rain for your terminal\n"
	"Usage: %s [OPTIONS]\n\n"
	" -h / --help                  Prints this help text\n\n"
	" -v / --version               Prints program versioning\n"
	"                              and licensing info\n\n"
	" -u / --update-delay          Makes the rain fall slower\n"
	"                              or faster (default value:\n"
	"                              80)\n\n"
	" -T / --disable-truecolor     Disables truecolor for\n"
	"                              terminals that does not\n"
	"                              support it (often\n"
	"                              automatically\n"
	"                              (Overridden by -t)\n"
	" -t / --force-truecolor       Forces truecolor for a\n"
	"                              wider range of terminal\n"
	"                              colors (only terminals\n"
	"                              that support it, and often\n"
	"                              enabled automatically)\n"
	"                              (Overrides -T)\n\n"
	" -A / --no-async              Disables asynchronous rain\n\n"
	" -F / --no-tail-fade          Disables tail fading\n\n"
	" -C / --no-color              Disables colors\n"
	"                              (Overridden by -c)\n\n"
	" -c / --force-color           Forces colors to be on\n"
	"                              (Overrides -C)\n\n"
	" -w / --distinct-head         Head shows up distinct\n\n"
	" -B / --no-bold               Disables bold entirely\n\n"
	" -b / --all-bold              Applies bold to all\n"
	"                              characters\n\n"
	" -s / --binary-charset        Changes the raining\n"
	"                              characters to 0 and 1\n\n",
	program_name);
}

void print_version() {
	printf("GMatrix " VERSION " by GDucpm\n\n" // Replaced by macro
		"Contributors:\n"
		"none\n"
		"\n"
		"GMatrix is libre software licensed under the GNU\n"
		"General Public License 3+, released WITHOUT\n"
		"WARRANTY OF ANY KIND.\n");
}
