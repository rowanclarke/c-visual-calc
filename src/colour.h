#ifndef COLOUR
#define COLOUR

#include <math.h>

void colour(float a, float *r, float *g, float *b);

void colour(float a, float *r, float *g, float *b) {
	*r = fabs((a+1)/2)-1;
	*g = -fabs((a-1)/2)+2;
	*b = (1-a)/2;
}

#endif
