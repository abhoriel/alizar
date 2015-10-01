#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "alizarin.h"
#include "log.h"
#include "tile.h"
#include "world.h"
#include "terrain.h"


static void subdivideDisplace(int *heightMap, const int width, const int height, unsigned int maxHeight, const float jaggedness, const unsigned int maxIterations);
static void offsetArea(int *heightMap, const unsigned width, const unsigned height, const float centreX, const float centreY, const float radius, const float amount);

static void createTerrainFromHeightMap(int *heightMap, int worldXSize, int worldYSize, int worldZSize);
// the limits MUST be powers of 2
void terrainGenerate(int worldXSize, int worldYSize, int worldZSize) {
	// diamond / square algorithm
	int *heightMap = mallocOrDie(worldXSize * worldYSize * sizeof(int));
	// flatten world
	for (int i = 0; i < (worldXSize * worldYSize); i++) {
		heightMap[i] = worldZSize / 2;
	}
	subdivideDisplace(heightMap, worldXSize, worldYSize, worldZSize, 0.7, 6);
	// ensure world is in bounds
	for (int i = 0; i < (worldXSize * worldYSize); i++) {
		if (heightMap[i] < 0) {
			heightMap[i] = 0;
		} else if (heightMap[i] >= worldZSize) { 
			heightMap[i] = worldZSize - 1;
		}
	}

	createTerrainFromHeightMap(heightMap, worldXSize, worldYSize - 1, worldZSize / 3);

	free(heightMap);
}

// very simple world generation from heightmap
static void createTerrainFromHeightMap(int *heightMap, int worldXSize, int worldYSize, int worldZSize) {
	const int waterLevel = 32;
	for (int y = 0; y < worldYSize; y++) {
		for (int x = 0; x < worldXSize; x++) {
			int height = heightMap[x + y * worldXSize];
			for (int z = 0; z < worldZSize; z++) {
				Tile *tile = worldGetTile(x, y, z);
				if (z < height) {
					tile->tileType = ROCK;
				} else if (z == height) {
					if (z <= waterLevel) {
						tile->tileType = SAND;
					} else {
						tile->tileType = SOIL;
					}
				} else {
					if (z <= waterLevel) {
						tile->tileType = WATER;
					} else {
						tile->tileType = EMPTY;
					}
				}
			}
		}
	}
}



// I wrote this code ages ago, I know it sucks. (I wrote this comment ages ago. It also sucks)
static void subdivideDisplace(int *heightMap, const int width, const int height, unsigned int maxDisplacement, const float jaggedness, const unsigned int maxIterations) {
	srand(time(NULL));
	signed int randomHeight;
	unsigned int sign, xInterval, yInterval, line, point;
	unsigned int powerOfTwo = 1;
	logWrite(LOG_INFO, "generating map (subdivide and displace)... ");
	for (unsigned int i = 0; i < maxIterations; ++i) {
		xInterval = width / powerOfTwo;
		yInterval = height / powerOfTwo;
		for (line = 0; line < powerOfTwo + 1; ++line) {
			for (point = 0; point < powerOfTwo; ++point) {
				randomHeight = rand() % maxDisplacement;
				sign = rand() % 2;
				if (sign != 0) {
					randomHeight = randomHeight * -1;
				}
				offsetArea(heightMap, width, height, (float)(xInterval * (((float)point) + 0.5)), yInterval * line, ((float)width) / powerOfTwo, randomHeight);
			}
		}
		for (line = 0; line < powerOfTwo; ++line) {
			for(point = 0; point < powerOfTwo + 1; ++point) {
				randomHeight = rand() % maxDisplacement;
				sign = rand() % 2;
				if (sign != 0) {
					randomHeight = randomHeight * -1;
				}
				offsetArea(heightMap, width, height, xInterval * point, (yInterval * (((float)line) + 0.5)), ((float)width) / powerOfTwo, randomHeight);
			}
		}
		powerOfTwo = powerOfTwo * 2;
		maxDisplacement = maxDisplacement * jaggedness;
	}
	logWrite(LOG_INFO, "done\n");
}

static void offsetArea(int *heightMap, const unsigned width, const unsigned height, const float centreX, const float centreY, const float radius, const float amount) {
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x)	{
			float distance = sqrtf(((centreX - x) * (centreX - x)) +
			                              ((centreY - y) * (centreY - y)));
			if (distance > radius) {
				distance = radius;
			}
			float offset = (radius - distance) * (amount / radius);
			int height = heightMap[x + y * width];
			height += offset;
			if (height < 0) {
				height = 0;
			}
			// FIXME: hardcoded value
			if (height > 127) {
				height = 127;
			}
			heightMap[x + y * width] = height;
		}
	}
}

#if 0
static void offsetArea(int *heightMap, const unsigned width, const unsigned height, const float centreX, const float centreY, const float radius, const float amount) {
	float distance;
	float offset;
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x)	{
			distance = (unsigned int)sqrtf(((centreX - x) * (centreX - x)) +
			                              ((centreY - y) * (centreY - y)));
			if (distance > radius) {
				distance = radius;
			}
			offset = (radius - distance) * (amount / radius);		
			if (offset > (255 - map_[0][x][y])) {
				map_[0][x][y] = ~0;
			} else if ((offset * -1) > map_[0][x][y]) {
				map_[0][x][y] = 0;
			} else {
				map_[0][x][y] = map_[0][x][y] + (signed char)offset;
			}
		}
	}
}
#endif

/*
static int isPowerOfTwo(unsigned int x) {
	return ((x != 0) && !(x & (x - 1)));
}
*/
