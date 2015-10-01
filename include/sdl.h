#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
int sdlInit(int w, int h, bool fullscreen);
void sdlFini();
void sdlMain();

SDL_Renderer *sdlGetRenderer();

#endif	// SDL_H
