#include "matrix.h"

//defines axis constants (I don't know why I need this in both the .h and .c file for stuff to compile)
typedef enum {XAXIS, YAXIS, ZAXIS} axis;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


//matricies stored as arr[W][H]

/*
 * makes a matrix and returns it
 * returns: pointer to made matrix
*/
matrix *mk_matrix (const int w)
{

    int ws=1;
    while (ws < w) ws <<= 1;

    matrix *m = calloc (1,sizeof (matrix) + sizeof (mxtype[ws][MAX_MATRIX_HEIGHT]));

    m->w = w; m->w_size = ws;

    return m;
}

/*
 * takes the src matrix and copies into the dest matrix, note the matrices must be the same size
 * returns: void
*/

inline void cpy_matrix (const matrix *src, matrix *dest)
{
    memcpy (dest->mtrx, src->mtrx, sizeof (mxtype) * src->w * MAX_MATRIX_HEIGHT);
}

/*
 * take a matrix and adds the wanted amount of junk columns
 * returns: pointer to the matrix
*/

matrix *grow_matrix (matrix *mtrx, const int newcols)
{
    mtrx->w += newcols;

    if (mtrx->w > mtrx->w_size)
    {
        int nsize = mtrx->w_size;
        while (nsize < mtrx->w) nsize <<= 1;
        mtrx->w_size = nsize;

        return realloc (mtrx, sizeof (matrix) + sizeof (mxtype[nsize][MAX_MATRIX_HEIGHT]));
    }
    return mtrx;
}

/*
 * reserves space for a matrix of a certain size
 * returns: pointer to the matrix
*/

matrix *mx_reserve (matrix *mtrx, const int n) 
{
    const int old_w = mtrx->w;

    if (mtrx->w_size < n) mtrx = grow_matrix (mtrx, n);
    mtrx->w = old_w;

    return mtrx;
}

/*
 * resizes space for a matrix of a certain size
 * returns: pointer to the matrix
*/

matrix *mx_resize (matrix *mtrx, const int n) 
{
    if (n > mtrx->w) 
    {
        mtrx = grow_matrix (mtrx, n-mtrx->w);
    }
    mtrx->w = n;

    return mtrx;
}


/*
 * frees a matrix and returns null
 * returns: null
*/

matrix *free_matrix (matrix *mtrx) 
{
    free (mtrx);
    return NULL;
}

/*
 * prints the matrix to stdout
 * returns: void
*/

void print_matrix (const matrix *mtrx) 
{
    int w = mtrx->w;
    int i,j;
    for (i = 0; i < MAX_MATRIX_HEIGHT; i++) 
    {
        for (j = 0; j < w; j++) 
        {
            printf ("%lf ", mtrx->mtrx[j][i]);
        }
        printf ("\n");
    }
}

/*
 * overwrites SQUARE matrix m with identity matrix
 * returns: void
*/

void ident (matrix *m) 
{
    int i,j;
    for (i = 0; i < MAX_MATRIX_HEIGHT; i++) 
    {
        for (j = 0; j < MAX_MATRIX_HEIGHT; j++) 
        {
            m->mtrx[i][j] = 0.0;
        }
    }
    for (i = 0; i < MAX_MATRIX_HEIGHT; i++) 
    {
        m->mtrx[i][i] = 1.0;
    }
}

/*
 * creates and returns a translation matrix, translated by x,y,z
 * returns: translation matrix
*/

matrix *mk_translate (const mxtype x, const mxtype y, const mxtype z)
{
    matrix *mout = mk_matrix (4);
    ident (mout);

    mout->mtrx[0][3] = x;
    mout->mtrx[1][3] = y;
    mout->mtrx[2][3] = z;

    return mout;
}

/*
 * creates and returns a dilation matrix, dilated by x,y,z
 * returns: dilation matrix
*/

matrix *mk_scale (const mxtype x, const mxtype y, const mxtype z)
{
    matrix *mout = mk_matrix (4);

    mout->mtrx[0][0] = x;
    mout->mtrx[1][1] = y;
    mout->mtrx[2][2] = z;
    mout->mtrx[3][3] = 1.0;

    return mout;
}

/*
 * creates and returns a rotation matrix, rotation by theta in the specified axis
 * returns: rotation matrix
*/

matrix *mk_rot (const double theta, const axis a)
{
    matrix *mout = mk_matrix (4);

    mout->mtrx[3][3] = 1.0;
    if (a == ZAXIS)
    {
        mout->mtrx [0][0] =  cos (theta);
        mout->mtrx [0][1] = -sin (theta);
        mout->mtrx [1][0] =  sin (theta);
        mout->mtrx [1][1] =  cos (theta);

        mout->mtrx[2][2] = 1.0;
    } else if (a == XAXIS)
    {
        mout->mtrx[1][1] =  cos (theta);
        mout->mtrx[1][2] = -sin (theta);
        mout->mtrx[2][1] =  sin (theta);
        mout->mtrx[2][2] =  cos (theta);

        mout->mtrx[0][0] = 1.0;
    } else 
    {
        mout->mtrx[0][0] =  cos (theta);
        mout->mtrx[0][2] =  sin (theta);
        mout->mtrx[2][2] =  cos (theta);
        mout->mtrx[2][1] = -sin (theta);

        mout->mtrx[1][1] = 1.0;
    }

    return mout;
}

/*
 * takes two matricies where multiplication is defined and such the the 
 * demensions of b are the same as the resultant and multiplies them placing
 * the result in b
 * returns: void
*/

void matrix_mult (const matrix *a, matrix *b) 
{
    mxtype tmp_b[b->w][MAX_MATRIX_HEIGHT];
    mxtype tmp_a[MAX_MATRIX_HEIGHT][a->w];

    int w_b = b->w, w_a = a->w;

    int i,j,k;
    for (i = 0; i < w_b; i++) 
    {
        for (j = 0; j < MAX_MATRIX_HEIGHT; j++) 
        {
            tmp_b[i][j] = b->mtrx[i][j];
        }
    }
    for (i = 0; i < w_a; i++) 
    {
        for (j = 0; j < MAX_MATRIX_HEIGHT; j++) 
        {
            tmp_a[j][i] = a->mtrx[i][j];
        }
    }

    memset (b->mtrx, 0, sizeof(mxtype) * MAX_MATRIX_HEIGHT * w_b);

    for (i = 0; i < MAX_MATRIX_HEIGHT; i++) 
    {
        for (j = 0; j < w_b; j++) 
        {
            for (k = 0; k < MAX_MATRIX_HEIGHT; k++) 
            {
                b->mtrx[j][i] += tmp_a[i][k] * tmp_b[j][k];
            }
        }
    }
}
