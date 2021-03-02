#include "matrix.h"

#include <stdio.h>
#include <string.h>

//for convience matricies are stored mx[H][W]

/*
 * prints the matrix to stdout
 * returns: void
*/

void print_matrix(const int w, const int h, const mxtype m[h][w]) {
    int i,j;
    for(i = 0; i < h; i++) {
        for(j = 0; j < w; j++) {
            printf("%lf ", m[i][j]);
        }
        printf("\n");
    }
}

/*
 * overwrites SQUARE matrix m with identity matrix
 * returns: void
*/

void ident(const int s, mxtype m[s][s]) {
    int i,j;
    for(i = 0; i < s; i++) {
        for(j = 0; j < s; j++) {
            m[i][j] = 0.0;
        }
    }
    for(i = 0; i < s; i++) {
        m[i][i] = 1.0;
    }
}


/*
 * takes the src matrix and copies into the dest matrix, note the matrices must be the same size
 * returns: void
*/

void copy_matrix(const int w, const int h, const mxtype src[h][w], mxtype dest[h][w]) {
    memcpy(dest, src, sizeof(mxtype) * w * h);
}

/*
 * takes two matricies where multiplication is defined and such the the 
 * demensions of b are the same as the resultant and multiplies them placing
 * the result in b
 * returns: void
*/
void matrix_mult(const int wa, const int ha , const mxtype a[ha][wa],
        const int wb, const int hb, mxtype b[hb][wb]) {
    mxtype tmp[wb][ha];
    int i,j,k;
    for(i = 0; i < wb; i++) {
        for(j = 0; j < ha; j++) {
            tmp[i][j] = b[j][i];
        }
    }

    for(i = 0; i < ha; i++) {
        for(j = 0; j < wb; j++) {
            for(k = 0; k < hb; k++) {
                b[i][j] += a[i][k] * tmp[j][k];
            }
        }
    }
}
