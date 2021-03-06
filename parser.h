#ifndef PARSER_H_
#define PARSER_H_

#include "matrix.h"
#include "io.h"

//I am going to use bad scanf for now because there is not need to this to be great, it will probably be replaced later

#define MAX_BUFFER_SIZE 128
#define STEP_SIZE 20

//tokens 
enum token
{
    END,

    LINE,
    IDENT,

    SCALE,
    MOVE,
    ROTATE,

    CIRCLE,
    HERMITE,
    BEZIER,

    BOX,
    SPHERE,
    TORUS,

    CLEAR,
    APPLY,
    DISPLAY,
    SAVE,

    INVALID
};

enum token get_token (void);
matrix *parse_token (const enum token tok, matrix *t_rix, matrix *e_rix, 
        const int w, const int h, unsigned char img[h][w][RGB_NUM]);

#endif
