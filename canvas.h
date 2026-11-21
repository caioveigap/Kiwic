#ifndef CANVAS_H
#define CANVAS_H

#include <stdlib.h>
#include <stdint.h>


/* CONSTANTS */
#define WIDTH 400
#define HEIGHT 400

typedef struct Color {
    uint8_t r, g, b, a;
} Color;

typedef struct Canvas {
    uint32_t width, height;
    uint32_t *pixels;
    int stride;
} Canvas;

#endif /* CANVAS_H */