#include "def.h"

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
int update_delay = 80;

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
	{0, 0, 0, 0}
};

void parse_opts(int argc, char *argv[]) {
	int opt;
	while ((opt = getopt_long(argc, argv, "u:hvATtFCcdBbs", long_options, NULL)) != -1) {
		switch (opt) {
			case 'h': print_help(argv[0]); exit(0); //done
			case 'v': print_version(); exit(0); //done
			case 'u': update_delay = atoi(optarg); break; //done
			case 'A': no_async = true; break; //done
			case 'T': no_truecolor = true; break; //done
			case 't': force_truecolor = true; break; //done
			case 'F': no_tail_fade = true; break; //done
			case 'C': no_color = true; break; //done
			case 'c': force_color = true; break; //done
			case 'd': distinct_head = true; break; //done
			case 'B': no_bold = true; break; //done
			case 'b': all_bold = true; break; //done
			case 's': binary_charset = true; break; //done
			case '?': fprintf(stderr, "Consult %s --help for more information.\n", argv[0]); exit(42);
            		default: exit(42);
		}
	}
}
