#include "io.h"
#include "drawer.h"
#include "matrix.h"
#include "parser.h"
#include "stack.h"

#include <time.h>
#include <stdio.h>
#include <string.h>

#define W 500
#define H 500


unsigned char img[H][W][RGB_NUM];

int main()
{
    const clock_t start = clock ();

    const unsigned char col[] = {255,255,255};

    stack *transform_stack = new_stack();

    enum token tok;
    while ((tok = get_token()))
    {
        parse_token_2d (tok, transform_stack, col, W, H, img);
        parse_token_3d (tok, transform_stack, col, W, H, img);
        parse_token_meta (tok, transform_stack, col, W, H, img);
    }

    free_stack (transform_stack);

    const clock_t end = clock ();
    printf ("\ntime: %lfsec\n", (double)(end - start)/CLOCKS_PER_SEC);

    return 0;
}
