#ifndef COLOUR
#define COLOUR

#include <math.h>

void colour(float a, float min, float max, float *c);

void colour(float a, float min, float max, float *c) {
	float b = 10*(a-(max+min)/2)/(max-min);
	*(c) = fabs((b+1)/2)-1;
	*(c+1) = -fabs((b-1)/2)+2;
	*(c+2) = (1-b)/2;
}

#endif
