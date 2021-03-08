#include "io.h"
#include "drawer.h"
#include "matrix.h"

#include <time.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_MX_SIZE 10


void test_matricies();
void cool_test_image();

int main() {
    const clock_t start = clock();

    test_matricies();
    cool_test_image();
    
    const clock_t end = clock();
    printf("\ntime: %lfsec\n", (double)(end - start)/CLOCKS_PER_SEC);

    return 0;
}

#define W 800
#define H 800

void cool_test_image() {
    unsigned char img[H][W][RGB_NUM];
    memset(img, 0, sizeof(img));

    //const int root = W/2;

    int i;
    for(i = 0; i < 200; i++) {
        const char color[] = {255-201+i, 20, 20};
        matrix *src = mk_matrix(0);
        src = mx_reserve(src, 2);

        add_edge(src, W-1, H/2 + i, 0, 0, H/2 - i, 0);
        add_edge(src, W/2 + i, H-1, 0, W/2 - i, 0, 0);
        add_edge(src, 0, H/2 + i, 0, W-1, H/2 - i, 0);
        add_edge(src, W/2 + i, 0, 0, W/2 - i, H-1, 0);

        draw_lines(src, W, H, img, color);

        src = free_matrix(src);
    }

    write_PPM(W, H, img, "test.ppm");
}

void test_matricies() {
    matrix *m2 = mk_matrix(0);
    m2 = mx_reserve(m2,4);

    printf("adding (1,2,3) and (4,5,6)\n");

    add_edge(m2, 1, 2, 3, 4, 5, 6);
    print_matrix(m2);

    matrix *m1 = mk_matrix(4);

    printf("creating an identity matrix\n");

    ident(m1);
    print_matrix(m1);

    printf("multiplying by an identity matrix\n");

    matrix_mult(m1, m2);
    print_matrix(m2);

    printf("setting m1 to test matrix multiplication\n");

    mx_resize(m1, 0);
    int i;
    for(i = 1; i <= 12-5; i+=6) {
        add_edge(m1, i, i + 1, i + 2, i + 3, i + 4, i + 5);
    }
    print_matrix(m1);

    printf("multiplying the matricies\n");

    matrix_mult(m1, m2);
    print_matrix(m2);

    m1 = free_matrix(m1), m2 = free_matrix(m2);
}
