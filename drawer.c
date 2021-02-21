#include "drawer.h"

#include <string.h>

/*
 * plots a color, make sure to check the number is in bounds
 * the color array should be {R,G,B}
 * Returns: void
*/

void plot(const int x, const int y, 
        const int w, unsigned char *img, const char color[3]) {
    memcpy(img+(y*w*3)+x*3, color, sizeof(char) * 3);
}

/*
 * bresenham's line algorithm, draws a line given two coordinates currently the dumb casework version
 * Returns: void
*/

void draw_line(const int x1a, const int y1a, const int x2a, const int y2a,
        const int w, unsigned char *img, const char color[3]) {
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
            plot(x,y,w,img,color);

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
            plot(x,y,w,img,color);

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
