#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "alizarin.h"
#include "log.h"
#include "uthash.h"
#include "tile.h"

// 16x16 tiles: 720p, 80x45
static TileDescriptor *tileNewDescriptor(int tileType);
static TileDescriptor *tileNewBasicDescriptor(int tileType, const char *fn, SDL_Renderer *renderer);

TileDescriptor *tileDescriptors = NULL;
int nTileDescriptors = 0;

void tileInit(SDL_Renderer *renderer) {
	tileNewBasicDescriptor(EMPTY, NULL, renderer);
	tileNewBasicDescriptor(SOIL, "soil.png", renderer);
	tileNewBasicDescriptor(GRASS, "grass.png", renderer);
	tileNewBasicDescriptor(WATER, "water.png", renderer);
	tileNewBasicDescriptor(SAND, "sand.png", renderer);
	tileNewBasicDescriptor(ROCK, "rock.png", renderer);
}

void tileFini() {
	for (int i = 0; i < nTileDescriptors; i++) {
		if (tileDescriptors[i].texture != NULL) {
			SDL_DestroyTexture(tileDescriptors[i].texture);
		}
	}
	free(tileDescriptors);
}

TileDescriptor *tileGetDescriptor(int tileType) {
	assert(tileType < nTileDescriptors);
	return &tileDescriptors[tileType];
}

static TileDescriptor *tileNewBasicDescriptor(int tileType, const char *fn, SDL_Renderer *renderer) {
	SDL_Texture *texture = NULL;
	if (fn != NULL) {
		const char *dir = "assets/gfx/tiles/";
		char *path = mallocOrDie(strlen(dir) + strlen(fn) + 1);
		strcpy(path, dir);
		strcat(path, fn);
		texture = IMG_LoadTexture(renderer, path);
		free(path);
		if (texture == NULL) {
			logWrite(LOG_ERROR, "could not load tile image: %s\n", path);
			exit(EXIT_FAILURE);
		}
	}
	TileDescriptor *td = tileNewDescriptor(tileType);
	td->texture = texture;
	return td;
}


static TileDescriptor *tileNewDescriptor(int tileType) {
	if ((tileType + 1) > nTileDescriptors) {
		nTileDescriptors = tileType + 1;
		tileDescriptors = reallocOrDie(tileDescriptors, nTileDescriptors * sizeof(TileDescriptor));
	} else {
		assert(0 && "added tile with same tileType");
	}
	TileDescriptor *td = &tileDescriptors[tileType];
	td->tileType = tileType;
	return td;
}

