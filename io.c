#include "io.h"

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

/*
 * flips the y coordinates so it is upsidown 
 * here because I use different x y compared to DW and want to match his specs
 * this is basically just a hack
 * returns: void
*/

static void __h_swap (unsigned char *t1, unsigned char *t2) {unsigned char tmp = *t1; *t1 = *t2, *t2 = tmp;}

void flip_y (const int w, const int h, unsigned char img[h][w][RGB_NUM])
{
    int i,j,k;
    for (i = 0; i < (h+1)/2; i++)
        for (j = 0; j < w; j++)
            for (k = 0; k < RGB_NUM; k++)
                __h_swap (&img[i][j][k], &img[h-i-1][j][k]);
}

/*
 * writes to ppm file
 * the array should be img[height][width][RGB_NUM]
 * returns: 0 on success else -1
*/

int write_PPM (const int w, const int h, const unsigned char img[h][w][RGB_NUM], const char *out_file)
{
    int fd = open (out_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    dprintf (fd, "P6\n%d %d\n255\n",w,h);
    write (fd, img, sizeof(char) * w * h * 3);

    close (fd);

    return 0;
}

/*
 * uses imagemagick to display the image
 * returns: void
*/

void display (const int w, const int h, const unsigned char img[h][w][RGB_NUM])
{
    FILE *f;
    f = popen ("display", "w");

    fprintf (f, "P6\n%d %d\n255\n",w,h);
    fwrite (img, w * h * 3, sizeof (char), f);
    fprintf (f, "\n");

    pclose (f);
}

/*
 * uses imagemagick to save an image with a certain extension makesure out_file isn't too long
 * returns void
*/

void save_extension (const int w, const int h, const unsigned char img[h][w][RGB_NUM], const char *out_file) 
{
    FILE *f;
    char line[256];

    sprintf (line, "convert - %s", out_file);

    f = popen (line, "w");

    fprintf (f, "P6\n%d %d\n255\n",w,h);
    fwrite (img, w * h * 3, sizeof (char), f);
    fprintf (f, "\n");

    pclose (f);
}
