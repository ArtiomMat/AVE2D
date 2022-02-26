// A math library based on the math.h library.

#pragma once

#include <math.h>

typedef float vec;
typedef vec vec3[3];

void vec3add(vec3 a, vec3 b, vec3 out);
void vec3sub(vec3 a, vec3 b, vec3 out);
void vec3mul(vec3 a, float b, vec3 out);
void vec3inv(vec3 a, vec3 out);

vec getmagnitude(vec3 a);
void normalized(vec3 a, vec3 out);
void crossproduct(vec3 a, vec3 b, vec3 out);
vec dotproduct(vec3 a, vec3 b);

void rotatevectoraround(vec3 a, vec3 b, vec3 c, float d);
