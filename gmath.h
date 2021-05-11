#ifndef GMATH_H
#define GMATH_H

#include "matrix.h"

//vector functions
//A vector is represented as an array, {x,y,z}
void normalize (double *vector);
double dot_product (const double *a, const double *b);
void calculate_normal (const matrix *polys, const int i, double *out);

#endif
