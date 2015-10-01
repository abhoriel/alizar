#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include "log.h"
#include "sdl.h"
#include "tile.h"
#include "world.h"

static void usage(char **argv);

bool verbose = 0;

static const char *opt_string = "t:?";

static const struct option long_opts[] = {
	{"help", no_argument, NULL, '?'},
	{"verbose", no_argument, NULL, 'v'},
	{"fullscreen", no_argument, NULL, 'f'},
	{"width", required_argument, NULL, 'w'},
	{"height", required_argument, NULL, 'h'},
	{NULL, no_argument, NULL, 0}
};


int main(int argc, char **argv) {
	int opt = 0;
	int long_index;
	bool fullscreen = 0;
	int w = 0, h = 0;
	opt = getopt_long(argc, argv, opt_string, long_opts, &long_index);
	while(opt != -1) {
		switch(opt) {
			case 'v':
				verbose = 1;
				break;
			case '?':
				usage(argv);
				return EXIT_FAILURE;
			case 'w':
				w = atoi(optarg);
				break;
			case 'h':
				h = atoi(optarg);
				break;
			case 'f':
				fullscreen = 1;
				break;
			case 0:     /* long option without a short arg */
				//if(strcmp("randomize", long_opts[long_index].name) == 0) {
				//}
				break;
			default:
				// not reached
				break;
		}
		opt = getopt_long(argc, argv, opt_string, long_opts, &long_index);
	}

	sdlInit(w, h, fullscreen);
	tileInit(sdlGetRenderer());
	worldInit();

	sdlMain();

	worldFini();
	tileFini();
	sdlFini();
	return 0;
}


static void usage(char **argv) {
	logWrite(LOG_ERROR, "Usage: %s\n", argv[0]);
}


void *mallocOrDie(size_t size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		logWrite(LOG_ERROR, "fatal: memory allocation failure\n");
		exit(EXIT_FAILURE);
	}
	return ptr;
}

void *reallocOrDie(void *old, size_t size) {
	void *ptr = realloc(old, size);
	if (ptr == NULL) {
		logWrite(LOG_ERROR, "fatal: memory allocation failure\n");
		exit(EXIT_FAILURE);
	}
	return ptr;
}
