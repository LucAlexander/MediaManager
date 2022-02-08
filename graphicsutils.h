#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <inttypes.h>

#include "../Entity-Component-System/Type-Safe-Hash-Map/hashMap.h"

struct v2;
struct v4;

enum RECT_DRAW_PARAMS{
	FILL=1,
	OUTLINE=2
};

typedef struct view{
	float x;
	float y;
	float px;
	float py;
	float pw;
	float ph;
}view;

typedef struct font{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	uint16_t kerning;
	uint16_t leading;
	float ptSize;
	float scale;
	SDL_Texture* glyphMap[128];
}font;

HASHMAP(FontMap, const char*, font)

typedef struct fontHandler{
	char* charList;
	char* activeFont;
	font* fnt;
	FontMap list;
}fontHandler;

typedef struct GraphicsHandler{
	SDL_Window* window;
	SDL_Renderer* renderer;
	view renderView;
	fontHandler fonts;
	uint16_t windowW;
	uint16_t windowH;
	float spriteScaleX;
	float spriteScaleY;
}GraphicsHandler;

void graphicsInit(uint16_t width, uint16_t height, const char* windowTitle);
void graphicsClose();

void handlerRenderViewInit();

SDL_Texture* getTexture(const char* src);

void renderSetView(view v);

void renderSetSpriteScale(float scaleX, float scaleY);
float scaleOnX(float val);
float scaleOnY(float val);

void toggleFullscreen();

struct v2 viewToWorldV2(struct v2 coords);
struct v2 worldToViewV2(struct v2 coords);
struct v2 viewToWorld(float x, float y);
struct v2 worldToView(float x, float y);

void renderFlip();
void renderClear();

void renderSetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

void formatDestRectToView(SDL_Rect* destRect);
void formatDestFRectToView(SDL_FRect* destRect);

enum BLITABLE_FLAGS{
	BLITABLE_VISIBLE=1,
	BLITABLE_FLIP_H=2,
	BLITABLE_FLIP_V=4
};

typedef struct Blitable{
	SDL_Texture* texture;
	SDL_Rect drawBound;
	uint32_t displayW;
	uint32_t displayH;
	int32_t textureW;
	int32_t textureH;
	uint8_t flags;
	double angle;
	SDL_FPoint center;
}Blitable;

void BlitableInitF(Blitable* blit, const char* source, uint32_t w, uint32_t h);
void BlitableInit(Blitable* blit, SDL_Texture* t, uint32_t w, uint32_t h);
void renderBlitable(Blitable* blit, float x, float y);
void renderBlitableV2(Blitable* blit, struct v2 pos);
void BlitableFree(Blitable* blit);

void blitSurface(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect destRect);
void blitSurfaceEX(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect destRect, double angle, SDL_Point* center, SDL_RendererFlip flip);
void blitSurfaceF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect destRect);
void blitSurfaceEXF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect destRect, double angle, SDL_FPoint* center, SDL_RendererFlip flip);

void drawLineV2(struct v2, struct v2);
void drawLine(float x, float y, float xx, float yy);

// DIMENSION [X, Y, WIDTH, HEIGHT] DRAWING
void drawRectV2(struct v2, struct v2, uint8_t p);
void drawRectV4(struct v4, uint8_t p);
void drawRect(float x1, float y1, float x2, float y2, uint8_t p);
// BOUNDS [LEFT TOP RIGHT BOTTOM] DRAWING
void drawRectV2B(struct v2, struct v2, uint8_t p);
void drawRectV4B(struct v4, uint8_t p);
void drawRectB(float x1, float y1, float x2, float y2, uint8_t p);

void drawTextV2(struct v2 pos, char* text);
void drawTextV2C(struct v2 pos, char* text, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void drawText(float x, float y, char* text);
void drawTextC(float x, float y, char* text, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void drawTextEX(float x, float y, int32_t n, ...);
void drawCharacter(char c, SDL_Rect* dest, float startX, float cSize, font* f);
void drawCharacterC(char c, SDL_Rect* dest, float startX, float cSize, font* f, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void resetFontGlypthBlend(font* f);
SDL_Texture* handleWhitespace(char c, SDL_Rect* dest, float startX, float cSize, font* f);

void textWidthCharEval(char c, uint32_t* longest, uint32_t* current);
float getTextWidth(const char* c);
float getTextHeight(const char* c);

void fontHandlerInit();
void loadFont(const char* src);
void loadFontC(const char* src, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void setFont(char* fnt);
void fontClose(font* f);
void fontHandlerClose();

#endif
