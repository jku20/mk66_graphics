#include "io.h"

#include <time.h>
#include <stdlib.h>

#include <stdio.h>

#define W 1000
#define H 1000

#define min(x,y) (x) > (y) ? (y) : (x)
#define max(x,y) (x) < (y) ? (y) : (x)

void chaos(int *pair, unsigned char image[H][W][RGB_NUM], const int t);

unsigned char img[H][W][RGB_NUM];

const int t = W;

int main() {

    srand(time(NULL));

    int pair[] = {t,t};

    const int eopch = 50000000;

    int i;
    for(i = 0; i < eopch; i++) {
        if(!(i % (eopch/10))) {
            printf("p[0]: %d\tp[1]: %d\n", pair[0], pair[1]);
        }
        chaos(pair, img, t);
    }

    write_PPM((unsigned char*) img, H, W, "test.ppm");

    return 0;
}

//i don't even know
const int d[] = {-1,1,0,0};
const int zero_or_one[] = {0,1,1};
const int prob = 3;
void chaos(int *p, unsigned char image[H][W][RGB_NUM], const int t) {
    const int thing = 800;

    int x = p[0], y = p[1];
    x = min(W-2, max(x,0));
    y = min(H-2, max(y,0));
    /*
    printf("p[0]: %d\tp[1]: %d\n", p[0], p[1]);
    printf("x: %d\ty: %d\n", x, y);
    */
    image[y][x][R] = (image[y+1][x+1][R] + 5*zero_or_one[rand()%prob]) % 256;
    image[y][x][G] = (image[y+1][x+1][G] + 5*zero_or_one[rand()%prob]) % 256;
    image[y][x][B] = (image[y+1][x+1][B] + 5*zero_or_one[rand()%prob]) % 256;

    p[1] = p[1] + d[rand() % 4];
    p[0] = p[0] + d[rand() % 4];

    p[0] %= W;
    p[1] %= H;

}
