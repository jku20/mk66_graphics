#include "io.h"
#include "drawer.h"
#include "matrix.h"

#include <time.h>
#include <stdio.h>

#define NH 4
#define NW 2
#define MH 4
#define MW 4

int main() {
    const clock_t start = clock();

    printf("here\n");
    int cnt = 1;
    int i,j;
    mxtype n[NH][NW];
    for(i = 0; i < NW; i++) 
        for(j = 0; j < NH-1; j++)
            n[j][i] = cnt++;
    for(i = 0; i < NW; i++) 
        n[NH-1][i] = 1;

    cnt = 1;
    mxtype m[MH][MW];
    ident(MH, m);

    for(i = 0; i < 500; i++) 
        for(j = 0; j < 500; j++) 
            m[j][i] = cnt++;
    for(i = 0; i < 500; i++) 
        m[499][i] = 1;


    //print_matrix(4, 4, m);
    //print_matrix(2, 4, n);

    matrix_mult(500,500,m,250,500,n);

    //print_matrix(2, 4, n);

    ident(4, m);

    const clock_t end = clock();
    printf("time: %lfsec\n", (double)(end - start)/CLOCKS_PER_SEC);

    return 0;
}
