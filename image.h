#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Pixel {
    uint8_t r, g, b;
} Pixel;

typedef struct PPM_Image {
    uint32_t rows;
    uint32_t cols;
    uint32_t maxval;

    Pixel *pixels;
} PPM_Image;

PPM_Image* ppm_read_image(const char *filename);
bool ppm_write_image(const char *filename, const PPM_Image *image);
void ppm_destroy(PPM_Image *image);
bool ppm_save_pixels(uint32_t *pixels, size_t width, size_t height, const char *file_path);

#endif /* IMAGE_H */