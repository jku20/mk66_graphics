#ifndef DRAWER_H_
#define DRAWER_H_

#include "matrix.h"

matrix *add_point(matrix *points, const double x, const double y, const double z);
matrix *add_edge(matrix *points,
        const double x0, const double y0, const double z0,
        const double x1, const double y1, const double z1
        );
void draw_lines(matrix *points, const int w, const int h, unsigned char img[h][w][3], const char color[3]);

void plot(const int x, const int y, 
        const int w, const int h, unsigned char img[h][w][3], const char color[3]);
void draw_line(const int x1a, const int y1a, const int x2a, const int y2a,
        const int w, const int h, unsigned char img[h][w][3], const char color[3]);

#endif
