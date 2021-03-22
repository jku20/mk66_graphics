#include "io.h"

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

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
