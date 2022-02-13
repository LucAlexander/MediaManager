#define _GNU_SOURCE
#include "graphicsutils.h"
#include "mathutils.h"
#include "sdlfileutils.h"
#include <string.h>

#include <stdio.h>

HASHMAP_SOURCE(FontMap, const char*, TTF_Font*, hashS)

static GraphicsHandler ghandle;

void graphicsInit(uint16_t width, uint16_t height, const char* windowTitle){
	ghandle.renderScale = RENDER_SCALE_NEAREST;
	ghandle.window = NULL;
	ghandle.renderer = NULL;
	handlerRenderViewInit();
	ghandle.windowW = 1920;
	ghandle.windowH = 1080;
	ghandle.spriteScaleX = 1;
	ghandle.spriteScaleY = 1;
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	if (ghandle.window != NULL){
		SDL_DestroyWindow(ghandle.window);
	}
	if (ghandle.renderer != NULL){
		SDL_DestroyRenderer(ghandle.renderer);
	}
	SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &(ghandle.window), &(ghandle.renderer));
	SDL_SetWindowTitle(ghandle.window, windowTitle);
	view defaultView = {0, 0, 0, 0, width, height};
	ghandle.windowW = width;
	ghandle.windowH = height;
	renderSetView(defaultView);
	fileLoaderInit();
	fontHandlerInit();
}

void graphicsClose(){
	fontHandlerClose();
	fileLoaderClose();
	TTF_Quit();
	SDL_DestroyWindow(ghandle.window);
	SDL_DestroyRenderer(ghandle.renderer);
	ghandle.window = NULL;
	ghandle.renderer = NULL;
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
}

void handlerRenderViewInit(){
	ghandle.renderView.x = 0;
	ghandle.renderView.y = 0;
	ghandle.renderView.px = 0;
	ghandle.renderView.py = 0;
	ghandle.renderView.pw = 0;
	ghandle.renderView.ph = 0;
}

SDL_Texture* getTexture(const char* src){
	SDL_Texture* item = SDL_CreateTextureFromSurface(ghandle.renderer, loadImage(src));
	if (item == NULL){
		printf("[!] Unable to load image \'%s\'\n",src);
	}
	return item;
}

void renderSetSpriteScale(float scaleX, float scaleY){
	ghandle.spriteScaleX = scaleX;
	ghandle.spriteScaleY = scaleY;
	SDL_RenderSetScale(ghandle.renderer, ghandle.spriteScaleX, ghandle.spriteScaleY);
}

float scaleOnX(float val){
	return val/ghandle.spriteScaleX;
}

float scaleOnY(float val){
	return val/ghandle.spriteScaleY;
}

void renderSetView(view v){
	ghandle.renderView = v;
	const SDL_Rect port = {v.px, v.py, v.pw, v.ph};
	SDL_RenderSetViewport(ghandle.renderer, &port);
	SDL_RenderSetLogicalSize(ghandle.renderer, v.pw, v.ph);
}

void renderSetScaleQuality(RENDER_SCALE_QUALITY hint){
	ghandle.renderScale = hint;
}

void toggleFullscreen(){
	uint32_t flags = (SDL_GetWindowFlags(ghandle.window) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (SDL_SetWindowFullscreen(ghandle.window, flags) < 0){
		printf("[!] Toggling fullscreen mode failed\n");
		return;
	}
	renderSetView(ghandle.renderView);
	if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0){
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, (const char*)ghandle.renderScale);
	}
}

v2 viewToWorldV2(v2 coords){
	coords.x += ghandle.renderView.x;
	coords.y += ghandle.renderView.y;
	return coords;
}

v2 worldToViewV2(v2 coords){
	coords.x -= ghandle.renderView.x;
	coords.y -= ghandle.renderView.y;
	return coords;
}

v2 viewToWorld(float x, float y){
	v2 a = {x,y};
	return viewToWorldV2(a);
}

v2 worldToView(float x, float y){
	v2 a = {x,y};
	return worldToViewV2(a);
}

void viewToWorldV2Ptr(struct v2* coords){
	coords->x += ghandle.renderView.x;
	coords->y += ghandle.renderView.y;
}

void worldToViewV2Ptr(struct v2* coords){
	coords->x -= ghandle.renderView.x;
	coords->y -= ghandle.renderView.y;
}

