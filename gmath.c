#include "gmath.h"
#include "matrix.h"

#include <math.h>

/*
 * normalizes the vector
 * returns: void
*/

void normalize (double *vector)
{
    const double rat = sqrt (vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
    vector[0] /= rat, vector[1] /= rat, vector[2] /= rat;
}

/*
 * computes the dot product of the two vectors
 * returns: double, the product
*/

double dot_product (const double *a, const double *b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

/*
 * computes the surface normal of polygon starting at index i
 * returns: void, result is stored in out
*/

void calculate_normal (const matrix *polys, const int i, double *out)
{
    const double x0 = polys->mtrx[i][0];
    const double x1 = polys->mtrx[i+1][0];
    const double x2 = polys->mtrx[i+2][0];
    const double y0 = polys->mtrx[i][1];
    const double y1 = polys->mtrx[i+1][1];
    const double y2 = polys->mtrx[i+2][1];
    const double z0 = polys->mtrx[i][2];
    const double z1 = polys->mtrx[i+1][2];
    const double z2 = polys->mtrx[i+2][2];

    const double ax = x1-x0;
    const double ay = y1-y0;
    const double az = z1-z0;

    const double bx = x2-x0;
    const double by = y2-y0;
    const double bz = z2-z0;

    out[0] = ay*bz - az*by;
    out[1] = az*bx - ax*bz;
    out[2] = ax*by - ay*bx;
}
