#ifndef MATH
#define MATH

#include <math.h>

void cos2(float *a, float *b);
void sin2(float *a, float *b);

void cos2(float *a, float *b) {
	*b = cos(*a)*cosh(*(a+1));
	*(b+1) = -sin(*a)*sinh(*(a+1));
}

void sin2(float *a, float *b) {
	*b = sin(*a)*cosh(*(a+1));
	*(b+1) = cos(*a)*sinh(*(a+1));
}

#endif

