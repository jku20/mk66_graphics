#include "io.h"
#include "drawer.h"
#include "matrix.h"
#include "parser.h"

#include <time.h>
#include <stdio.h>
#include <string.h>

#define W 500
#define H 500


unsigned char img[H][W][RGB_NUM];

int main()
{
    const clock_t start = clock ();

    matrix *e_list = mk_matrix (0);
    matrix *t_rix = mk_matrix (4); //I am sorry, the pun is just too good

    enum token tok;
    while ((tok = get_token())) 
        e_list = parse_token ( tok, t_rix, e_list, W, H, img);

    free_matrix (t_rix); t_rix = NULL;
    free_matrix (e_list); e_list = NULL;

    const clock_t end = clock ();
    printf ("\ntime: %lfsec\n", (double)(end - start)/CLOCKS_PER_SEC);

    return 0;
}
