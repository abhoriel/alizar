#include <SDL2/SDL.h>
#include <errno.h>
#include <stdbool.h>
#include "world.h"
#include "log.h"
#include "sdl.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static int width, height;

int sdlInit(int w, int h, bool fullscreen) {
	Uint32 flags = 0;
	if (fullscreen) {
		if ((w == 0) && (h == 0)) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		} else {
			flags |= SDL_WINDOW_FULLSCREEN;
		}
	} else {
		flags |= SDL_WINDOW_RESIZABLE;
	}
	// if width and height are unspecified, try defaulting to 720p
	if (w == 0) {
		w = 1024;
	}
	if (h == 0) { 
		h = 786;
	}

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		logWrite(LOG_ERROR, "could not initialise sdl: %s\n", SDL_GetError());
		return -1;
	}
	
	window = SDL_CreateWindow("alizarin", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);
	if(window == NULL) {
		logWrite(LOG_ERROR, "sdl window could not be created: %s\n", SDL_GetError());
		return -1;
	}
	
	SDL_SetWindowGrab(window, SDL_TRUE);

	SDL_GetWindowSize(window, &width, &height);

	//width = w;
	//height = h;

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		logWrite(LOG_ERROR, "sdl renderer could not be created: %s\n", SDL_GetError());
		return -1;
	}
	//SDL_RenderGetLogicalSize(renderer, &width, &height);
	//logWrite(LOG_INFO, "sdl texture created (%dx%d)\n", width, height);
	
	SDL_SetWindowTitle(window, "alizarin");
		
	return 0;
}

void sdlFini() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void sdlMain() {
	int quit = 0;
	SDL_Event event;
	
	int leftMouseButtonDown = 0;
	int rightMouseButtonDown = 0;
	int mouseX = 0;
	int mouseY = 0;

	int worldX = 0;
	int worldY = 0;
	int worldZ = 0;
	int tileWidth = TILE_WIDTH;
	int tileHeight = TILE_HEIGHT;
	
	extern int worldXSize;
	extern int worldYSize;
	extern int worldZSize;
	int worldXLimit = worldXSize - 1;
	int worldYLimit = worldYSize - 1;
	int worldZLimit = worldZSize - 1;
	
	while (!quit) {
		SDL_RenderClear(renderer);
		//SDL_RenderCopy(renderer, texture, NULL, NULL);
		worldRender(renderer, worldX, worldY, worldZ, width, height, tileWidth, tileHeight);
		SDL_RenderPresent(renderer);
		
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
						leftMouseButtonDown = 0;
					if (event.button.button == SDL_BUTTON_RIGHT)
						rightMouseButtonDown = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT)
						leftMouseButtonDown = 1;
					if (event.button.button == SDL_BUTTON_RIGHT)
						rightMouseButtonDown = 1;
					break;
				case SDL_MOUSEMOTION:
					mouseX = event.motion.x;
					mouseY = event.motion.y;
					//logWrite(LOG_INFO, "mouse %d %d\n", mouseX, mouseY);
					break;
				case SDL_MOUSEWHEEL:
					if (event.wheel.y < 0) {
						if (worldZ > 0) {
							worldZ--;
						}
						logWrite(LOG_INFO, "worldZ: %d\n", worldZ);
					} else {
						if ((worldZ + 1) < worldZLimit) {
							worldZ++;
						}
						logWrite(LOG_INFO, "worldZ: %d\n", worldZ);
					}
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							quit = 1;
							break;
						case SDLK_UP:
							worldY -= 8;
							break;
						case SDLK_DOWN:
							worldY += 8;
							break;
						case SDLK_RIGHT:
							worldX += 8;
							break;
						case SDLK_LEFT:
							worldX -= 8;
							break;
					}
			}
			if (event.type == SDL_WINDOWEVENT) {
				switch (event.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						width = event.window.data1;
						height = event.window.data2;
						break;
				}
			}
		}
		
		if (leftMouseButtonDown || rightMouseButtonDown) {
			(void) mouseX; (void) mouseY;
			// do something
		}


		// scroll the screen
		if (mouseX == 0) {
			worldX -= 8;
		}
		if (mouseX == width) {
			worldX += 8;
		}
		if (mouseY == 0) {
			worldY -= 8;
		}
		if (mouseY == width) {
			worldY += 8;
		}

		if (worldX < 0) {
			worldX = 0;
		} else if (worldX > ((worldXLimit * tileWidth) - width)) {
			worldX = (worldXLimit * tileWidth) - width;
		}
		if (worldY < 0) {
			worldY = 0;
		} else if (worldY > (worldYLimit * tileHeight) - height) {
			worldY = (worldYLimit * tileHeight) - height;
		}
		SDL_Delay(10);
	}
}


SDL_Renderer *sdlGetRenderer() {
	return renderer;
}
