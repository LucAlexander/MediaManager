#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <inttypes.h>

#ifndef PI
#define PI 3.1415926
#endif

typedef struct v2{
	float x;
	float y;
}v2;

typedef struct v3{
	float x;
	float y;
	float z;
}v3;

typedef struct v4{
	float x;
	float y;
	float w;
	float h;
}v4;

float lenDirX(float len, int32_t dir);

float lenDirY(float len, int32_t dir);

int32_t sign(int32_t val);

float distancePoint(v2, v2);

uint32_t pointInRect(v2, v4);

uint32_t pointInRectB(v2, v4);

void approachZero(int32_t* val, int32_t amount);
void approachZeroF(float* val, float amount);

#endif
