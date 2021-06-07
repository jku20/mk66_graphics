#include "parser.h"

#include "matrix.h"
#include "io.h"
#include "drawer.h"
#include "stack.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

/*
 * reads a token from stdin
 * returns: the token it read
*/

enum token get_token (void)
{
    //psudo map, essentually hashing the tokens by a predefined index
    static const char *tokens[] = {"line", "scale", "move", "rotate", "display", "save", "circle", "hermite", "bezier", "box", "sphere", "torus", "push", "pop", NULL};
    static const enum token semantics[] = {LINE, SCALE, MOVE, ROTATE, DISPLAY, SAVE, CIRCLE, HERMITE, BEZIER, BOX, SPHERE, TORUS, PUSH, POP};

    char buff[MAX_BUFFER_SIZE];

    if (fgets (buff, MAX_BUFFER_SIZE, stdin) != NULL)
    {
        sscanf (buff, "%s", buff);

        buff[strcspn (buff, "\n")] = '\0';

        int i;
        for (i = 0; tokens[i] != NULL; i++)
            if (strcmp (buff, tokens[i]) == 0)
                return semantics[i];

        return INVALID;
    } 
    else 
    {
        return END;
    }
}

/*
 * takes a token and processes it (possibly reading from stdin itself)
 * 3 different versions each with their own slightly different functions
 * returns: void
*/

void parse_token_2d (const enum token tok, 
        stack *transform_stack, const unsigned char col[3],
        const int w, const int h, unsigned char img[h][w][RGB_NUM])
{
    char buff[MAX_BUFFER_SIZE];

    double x0,x1,x2,x3,y0,y1,y2,y3,z0,z1;
    double cx, cy, cz, r;

    matrix *edges = mk_matrix (0);
    switch (tok)
    {
        case END:
            break;
        case INVALID:
            break;
        case LINE:
            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%lf %lf %lf %lf %lf %lf", &x0, &y0, &z0, &x1, &y1, &z1);

            edges = add_edge (edges, x0, y0, z0, x1, y1, z1);
            matrix_mult (peek (transform_stack), edges);

            draw_lines (edges, w, h, img, col);
            break;
        case CIRCLE:
            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%lf %lf %lf %lf", &cx, &cy, &cz, &r);

            edges = add_circle (edges, cx, cy, cz, r, STEP_SIZE);
            matrix_mult (peek (transform_stack), edges);

            draw_lines (edges, w, h, img, col);
            break;
        case HERMITE:
            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%lf %lf %lf %lf %lf %lf %lf %lf", &x0, &y0, &x1, &y1, &x2, &y2, &x3, &y3);
            
            edges = add_curve (edges,x0,y0,x1,y1,x2,y2,x3,y3,STEP_SIZE,HERMITE_T);
            matrix_mult (peek (transform_stack), edges);

            draw_lines (edges, w, h, img, col);
            break;
        case BEZIER:
            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%lf %lf %lf %lf %lf %lf %lf %lf", &x0, &y0, &x1, &y1, &x2, &y2, &x3, &y3);
            
            edges = add_curve (edges,x0,y0,x1,y1,x2,y2,x3,y3,STEP_SIZE,BEZIER_T);
            matrix_mult (peek (transform_stack), edges);

            draw_lines (edges, w, h, img, col);
            break;

        default:
            break;
    }

    free_matrix (edges), edges = NULL;
    return;
}

void parse_token_3d (const enum token tok, 
        stack *transform_stack, const unsigned char col[3],
        const int w, const int h, unsigned char img[h][w][RGB_NUM])
{
    char buff[MAX_BUFFER_SIZE];

    double x0,y0,z0;
    double r, r1, r2;
    double width, height, depth;

    matrix *polygons = mk_matrix (0);
    switch (tok)
    {
        case END:
            break;
        case INVALID:
            break;
        case BOX:
            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%lf %lf %lf %lf %lf %lf", &x0, &y0, &z0, &width, &height, &depth);

            polygons = add_box (polygons, x0, y0, z0, width, height, depth);
            matrix_mult (peek (transform_stack), polygons);

            draw_polygons (polygons, w, h, img, col);
            break;
        case SPHERE:
            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%lf %lf %lf %lf", &x0, &y0, &z0, &r);

            polygons = add_sphere (polygons, x0, y0, z0, r, STEP_SIZE);
            matrix_mult (peek (transform_stack), polygons);

            draw_polygons (polygons, w, h, img, col);
            break;
        case TORUS:
            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%lf %lf %lf %lf %lf", &x0, &y0, &z0, &r1, &r2);

            polygons = add_torus (polygons, x0, y0, z0, r1, r2, STEP_SIZE);
            matrix_mult (peek (transform_stack), polygons);

            draw_polygons (polygons, w, h, img, col);
            break;
        default:
            break;
    }

    free_matrix (polygons);
    return;
}

void parse_token_meta (const enum token tok, 
        stack *transform_stack, const unsigned char col[3],
        const int w, const int h, unsigned char img[h][w][RGB_NUM]) 
{
    char buff[MAX_BUFFER_SIZE];
    matrix *tmp;

    double sx, sy, sz;
    double tx, ty, tz;
    char axis;
    double theta;
    switch (tok)
    {
        case END:
            break;
        case INVALID:
            break;
        case PUSH:
            push (transform_stack);
            break;
        case POP:
            pop (transform_stack);
            break;
        case SCALE:
            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%lf %lf %lf", &sx, &sy, &sz);

            //make functions which just morph an array in the future
            tmp = mk_scale (sx, sy, sz);
            matrix_mult (peek (transform_stack), tmp);
            cpy_matrix (tmp, peek (transform_stack));

            free_matrix (tmp), tmp = NULL;
            break;
        case MOVE:
            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%lf %lf %lf", &tx, &ty, &tz);

            tmp = mk_translate (tx, ty, tz);
            matrix_mult (peek (transform_stack), tmp);
            cpy_matrix (tmp, peek (transform_stack));

            free_matrix (tmp), tmp = NULL;
            break;
        case ROTATE:
            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%c %lf", &axis, &theta);

            theta *= M_PI/180.0;

            if (axis == 'x')
            {
                tmp = mk_rot_xaxis (theta);
            }
            else if (axis == 'y')
            {
                tmp = mk_rot_yaxis (theta);
            }
            else if (axis == 'z')
            {
                tmp = mk_rot_zaxis (theta);
            }
            else 
            {
                tmp = mk_matrix (4);
                ident (tmp);
            }
            matrix_mult (peek (transform_stack), tmp);
            cpy_matrix (tmp, peek (transform_stack));

            free_matrix (tmp), tmp = NULL;
            break;
        case DISPLAY:
            flip_y (w, h, img);
            display (w, h, img);
            flip_y (w, h, img);

            break;
        case SAVE:
            fgets (buff, MAX_BUFFER_SIZE, stdin);
            buff[strcspn (buff, "\n")] = '\0';

            flip_y (w, h, img);
            save_extension (w, h, img, buff);
            flip_y (w, h, img);

            break;
        default:
            break;
    }
}
