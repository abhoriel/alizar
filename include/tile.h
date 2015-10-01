#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>
#include "uthash.h"

#define TILE_WIDTH 16 
#define TILE_HEIGHT 16


typedef struct TileDescriptor_ {
	int tileType;
	SDL_Texture *texture;
	UT_hash_handle hh;
} TileDescriptor;

typedef struct Tile_ {
	int tileType;
	void *data;
} Tile;

typedef enum {
	EMPTY = 0,
	SOIL,
	GRASS,
	WATER,
	SAND,
	ROCK
} TileType;

void tileInit(SDL_Renderer *renderer);
void tileFini();

TileDescriptor *tileGetDescriptor(int tileType);

#endif	// TILE_H