void viewToWorldPtr(float* x, float* y){
	if (x!=NULL){
		*x += ghandle.renderView.x;
	}
	if (y!=NULL){
		*y += ghandle.renderView.y;
	}
}

void worldToViewPtr(float* x, float* y){
	if (x!=NULL){
		*x -= ghandle.renderView.x;
	}
	if (y!=NULL){
		*y -= ghandle.renderView.y;
	}
}

void renderFlip(){
	SDL_RenderPresent(ghandle.renderer);
}

void renderClear(){
	SDL_RenderClear(ghandle.renderer);
}

void renderSetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	SDL_SetRenderDrawColor(ghandle.renderer, r, g, b, a);
}

void formatDestRectToView(SDL_Rect* destRect){
	destRect->x-=ghandle.renderView.x;
	destRect->y-=ghandle.renderView.y;
}

void formatDestFRectToView(SDL_FRect* destRect){
	destRect->x-=ghandle.renderView.x;
	destRect->y-=ghandle.renderView.y;
}

void BlitableInitF(Blitable* blit, const char* source, uint32_t w, uint32_t h){
	BlitableInit(blit, getTexture(source), w, h);
}

void BlitableInit(Blitable* blit, SDL_Texture* t, uint32_t w, uint32_t h){
	blit->texture = t;
	SDL_Rect bound = {0, 0, w, h};
	blit->drawBound = bound;
	blit->displayW = w;
	blit->displayH = h;
	SDL_QueryTexture(t, NULL, NULL, &(blit->textureW), &(blit->textureH));
	blit->flags = BLITABLE_VISIBLE;
	blit->angle = 0;
	SDL_FPoint mid = {w/2, h/2};
	blit->center = mid;
}

void renderBlitable(Blitable* blit, float x, float y){
	if (!(blit->flags & BLITABLE_VISIBLE)){
		return;
	}
	SDL_RendererFlip flipArg = SDL_FLIP_NONE;
	if (blit->flags & BLITABLE_FLIP_H){
		flipArg |= SDL_FLIP_HORIZONTAL;
		flipArg &= ~(SDL_FLIP_NONE);
	}
	if (blit->flags & BLITABLE_FLIP_V){
		flipArg |= SDL_FLIP_VERTICAL;
		flipArg &= ~(SDL_FLIP_NONE);
	}
	SDL_FRect dest = {x-blit->center.x, y-blit->center.y, blit->displayW, blit->displayH};
	blitSurfaceEXF(blit->texture, &(blit->drawBound), dest, blit->angle, &(blit->center), flipArg);
}

void renderBlitableV2(Blitable* blit, v2 pos){
	renderBlitable(blit, pos.x, pos.y);
}

void BlitableFree(Blitable* blit){
	SDL_DestroyTexture(blit->texture);
	blit->texture = NULL;
}

void blitSurface(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect destRect){
	formatDestRectToView(&destRect);
	SDL_RenderCopy(ghandle.renderer, texture, srcRect, &destRect);
}

void blitSurfaceEX(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect destRect, double angle, SDL_Point* center, SDL_RendererFlip flip){
	formatDestRectToView(&destRect);
	SDL_RenderCopyEx(ghandle.renderer, texture, srcRect, &destRect, angle, center, flip);
}

void blitSurfaceF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect destRect){
	formatDestFRectToView(&destRect);
	SDL_RenderCopyF(ghandle.renderer, texture, srcRect, &destRect);
}

void blitSurfaceEXF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect destRect, double angle, SDL_FPoint* center, SDL_RendererFlip flip){
	formatDestFRectToView(&destRect);
	SDL_RenderCopyExF(ghandle.renderer, texture, srcRect, &destRect, angle, center, flip);
}

void drawLineV2(v2 a, v2 b){
	drawLine(a.x, a.y, b.x, b.y);
}

void drawLine(float x, float y, float xx, float yy){
	SDL_RenderDrawLine(ghandle.renderer, x-ghandle.renderView.x, y-ghandle.renderView.y, xx-ghandle.renderView.x, yy-ghandle.renderView.y);
}

void drawRectV2(v2 a, v2 b, uint8_t p){
	drawRect(a.x, a.y, b.x, b.y, p);
}

void drawRectV4(v4 r, uint8_t p){
	drawRect(r.x, r.y, r.w, r.h, p);
}

