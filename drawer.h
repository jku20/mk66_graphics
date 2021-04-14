#ifndef DRAWER_H_
#define DRAWER_H_

#include "matrix.h"
#include "io.h"

//curve types for hermite and bezier curves
enum curve_type {HERMITE_T, BEZIER_T};

matrix *mk_matrix (const int w);
matrix *add_point (matrix *points, const double x, const double y, const double z);
matrix *add_edge (matrix *points,
        const double x0, const double y0, const double z0,
        const double x1, const double y1, const double z1);
void draw_lines (matrix *points, const int w, const int h, unsigned char img[h][w][3], const unsigned char color[3]);

void plot (const int x, const int y, 
        const int w, const int h, unsigned char img[h][w][3], const unsigned char color[3]);
void draw_line (const int x1a, const int y1a, const int x2a, const int y2a,
        const int w, const int h, unsigned char img[h][w][3], const unsigned char color[3]);
void clear_screen (const int w, const int h, unsigned char img[h][w][RGB_NUM]);

matrix *add_circle (matrix *edges, 
        const double cx, const double cy, const double cz,
        const double r, const double step
        );
matrix *add_curve (matrix *edges,
        const double x0, const double y0,
        const double x1, const double y1,
        const double x2, const double y2,
        const double x3, const double y3,
        const double step, enum curve_type type);

#endif
