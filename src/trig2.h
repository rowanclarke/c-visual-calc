#ifndef MATH
#define MATH

#include <math.h>

void ncos(float *a, float *b);
void nsin(float *a, float *b);

void ncos(float *a, float *b) {
	*b = cos(*a)*cosh(*(a+1));
	*(b+1) = -sin(*a)*sinh(*(a+1));
}

void nsin(float *a, float *b) {
	*b = sin(*a)*cosh(*(a+1));
	*(b+1) = cos(*a)*sinh(*(a+1));
}

#endif

