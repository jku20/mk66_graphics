#include "drawer.h"

#include "matrix.h"

#include <string.h>

/*
 * adds a point to a given matrix
 * returns: pointer to the matrix
*/

matrix *add_point(matrix *points, const double x, const double y, const double z) {
    points = grow_matrix(points, 1);

    const int ind = points->w-1;

    points->mtrx[ind][0] = x;
    points->mtrx[ind][1] = y;
    points->mtrx[ind][2] = z;
    points->mtrx[ind][3] = 1.0;

    return points;
}

/*
 * adds edge to a given matrix
 * returns: pointer to the matrix
*/

matrix *add_edge(matrix *points,
        const double x0, const double y0, const double z0,
        const double x1, const double y1, const double z1
        ) {
    points = add_point(points, x0, y0, z0);
    points = add_point(points, x1, y1, z1);

    return points;
}

/*
 * draws lines to a given img array assumes matrix has even amount of 
 * returns: void
*/

void draw_lines(matrix *points, const int w, const int h, unsigned char img[h][w][3], const char color[3]) {
    const int size = points->w;
    int i,j;
    for(i = 1; i < size; i+=2) {
        const int x0 = (int) points->mtrx[i][0], x1 = (int) points->mtrx[i-1][0];
        const int y0 = (int) points->mtrx[i][1], y1 = (int) points->mtrx[i-1][1];
        draw_line(x0, y0, x1, y1, w, h, img, color);
    }
}


/*
 * plots a color, make sure to check the number is in bounds
 * the color array should be {R,G,B}
 * Returns: void
*/

void plot(const int x, const int y, 
        const int w, const int h, unsigned char img[h][w][3], const char color[3]) {
    memcpy(img[y][x], color, sizeof(char) * 3);
}

/*
 * bresenham's line algorithm, draws a line given two coordinates currently the dumb casework version
 * Returns: void
*/

void draw_line(const int x1a, const int y1a, const int x2a, const int y2a,
        const int w, const int h, unsigned char img[h][w][3], const char color[3]) {
    int x1,x2,y1,y2;
    //preprocess starting coords so larger x is always x2
    if(x1a > x2a) {
        x1 = x2a, y1 = y2a;
        x2 = x1a, y2 = y1a;
    } else {
        x1 = x1a, y1 = y1a;
        x2 = x2a, y2 = y2a;
    }

    //some useful constants
    const int dx = x2-x1, dy = y2-y1;
    const int slope_pos = (dy > 0) ? 1 : -1;
    const int a = 2*dy, b = 2*dx*slope_pos*-1;

    //doing that drawing thing
    if(dx > dy*slope_pos) {
        //slope < 1
        int x = x1, y = y1;
        int d = 2*dy*slope_pos-dx*slope_pos*-1;

        while(x <= x2) {
            plot(x,y,w,h,img,color);

            if(d*slope_pos > 0) {
                y+=slope_pos;
                y = (y*slope_pos > y2*slope_pos) ? y2 : y;
                d+=b;
            }
            x+=1;
            d+=a;
        }
    } else {
        //slope >= 1
        int x = x1, y = y1;
        int d = 2*dx*slope_pos*-1-dy*slope_pos;

        while(y*slope_pos <= y2*slope_pos) {
            plot(x,y,w,h,img,color);

            if(d*slope_pos < 0) {
                x+=1;
                x = (x > x2) ? x2 : x;
                d+=a;
            }
            y+=slope_pos;
            d+=b;
        }
    }
}
