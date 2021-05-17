#include "drawer.h"

#include "matrix.h"
#include "gmath.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

//this is about zero and used in add_air_quotes_point
const static double epsilon = 1.0;

/*
 * adds a point to a given matrix
 * returns: pointer to the matrix
*/

matrix *add_point(matrix *points, const double x, const double y, const double z) 
{
    points = grow_matrix (points, 1);

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

matrix *add_edge (matrix *points,
        const double x0, const double y0, const double z0,
        const double x1, const double y1, const double z1
        ) 
{
    points = add_point (points, x0, y0, z0);
    points = add_point (points, x1, y1, z1);

    return points;
}

/*
 * adds polygon (triangle) to a given matrix
 * returns: pointer to the matrix
*/

matrix *add_polygon (matrix *points,
        const double x0, const double y0, const double z0,
        const double x1, const double y1, const double z1,
        const double x2, const double y2, const double z2)
{
    points = add_point (points, x0, y0, z0);
    points = add_point (points, x1, y1, z1);
    points = add_point (points, x2, y2, z2);

    return points;
}

/*
 * draws lines to a given img array assumes matrix has even amount of 
 * returns: void
*/

void draw_lines (matrix *points, const int w, const int h, unsigned char img[h][w][3], const unsigned char color[3]) 
{
    const int size = points->w;
    int i;
    for (i = 1; i < size; i+=2) 
    {
        const int x0 = (int) points->mtrx[i][0], x1 = (int) points->mtrx[i-1][0];
        const int y0 = (int) points->mtrx[i][1], y1 = (int) points->mtrx[i-1][1];
        draw_line (x0, y0, x1, y1, w, h, img, color);
    }
}

/*
 * draws lines to a given img array assumes matrix has even amount of 
 * returns: void
*/

void draw_polygons (matrix *points, const int w, const int h, unsigned char img[h][w][3], const unsigned char color[3])
{
    const int size = points->w;
    int i;
    for (i = 2; i < size; i+=3) 
    {
        const int x0 = (int) points->mtrx[i][0], x1 = (int) points->mtrx[i-1][0], x2 = (int) points->mtrx[i-2][0];
        const int y0 = (int) points->mtrx[i][1], y1 = (int) points->mtrx[i-1][1], y2 = (int) points->mtrx[i-2][1];

        //do I draw the thing?
        double norm[] = {0.0,0.0,0.0};
        calculate_normal (points, i-2, norm);
        const double v[] = {0.0,0.0,1.0};

        if (0.0 < dot_product (norm, v)) 
        {
            draw_line (x0, y0, x1, y1, w, h, img, color);
            draw_line (x0, y0, x2, y2, w, h, img, color);
            draw_line (x1, y1, x2, y2, w, h, img, color);
        }
    }
}

/*
 * plots a color, make sure to check the number is in bounds
 * the color array should be {R,G,B}
 * Returns: void
*/

void plot (const int x, const int y, 
        const int w, const int h, unsigned char img[h][w][3], const unsigned char color[3]) 
{
    if (x < w && y < h && x >=0 && y >= 0) 
        memcpy (img[y][x], color, sizeof(unsigned char) * 3);
}

/*
 * sets everything in img to 0
 * returns: void
*/

void clear_screen (const int w, const int h, unsigned char img[h][w][3]) 
{
    memset (img, 0, sizeof (char) * 3 * w * h);
}

/*
 * bresenham's line algorithm, draws a line given two coordinates currently the dumb casework version
 * Returns: void
*/


void draw_line (const int x1a, const int y1a, const int x2a, const int y2a,
        const int w, const int h, unsigned char img[h][w][3], const unsigned char color[3]) 
{
    int x1,x2,y1,y2;
    //preprocess starting coords so larger x is always x2
    if (x1a > x2a) 
    {
        x1 = x2a, y1 = y2a;
        x2 = x1a, y2 = y1a;
    } else 
    {
        x1 = x1a, y1 = y1a;
        x2 = x2a, y2 = y2a;
    }

    //some useful constants
    const int dx = x2-x1, dy = y2-y1;
    const int slope_pos = (dy > 0) ? 1 : -1;
    const int a = 2*dy, b = 2*dx*slope_pos*-1;

    //doing that drawing thing
    if (dx > dy*slope_pos) 
    {
        //slope < 1
        int x = x1, y = y1;
        int d = 2*dy*slope_pos-dx*slope_pos*-1;

        while (x <= x2) 
        {
            plot (x,y,w,h,img,color);

            if (d*slope_pos > 0) 
            {
                y+=slope_pos;
                y = (y*slope_pos > y2*slope_pos) ? y2 : y;
                d+=b;
            }
            x+=1;
            d+=a;
        }
    } else
    {
        //slope >= 1
        int x = x1, y = y1;
        int d = 2*dx*slope_pos*-1-dy*slope_pos;

        while (y*slope_pos <= y2*slope_pos) 
        {
            plot (x,y,w,h,img,color);

            if (d*slope_pos < 0) 
            {
                x+=1;
                x = (x > x2) ? x2 : x;
                d+=a;
            }
            y+=slope_pos;
            d+=b;
        }
    }
}

/*
 * adds a "circle" to the edge list based on a center x,y,z, radius and step
 * note the circle is in the xy plane, not the z plane
 * I think if step is too too big stuff might die but there is no reason for step to actually be that big
 * returns: edges matrix
*/

matrix *add_circle (matrix *edges, 
        const double cx, const double cy, const double cz,
        const double r, const int step
        )
{
    const double chng = 1.0 / step;
    double t,i;
    for (t = chng, i = 0; i < step; i++, t += chng)
    {
        double p_t = t-chng;
        edges = add_edge (edges,
                cx + r * cos (2 * M_PI * p_t), cy + r * sin (2 * M_PI * p_t), cz,
                cx + r * cos (2 * M_PI * t), cy + r * sin (2 * M_PI * t), cz
                );
    }

    return edges;
}

/*
 * adds a curve based on the specified type, HERMITE or BEZIER
 * returns: edges matrix
*/

matrix *add_curve (matrix *edges,
        const double x0, const double y0,
        const double x1, const double y1,
        const double x2, const double y2,
        const double x3, const double y3,
        const double step, enum curve_type type)
{
    //setting a,b,c,d
    double ax,bx,cx,dx,ay,by,cy,dy;
    if (type == HERMITE_T)
    {
        ax = 2*x0-2*x1+x2+x3; ay = 2*y0-2*y1+y2+y3;
        bx = -3*x0+3*x1-2*x2-x3; by = -3*y0+3*y1-2*y2-y3;
        cx = x2; cy = y2;
        dx = x0; dy = y0;
    }
    else if (type == BEZIER_T)
    {
        ax = -x0+3*x1-3*x2+x3; ay = -y0+3*y1-3*y2+y3;
        bx = 3*x0-6*x1+3*x2; by = 3*y0-6*y1+3*y2;
        cx = -3*x0+3*x1; cy = -3*y0+3*y1;
        dx = x0; dy = y0;
    }
    else
    {
        printf ("something has gone horribly wrong from add_curve\n");
        return edges;
    }

    //writing all the edges
    const double chng = 1.0 / step;
    double t,i;
    for (t = chng, i = 0; i < step; i++, t += chng)
    {
        double p_t = t-chng;
        edges = add_edge (edges,
                ax*p_t*p_t*p_t + bx*p_t*p_t + cx*p_t + dx, 
                ay*p_t*p_t*p_t + by*p_t*p_t + cy*p_t + dy, 
                0.0,
                ax*t*t*t + bx*t*t + cx*t + dx, 
                ay*t*t*t + by*t*t + cy*t + dy, 
                0.0
                );
    }

    return edges;
}

/*adds a very small line which apears like a point
 * returns: edge matrix
*/

matrix *add_air_quotes_point (matrix *points, const double x, const double y, const double z)
{
    return add_edge (points, x, y, z, x+epsilon, y, z);
}

/*
 * adds a box to the poly matrix
 * returns: poly matrix
*/

matrix *add_box (matrix *polys,
        const double xa, const double ya, const double za,
        const double width, const double height, const double depth
        )
{
    double xb,xc,xd,xe,xf,xg,xh;
    xb = xf = xe = xa;
    xd = xc = xg = xh = xa + width;
    double yb,yc,yd,ye,yf,yg,yh;
    yd = yh = ye = ya;
    yb = yc = yg = yf = ya - height;
    double zb,zc,zd,ze,zf,zg,zh;
    zb = zc = zd = za;
    ze = zh = zg = zf = za - depth;

    polys = add_polygon (polys,
        xe,ye,ze,
        xd,yd,zd,
        xh,yh,zh
    );
    polys = add_polygon (polys,
        xa,ya,za,
        xd,yd,zd,
        xe,ye,ze
    );
    polys = add_polygon (polys,
        xa,ya,za,
        xb,yb,zb,
        xd,yd,zd
    );
    polys = add_polygon (polys,
        xb,yb,zb,
        xc,yc,zc,
        xd,yd,zd
    );
    polys = add_polygon (polys,
        xb,yb,zb,
        xf,yf,zf,
        xc,yc,zc
    );
    polys = add_polygon (polys,
        xf,yf,zf,
        xg,yg,zg,
        xc,yc,zc
    );
    polys = add_polygon (polys,
        xf,yf,zf,
        xe,ye,ze,
        xg,yg,zg
    );
    polys = add_polygon (polys,
        xe,ye,ze,
        xh,yh,zh,
        xg,yg,zg
    );
    polys = add_polygon (polys,
        xd,yd,zd,
        xc,yc,zc,
        xh,yh,zh
    );
    polys = add_polygon (polys,
        xc,yc,zc,
        xg,yg,zg,
        xh,yh,zh
    );
    polys = add_polygon (polys,
        xe,ye,ze,
        xf,yf,zf,
        xa,ya,za
    );
    polys = add_polygon (polys,
        xf,yf,zf,
        xb,yb,zb,
        xa,ya,za
    );

    return polys;
}

/*
 * helper function, returns the wanted points on a sphere
 * returns: matrix of very short lines representing points
*/

matrix *generate_sphere (
        const double cx, const double cy, const double cz,
        const double r, const int step
        )
{
    matrix *out = mk_matrix (0);

    const double tchng = 1.0 / (step-1);
    const double uchng = 1.0 / (step);

    long double t,u; int i,j;
    for (u = 0, j = 0; j < step; j++)//, u += uchng) 
        for (t = 0, i = 0; i < step; i++)// t += tchng)
        {
            t = M_PI * (double) i / (step-1);
            u = 2.0 * M_PI * (double) j / step;
            out = add_point (out, 
                    r * cos (t) + cx,
                    r * sin (t) * cos (u) + cy,
                    r * sin (t) * sin (u) + cz
                    );
        }

    return out;
}

/*
 * adds a sphere to the edge matrix
 * returns: edge matrix
*/

matrix *add_sphere (matrix *edges,
        const double cx, const double cy, const double cz,
        const double r, const int step
        )
{
    matrix *helper = generate_sphere (cx, cy, cz, r, step);
    int i;
    for (i = 0; i < helper->w; i++)
    {
        //if (i % step == step-1) continue;

        const int p0 = (i) % (helper->w);
        const int p1 = (i+1) % (helper->w);
        const int p2 = (i+1+step) % (helper->w);
        const int p3 = (i+step) % (helper->w);

        if (i % step != step-1)
        {
            edges = add_polygon (edges,
                    helper->mtrx[p0][0], helper->mtrx[p0][1], helper->mtrx[p0][2],
                    helper->mtrx[p1][0], helper->mtrx[p1][1], helper->mtrx[p1][2],
                    helper->mtrx[p2][0], helper->mtrx[p2][1], helper->mtrx[p2][2]
                    );
        }
        if (i % step != 0)
        {
            edges = add_polygon (edges,
                    helper->mtrx[p0][0], helper->mtrx[p0][1], helper->mtrx[p0][2],
                    helper->mtrx[p2][0], helper->mtrx[p2][1], helper->mtrx[p2][2],
                    helper->mtrx[p3][0], helper->mtrx[p3][1], helper->mtrx[p3][2]
                    );

        }
    }

    free_matrix (helper); helper = NULL;

    return edges;
}

/*
 * helper function, returns the wanted points on a torus
 * returns: matrix of very short lines representing points
*/


matrix *generate_torus (
        const double cx, const double cy, const double cz,
        const double r1, const double r2, const int step
        )
{
    matrix *out = mk_matrix (0);

    const double chng = 2 * M_PI / step;
    int i,j;
    double t,u;
    for (u = 0, j = 0; j < step; j++, u += chng) 
        for (t = 0, i = 0; i < step; i++, t += chng)
            out = add_point (out, 
                    cos (u) * (r1 * cos (t) + r2) + cx,
                    r1 * sin (t) + cy,
                    -sin (u) * (r1 * cos (t) + r2) + cz
                    );
    return out;
}

/*
 * adds a torus to the edge matrix
 * returns: edge matrix
*/

matrix *add_torus (matrix *edges,
        const double cx, const double cy, const double cz,
        const double r1, const double r2, const int step
        )
{
    matrix *helper = generate_torus (cx, cy, cz, r1, r2, step);

    int i;
    for (i = 0; i < helper->w; i++)
    {
        int p0,p1,p2,p3;
        if (i % step == step-1) 
        {
            p0 = (i) % (helper->w);
            p1 = (i-step+1) % (helper->w);
            p2 = (i+1) % (helper->w);
            p3 = (i+step) % (helper->w);
        }
        else 
        {
            p0 = (i) % (helper->w);
            p1 = (i+1) % (helper->w);
            p2 = (i+1+step) % (helper->w);
            p3 = (i+step) % (helper->w);

        }
        edges = add_polygon (edges,
            helper->mtrx[p0][0], helper->mtrx[p0][1], helper->mtrx[p0][2],
            helper->mtrx[p1][0], helper->mtrx[p1][1], helper->mtrx[p1][2],
            helper->mtrx[p2][0], helper->mtrx[p2][1], helper->mtrx[p2][2]
        );
        edges = add_polygon (edges,
            helper->mtrx[p0][0], helper->mtrx[p0][1], helper->mtrx[p0][2],
            helper->mtrx[p2][0], helper->mtrx[p2][1], helper->mtrx[p2][2],
            helper->mtrx[p3][0], helper->mtrx[p3][1], helper->mtrx[p3][2]
        );
    }

    free_matrix (helper); helper = NULL;

    return edges;
}

