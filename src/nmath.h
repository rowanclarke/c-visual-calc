#ifndef NMATH
#define NMATH

#include <math.h>

void add(float *a, float *b, float *c, int n);
void subtract(float *a, float *b, float *c, int n);
void conjugate(float *a, float *b, int n);
void multiply(float *a, float *b, float *c, int n);
void reciprocal(float *b, float *c, int n);

void add(float *a, float *b, float *c, int n) {
	int i;
	for (i = 0; i < n; i++) *(c+i) = *(a+i)+*(b+i);
}

void subtract(float *a, float *b, float *c, int n) {
	int i;
	for (i = 0; i < n; i++) *(c+i) = *(a+i)-*(b+i); 
}

void conjugate(float *a, float *b, int n) {
	int i;
	for (i = 0; i < n; i++) *(b+i) = (i > 0) ? -*(a+i) : *(a+i);
}

void multiply(float *a, float *b, float *c, int n) {
	if (n == 1) {
		*c = (*a)*(*b);
	}
	else {
		int m = n/2;
		int i;
		float *f = calloc(6*m, sizeof(float));
		conjugate(b+m, f+2*m, m);
		conjugate(b, f+5*m, m);
		multiply(a,b,f,m);
		multiply(f+2*m,a+m,f+m,m);
		multiply(b+m,a,f+3*m,m);
		multiply(a+m,f+5*m,f+4*m,m);
		for (i = 0; i < m; i++) {
			*(c+i)=*(f+i)-*(f+m+i);
			*(c+m+i)=*(f+3*m+i)+*(f+4*m+i); 
		}
		free(f);
	}
}

void reciprocal(float *a, float *b, int n) {
	int i;
	conjugate(a, b, n);
	float k = 0;
	for (i = 0; i < n; i++) k += a[i] * a[i];
	for (i = 0; i < n; i++) b[i] /= k;
}

#endif
