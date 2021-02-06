#ifndef IO_H_
#define IO_H_

#define R 0
#define G 1
#define B 2
#define RGB_NUM 3

//returns 0 on success else -1, takes dimensions and array and writes ppm
//the array should be img[height][width][RGB_NUM]
int write_PPM(const unsigned char *img, const int h, const int w, const char *out_file);

#endif
