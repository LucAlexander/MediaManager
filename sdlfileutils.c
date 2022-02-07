#include "sdlfileutils.h"

#include <SDL2/SDL_image.h>

HASHMAP_SOURCE(TextureMap, const char*, SDL_Surface*, hashS)

static fileLoader loader = {0};

void fileLoaderInit(){
	loader.textures = TextureMapInit();
}

SDL_Surface* loadImage(const char* src){
	SDL_Surface* item = TextureMapGet(&(loader.textures), src).val;
	if (item != NULL){
		return item;
	}
	item = IMG_Load(src);
	TextureMapPush(&(loader.textures), src, item);
	return item;
}

void fileLoaderClose(){
	TextureMapIterator it = TextureMapIteratorInit(&(loader.textures));
	while(TextureMapIteratorHasNext(&it)){
		SDL_FreeSurface(TextureMapIteratorNext(&it).val);
	}
	TextureMapFree(&(loader.textures));
}
