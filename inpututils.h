#ifndef INPUTUTILS_H
#define INPUTUTILS_H

#include <SDL2/SDL.h>

#include <inttypes.h>

struct v2;

typedef struct input{
	uint8_t heldKeys[256];
	uint8_t pressedKeys[256];
	uint8_t releasedKeys[256];
	uint8_t heldMouseButtons[5];
	uint8_t pressedMouseButtons[5];
	uint8_t releasedMouseButtons[5];
	int8_t scrollDir;
	int32_t mouseX;
	int32_t mouseY;
}input;

void inputInit();

void newInputFrame();

void mouseMoveEvent(int32_t x, int32_t y);

int32_t mouseX();
int32_t mouseY();
struct v2 mousePos();

void mouseScrollEvent(int8_t);

void mouseUpEvent(const SDL_Event event);
void mouseDownEvent(const SDL_Event event);

void keyUpEvent(const SDL_Event event);
void keyDownEvent(const SDL_Event event);

uint8_t mouseScrolled(int8_t);

uint8_t mouseHeld(uint8_t);
uint8_t mousePressed(uint8_t);
uint8_t mouseReleased(uint8_t);

uint8_t keyHeld(const char*);
uint8_t keyPressed(const char*);
uint8_t keyReleased(const char*);

void mouseInterrupt(uint8_t);
void keyInterrupt(const char*);

#endif
