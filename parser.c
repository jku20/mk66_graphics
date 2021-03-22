#include "parser.h"

#include "matrix.h"
#include "io.h"
#include "drawer.h"

#include <stdio.h>
#include <string.h>

#define M_PI 3.14159265358979323846264338327

/*
 * reads a token from stdin
 * returns: the token it read
*/

enum token get_token (void)
{
    int out;
    char buff[MAX_BUFFER_SIZE];

    if (fgets (buff, MAX_BUFFER_SIZE, stdin) != NULL)
    {
        sscanf (buff, "%s", buff);

        buff[strcspn (buff, "\n")] = '\0';

        if (strcmp (buff, "line") == 0)
        {
            return LINE; 
        }
        else if (strcmp (buff, "ident") == 0)
        {
            return IDENT;
        }
        else if (strcmp (buff, "scale") == 0) 
        {
            return SCALE;
        }
        else if (strcmp (buff, "move") == 0)
        {
            return MOVE;
        }
        else if (strcmp (buff, "rotate") == 0) 
        {
            return ROTATE;
        }
        else if (strcmp (buff, "apply") == 0) 
        {
            return APPLY;
        }
        else if (strcmp (buff, "display") == 0) 
        {
            return DISPLAY;
        }
        else if (strcmp (buff, "save") == 0) 
        {
            return SAVE;
        }
        else 
        {
            return INVALID;
        }
    } 
    else 
    {
        return END;
    }
}

/*
 * takes a token and processes it (possibly reading from stdin itself)
 * t_rix is the transformation matrix (I couldn't help myself)
 * e_rix is the edge matrix
 * returns: pointer to e_rix
*/

matrix *parse_token (const enum token tok, matrix *t_rix, matrix *e_rix, 
        const int w, const int h, unsigned char img[h][w][RGB_NUM]) 
{
    char buff[MAX_BUFFER_SIZE];
    matrix *tmp;

    switch (tok)
    {
        case END:
            return e_rix;
            break;
        case INVALID:
            return e_rix;
            break;
        case LINE:
            ;
            double x0, y0, z0, x1, y1, z1;

            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%lf %lf %lf %lf %lf %lf", &x0, &y0, &z0, &x1, &y1, &z1);

            e_rix = add_edge (e_rix, x0, y0, z0, x1, y1, z1);
            return e_rix;
            break;
        case IDENT:
            ident (t_rix);
            return e_rix;
            break;
        case SCALE:
            ;
            double sx, sy, sz;

            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%lf %lf %lf", &sx, &sy, &sz);

            //make functions which just morph an array in the future
            tmp = mk_scale (sx, sy, sz);
            matrix_mult (tmp, t_rix);
            free_matrix (tmp), tmp = NULL;

            return e_rix;
            break;
        case MOVE:
            ;
            double tx, ty, tz;

            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%lf %lf %lf", &tx, &ty, &tz);

            tmp = mk_translate (tx, ty, tz);
            matrix_mult (tmp, t_rix);

            free_matrix (tmp); tmp = NULL;

            return e_rix;
            break;
        case ROTATE:
            ;
            char axis;
            double theta;

            fgets (buff, MAX_BUFFER_SIZE, stdin);
            sscanf (buff, "%c %lf", &axis, &theta);

            theta *= M_PI/180.0;

            if (axis == 'x')
            {
                tmp = mk_rot_xaxis (theta);
                matrix_mult (tmp, t_rix);
                free_matrix (tmp);
            }
            else if (axis == 'y')
            {
                tmp = mk_rot_yaxis (theta);
                matrix_mult (tmp, t_rix);
                free_matrix (tmp);
            }
            else if (axis == 'z')
            {
                tmp = mk_rot_zaxis (theta);
                matrix_mult (tmp, t_rix);
                free_matrix (tmp);
            }
            else 
            {
                return e_rix;
            }
            return e_rix;
            break;
        case APPLY:
            matrix_mult (t_rix, e_rix);
            return e_rix;
            break;
        case DISPLAY:
            ;
            const char col[] = {255,255,255}; 

            clear_screen (w, h, img);
            draw_lines (e_rix, w, h, img, col);

            display (w, h, img);

            return e_rix;
            break;
        case SAVE:
            ;

            fgets (buff, MAX_BUFFER_SIZE, stdin);
            buff[strcspn (buff, "\n")] = '\0';

            save_extension (w, h, img, buff);
            return e_rix;
            break;
        default:
            return e_rix;
            break;
    }
}
