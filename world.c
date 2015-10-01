#include <assert.h>
#include <SDL2/SDL.h>
#include "alizarin.h"
#include "log.h"
#include "tile.h"
#include "terrain.h"
#include "world.h"

int worldXSize = 128;
int worldYSize = 128;
int worldZSize = 128;

Tile *world = NULL;

void worldInit() {
	world = mallocOrDie(worldXSize * worldYSize * worldZSize * sizeof(Tile));
	terrainGenerate(worldXSize, worldYSize, worldZSize);
}

void worldFini() {
	free(world);
}

void worldSave() {

}

void worldLoad() {

}

void worldRender(SDL_Renderer *renderer, int worldX, int worldY, int worldZ, int xRes, int yRes, int tileWidth, int tileHeight) {
	int startX = 0 - (worldX % tileWidth);
	int startY = 0 - (worldY % tileHeight);
	for (int y = startY; y < yRes; y += tileHeight) {
		int tileY = (worldY + y) / tileHeight;
		if (tileY < 0) {
			continue;
		}
		for (int x = startX; x < xRes; x += tileWidth) {
			int tileX = (worldX + x) / tileWidth;
			if (tileX < 0) {
				continue;
			}
			Tile *tile = worldGetTile(tileX, tileY, worldZ);
			TileDescriptor *td = tileGetDescriptor(tile->tileType);
			if (td->texture != NULL) {
				SDL_Rect dst;
				dst.x = x;
				dst.y = y;
				dst.w = tileWidth;
				dst.h = tileHeight;
				SDL_RenderCopy(renderer, td->texture, NULL, &dst);
			}
		}
	}
}

