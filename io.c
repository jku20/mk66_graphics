#include "io.h"

#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>


int write_PPM(const unsigned char *img, const int h, const int w, const char *out_file) {
    int fd = open(out_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    dprintf(fd, "P6\n%d %d\n255\n",w,h);
    write(fd, img, sizeof(char) * w * h * 3);

    close(fd);

    return 0;
}
