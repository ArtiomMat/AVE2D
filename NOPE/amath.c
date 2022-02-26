#include "amath.h"

vec getmagnitude(vec3 a)
{
	return sqrt(a[0]*a[0] + 
				a[1]*a[1] +
				a[2]*a[2]);
}

void normalized(vec3 a, vec3 out)
{
	vec mag = getmagnitude(a);
	out[0] = a[0]/mag;
	out[1] = a[1]/mag;
	out[2] = a[2]/mag;
}

void vec3add(vec3 a, vec3 b, vec3 out)
{
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
}

void vec3sub(vec3 a, vec3 b, vec3 out)
{
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
}

void vec3div(vec3 a, float b, vec3 out)
{
	out[0] = a[0] / b;
	out[1] = a[1] / b;
	out[2] = a[2] / b;
}

void vec3mul(vec3 a, float b, vec3 out)
{
	out[0] = a[0] * b;
	out[1] = a[1] * b;
	out[2] = a[2] * b;
}

void vec3inv(vec3 a, vec3 out)
{
	out[0] = -a[0];
	out[1] = -a[1];
	out[2] = -a[2];
}

// Rotate a vector by a quaternion
void vec3rotatearound(vec3 a, vec3 b, vec3 c, float d)
{
	// TO BE IMPLEMENTED.
}

void crossproduct(vec3 a, vec3 b, vec3 out)
{
	out[0] = a[1]*b[2] - a[2]*b[1];
	out[1] = a[2]*b[0] - a[0]*b[2];
	out[2] = a[0]*b[1] - a[1]*b[0];
}

vec dotproduct(vec3 a, vec3 b)
{
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}
