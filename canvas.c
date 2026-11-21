#include "canvas.h"
#include "image.h"
#include <stdio.h>

/* RGB FORMAT: 0xAABBGGRR*/

Canvas* canvas_create(uint32_t width, uint32_t height, int stride) {
    Canvas *cv = (Canvas*)malloc(sizeof(Canvas));
    if (!cv) return NULL;

    cv->width = width;
    cv->height = height;

    cv->pixels = (uint32_t*)malloc(width * height * sizeof(uint32_t));
    if (!cv->pixels) {
        free(cv);
        return NULL;
    }

    return cv;
}

void canvas_fill_color(Canvas *cv, uint32_t color) {
    for (size_t y = 0; y < cv->height; y++) {
        for (size_t x = 0; x < cv->width; x++) {
            size_t offset = y * cv->width + x;
            cv->pixels[offset] = color;
        }
    }
}

int main() {

    uint32_t red = 0x000000FF;

    Canvas *cv = canvas_create(WIDTH, HEIGHT, 0);
    printf("CV_W: %u | CV_H: %u\n", cv->width, cv->width);
    canvas_fill_color(cv, red);

    ppm_save_pixels(cv->pixels, cv->width, cv->height, "sample.ppm");

    return EXIT_SUCCESS;
}