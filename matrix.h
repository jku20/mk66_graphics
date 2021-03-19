#ifndef MATRIX_H_
#define MATRIX_H_

#define MAX_MATRIX_HEIGHT 4

//defines axis constants
typedef enum {XAXIS, YAXIS, ZAXIS} axis;

typedef double mxtype;
//matricies stored as arr[W][H]
typedef struct 
{
    int w, w_size;
    mxtype mtrx[][MAX_MATRIX_HEIGHT];
} matrix;

matrix *mk_matrix (const int w);
matrix *free_matrix (matrix *mtrx);
void cpy_matrix (const matrix *src, matrix *dest);
matrix *grow_matrix (matrix *mtrx, const int newcols);

void ident (matrix *mtrx);
matrix *mk_translate (const mxtype x, const mxtype y, const mxtype z);
matrix *mk_scale (const mxtype x, const mxtype y, const mxtype z);
matrix *mk_rot (double theta, const axis a);

void print_matrix (const matrix *mtrx);
void matrix_mult (const matrix *a, matrix *b);
matrix *mx_resize (matrix *mtrx, const int n);
matrix *mx_reserve (matrix *mtrx, const int n);
//maybe some more if needed

#endif
