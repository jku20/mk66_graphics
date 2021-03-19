#include "io.h"

#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

/*
 * writes to ppm file
 * the array should be img[height][width][RGB_NUM]
 * Returns: 0 on success else -1
*/

int write_PPM (const int w, const int h, const unsigned char img[h][w][RGB_NUM], const char *out_file)
{
    int fd = open (out_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    dprintf (fd, "P6\n%d %d\n255\n",w,h);
    write (fd, img, sizeof(char) * w * h * 3);

    close (fd);

    return 0;
}
