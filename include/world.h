#ifndef WORLD_H
#define WORLD_H

#include <assert.h>
#include "tile.h"

void worldInit();
void worldFini();
void worldGenerate();

static inline Tile *worldGetTile(int x, int y, int z) {
	extern Tile *world;
	extern int worldXSize;
	extern int worldYSize;
	return &world[x + (y * worldXSize) + (z * worldXSize * worldYSize)];
}

static inline Tile *worldGetTileSafe(int x, int y, int z) {
	extern Tile *world;
	extern int worldXSize;
	extern int worldYSize;
	extern int worldZSize;
	assert((x >= 0) && (y >= 0) && (z >= 0));
	assert((x < worldXSize) && (y < worldYSize) && (z < worldZSize));
	return &world[x + (y * worldXSize) + (z * worldXSize * worldYSize)];
}


void worldRender(SDL_Renderer *renderer, int worldX, int worldY, int worldZ, int xRes, int yRes, int tileWidth, int tileHeight);

#endif	// WORLD_H

