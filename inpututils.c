#include "inpututils.h"
#include "mathutils.h"
#include "graphicsutils.h"

#include <string.h>

static input inp;

void inputInit(){
	memset(inp.heldKeys, 0, 256);
	memset(inp.pressedKeys, 0, 256);
	memset(inp.releasedKeys, 0, 256);
	memset(inp.heldMouseButtons, 0, 5);
	memset(inp.pressedMouseButtons, 0, 5);
	memset(inp.releasedMouseButtons, 0, 5);
	inp.scrollDir = 0;
}

void newInputFrame(){
	memset(inp.pressedKeys, 0, 256);
	memset(inp.releasedKeys, 0, 256);
	memset(inp.pressedMouseButtons, 0, 5);
	memset(inp.releasedMouseButtons, 0, 5);
	inp.scrollDir = 0;
}

void mouseScrollEvent(int8_t dir){
	inp.scrollDir = dir;
}

void mouseMoveEvent(int32_t x, int32_t y){
	inp.mouseX = x;
	inp.mouseY = y;
}

int32_t mouseX(){
	return scaleOnX(inp.mouseX);
}

int32_t mouseY(){
	return scaleOnY(inp.mouseY);
}

v2 mousePos(){
	v2 a = {mouseX(), mouseY()};
	return a;
}

void mouseUpEvent(const SDL_Event event){
	inp.releasedMouseButtons[event.button.button-1] = 1;
	inp.heldMouseButtons[event.button.button-1] = 0;
}

void mouseDownEvent(const SDL_Event event){
	inp.pressedMouseButtons[event.button.button-1] = 1;
	inp.heldMouseButtons[event.button.button-1] = 1;
}

void keyUpEvent(const SDL_Event event){
	inp.releasedKeys[event.key.keysym.scancode] = 1;
	inp.heldKeys[event.key.keysym.scancode] = 0;
}

void keyDownEvent(const SDL_Event event){
	inp.pressedKeys[event.key.keysym.scancode] = 1;
	inp.heldKeys[event.key.keysym.scancode] = 1;
}

uint8_t mouseScrolled(int8_t dir){
	return sign(dir)==sign(inp.scrollDir);
}

uint8_t mouseHeld(uint8_t button){
	return inp.heldMouseButtons[button-1];
}

uint8_t mousePressed(uint8_t button){
	return inp.pressedMouseButtons[button-1];
}

uint8_t mouseReleased(uint8_t button){
	return inp.releasedMouseButtons[button-1];
}

uint8_t keyHeld(const char* key){
	SDL_Scancode k = SDL_GetScancodeFromName(key);
	return inp.heldKeys[k];
}

uint8_t keyPressed(const char* key){
	SDL_Scancode k = SDL_GetScancodeFromName(key);
	return inp.pressedKeys[k];
}

uint8_t keyReleased(const char* key){
	SDL_Scancode k = SDL_GetScancodeFromName(key);
	return inp.releasedKeys[k];
}

void mouseInterrupt(uint8_t b){
	inp.pressedMouseButtons[b-1] = 0;
	inp.heldMouseButtons[b-1] = 0;
}

void keyInterrupt(const char* key){
	SDL_Scancode k = SDL_GetScancodeFromName(key);
	inp.pressedKeys[k] = 0;
	inp.heldKeys[k] = 0;
}