void drawRect(float x1, float y1, float x2, float y2, uint8_t p){
	SDL_FRect bound = {
		x1-ghandle.renderView.x,
		y1-ghandle.renderView.y,
	       	x2,
	       	y2
	};
	if (p & FILL){
		SDL_RenderFillRectF(ghandle.renderer, &bound);
	}
	if (p & OUTLINE){
		SDL_RenderDrawRectF(ghandle.renderer, &bound);
	}
}

void drawRectV2B(v2 a, v2 b, uint8_t p){
	drawRectB(a.x, a.y, b.x, b.y, p);
}

void drawRectV4B(v4 r, uint8_t p){
	drawRectB(r.x, r.y, r.w, r.h, p);
}

void drawRectB(float x1, float y1, float x2, float y2, uint8_t p){
	SDL_FRect bound = {
		x1-ghandle.renderView.x,
		y1-ghandle.renderView.y,
	       	x2-x1-ghandle.renderView.x,
	       	y2-y1-ghandle.renderView.y
	};
	if (p & FILL){
		SDL_RenderFillRectF(ghandle.renderer, &bound);
	}
	if (p & OUTLINE){
		SDL_RenderDrawRectF(ghandle.renderer, &bound);
	}
}

void fontHandlerInit(){
	ghandle.fonts.activeFont = "";
	ghandle.fonts.fnt = NULL;
	ghandle.fonts.list = FontMapInit();
}

void loadFont(const char* src, const char* name){
	TTF_Font* f = FontMapGet(&(ghandle.fonts.list), name).val;
	if (f != NULL){
		return;
	}
	f = TTF_OpenFont(src, 16);
	if (f == NULL){
		TTF_CloseFont(f);
		f = NULL;
		return;
	}
	FontMapPush(&(ghandle.fonts.list), name, f);
}

void setFont(char* name){
	TTF_Font* temp = FontMapGet(&(ghandle.fonts.list), name).val;
	if (temp == NULL){
		printf("[!] No font %s is loaded, and cannot be set to active font\n",name);
		return;
	}
	ghandle.fonts.activeFont = name;
	ghandle.fonts.fnt = temp;
}

void fontHandlerClose(){
	FontMapIterator it = FontMapIteratorInit(&(ghandle.fonts.list));
	while(FontMapIteratorHasNext(&it)){
		TTF_CloseFont(FontMapGet(&(ghandle.fonts.list), FontMapIteratorNext(&it).key).val);
	}
	FontMapFree(&(ghandle.fonts.list));
}

void drawTextV2(v2 pos, const char* text){
	drawText(pos.x, pos.y, text);
}

void drawTextV2C(v2 pos, const char* text, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	drawTextC(pos.x, pos.y, text, r, g, b, a);
}

void drawText(float x, float y, const char* text){
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(ghandle.renderer, &r, &g, &b, &a);
	drawTextC(x, y, text, r, g, b, a);
}

void drawTextC(float x, float y, const char* text, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	if (text==NULL){
		return;
	}
	SDL_Color c = {r, g, b};
	SDL_Surface* surf = TTF_RenderText_Solid(ghandle.fonts.fnt, text, c);
	SDL_Texture* t = SDL_CreateTextureFromSurface(ghandle.renderer, surf);
	SDL_SetTextureAlphaMod(t, a);
	int w, h;
	TTF_SizeText(ghandle.fonts.fnt, text, &w, &h);
	SDL_Rect dest = {x, y, w, h};
	blitSurface(t, NULL, dest);
	SDL_FreeSurface(surf);
	SDL_DestroyTexture(t);
}

void drawTextEX(float x, float y, int32_t n, ...){
	va_list args;
	va_start(args, n);
	const char* text;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
	uint32_t width = 0;
	while(n > 0){
		text = va_arg(args, const char*);
		red = va_arg(args,int);
		green = va_arg(args,int);
		blue = va_arg(args,int);
		alpha = va_arg(args,int);
		n--;
		drawTextC(x+width, y, text, red, green, blue, alpha);
		width += getTextWidth(text);
	}
	va_end(args);
}

uint32_t getTextWidth(const char* c){
	uint32_t w;
	TTF_SizeText(ghandle.fonts.fnt, c, &w, NULL);
	return w;
}

uint32_t getTextHeight(const char* c){
	uint32_t h;
	TTF_SizeText(ghandle.fonts.fnt, c, &h, NULL);
	return h;
}

void queryTextSize(const char* text, uint32_t* w, uint32_t* h){
	TTF_SizeText(ghandle.fonts.fnt, text, w, h);
}
