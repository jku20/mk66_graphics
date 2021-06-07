#ifndef PARSER_H_
#define PARSER_H_

#include "matrix.h"
#include "io.h"
#include "stack.h"

//I am going to use bad scanf for now because there is not need to this to be great, it will probably be replaced later

#define MAX_BUFFER_SIZE 128
#define STEP_SIZE 15

//tokens 
enum token
{
    END,

    PUSH,
    POP,

    SCALE,
    MOVE,
    ROTATE,

    LINE,
    CIRCLE,
    HERMITE,
    BEZIER,

    BOX,
    SPHERE,
    TORUS,

    DISPLAY,
    SAVE,

    INVALID
};

enum token get_token (void);
void parse_token_2d (const enum token tok, 
        stack *transform_stack, const unsigned char col[3],
        const int w, const int h, unsigned char img[h][w][RGB_NUM]);
void parse_token_3d (const enum token tok, 
        stack *transform_stack, const unsigned char col[3],
        const int w, const int h, unsigned char img[h][w][RGB_NUM]);
void parse_token_meta (const enum token tok, 
        stack *transform_stack, const unsigned char col[3],
        const int w, const int h, unsigned char img[h][w][RGB_NUM]);
#endif
