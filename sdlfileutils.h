#ifndef SDLFILEUTILS_H
#define SDLFILEUTILS_H

#include <SDL2/SDL.h>
#include "../Entity-Component-System/Type-Safe-Hash-Map/hashMap.h"

HASHMAP(TextureMap, const char*, SDL_Surface*)

typedef struct fileLoader{
	TextureMap textures;
}fileLoader;

void fileLoaderInit();

SDL_Surface* loadImage(const char* src);

void fileLoaderClose();

#endif
