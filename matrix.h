#ifndef MATRIX_H_
#define MATRIX_H_

typedef double mxtype;
//for convience matricies are stored mx[H][W]

void print_matrix(const int w, const int h, const mxtype m[h][w]);
void ident(const int s, mxtype m[s][s]);
void matrix_mult(const int wa, const int ha , const mxtype a[ha][wa],
        const int wb, const int hb, mxtype b[hb][wb]);

void copy_matrix(const int w, const int h, const mxtype src[h][w], mxtype dest[h][w]);
//maybe some more if needed

#endif
