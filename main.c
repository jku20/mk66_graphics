#include "io.h"
#include "drawer.h"

#include <time.h>

#include <stdio.h>
#include <string.h>

#define W 800
#define H 800

#define TESTS 12

unsigned char img[H][W][RGB_NUM];

const int tests[TESTS][4] = {
    //octants 1 and 5
    {0, 0, W-1, H-1},
    {0, 0, W-1, H/2},
    {W-1, H-1, 0, H/2},
    //octants 8 and 4
    {0, H-1, W-1, 0},
    {0, H-1, W-1, H/2},
    {W-1, 0, 0, H/2},
    //octants 2 and 6
    {0, 0, W/2, H-1},
    {W-1, H-1, W/2, 0},
    //octants 7 and 3
    {0, H-1, W/2, 0},
    {W-1, 0, W/2, H-1},
    //horizontal and vertical
    {0, H/2, W-1, H/2},
    {W/2, 0, W/2, H-1}
};

void line_test() {
    memset(img, 0, sizeof(img));

    unsigned char c[RGB_NUM] = {0,255,0};
    int i;
    for(i = 0; i < TESTS; ++i) {
        //changing color
        switch(i) {
            case 3: 
                c[B] = 255;
                break;
            case 6:
                c[R] = 255;
                c[G] = 0;
                c[B] = 0;
                break;
            case 8:
                c[B] = 255;
                break;
            case 10:
                c[G] = 255;
                c[B] = 0;
                break;
            default:
                break;
        }

        const int *test = (int *) tests[i];
        printf("runing test %d\n", i);
        draw_line(test[0], test[1], test[2], test[3], W, (unsigned char *)img, c);
    }

    write_PPM((unsigned char *) img, W, H, "test.ppm");
}

void cool_image() {
    memset(img, 0, sizeof(img));
}

int main() {
    const clock_t start = clock();

    line_test();

    const clock_t end = clock();
    printf("time: %lfsec\n", (double)(end - start)/CLOCKS_PER_SEC);

    return 0;
}
