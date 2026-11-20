#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdbool.h>

// uint16_t (antes uint8_t): uma amostra PPM pode ir até maxval = 65535 (16 bits).
// Com uint8_t, imagens de 16 bits seriam truncadas. Custo: o Pixel passa de 3 para 6 bytes.
typedef struct Pixel {
    uint16_t r, g, b;
} Pixel;

typedef struct PPM_Image {
    uint32_t rows;
    uint32_t cols;
    uint32_t maxval;

    Pixel **pixels;
} PPM_Image;

PPM_Image* ppm_read_image(const char *filename);
bool ppm_write_image(const char *filename, PPM_Image *image);
void ppm_destroy(PPM_Image *image);

#endif /* IMAGE_H */