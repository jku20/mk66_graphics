#ifndef DRAWER_H_
#define DRAWER_H_

void plot(const int x, const int y, 
        const int w, const int h, unsigned char img[h][w][3], const char color[3]);
void draw_line(const int x1a, const int y1a, const int x2a, const int y2a,
        const int w, const int h, unsigned char img[h][w][3], const char color[3]);

#endif
