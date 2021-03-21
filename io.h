#ifndef IO_H_
#define IO_H_

#define R 0
#define G 1
#define B 2
#define RGB_NUM 3

int write_PPM (const int w, const int h, const unsigned char img[h][w][RGB_NUM], const char *out_file);
void display (const int w, const int h, const unsigned char img[h][w][RGB_NUM]);
void save_extension (const int w, const int h, const unsigned char img[h][w][RGB_NUM], const char *out_file);
#endif
