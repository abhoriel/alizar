#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>

typedef struct Object_ {
	int x, y;
	SDL_Texture *texture;
	void (*tick)();
	struct Object_ *prev;
	struct Object_ *next;
	struct Object_ *tilePrev;
	struct Object_ *tileNext;
	struct Object_ *aiPrev;
	struct Object_ *aiNext;
} Object;

#endif
