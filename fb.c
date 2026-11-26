#include "fb.h"
#include <stdlib.h>
// TODO: definir se Framebuffer será passado por ponteiro ou cópia

#include <stddef.h>

void fb_clear(Framebuffer fb, uint32_t color) {
    for (int y = 0; y < fb.height; ++y) {
        uint32_t *row = fb.pixels + y * fb.stride;
        for (int x = 0; x < fb.width; ++x) {
            row[x] = color;
        }
    }
}

Framebuffer fb_view(Framebuffer src, int x0, int y0, int view_width, int view_height) {
    if ((!src.pixels)) return (Framebuffer){0};

    Framebuffer fb;
    uintptr_t offset_view = y0 * src.stride + x0;
    fb.width = view_width;
    fb.height = view_height;
    fb.pixels = src.pixels + offset_view;
    fb.stride = src.stride;

    return fb;
}

Framebuffer fb_create_from_memory(uint32_t *pixel_data, int width, int height, int stride) {
    Framebuffer fb = {.width = width, .height = height, .stride = stride};
    fb.pixels = pixel_data;
    return fb;
}

Framebuffer fb_create(int width, int height) {
    // Framebuffer *fb = malloc(sizeof(Framebuffer));
    Framebuffer fb = {.width = width, .height = height, .stride = width};
    fb.pixels = malloc(width * height * sizeof(uint32_t));

    if (!fb.pixels) return (Framebuffer){0};

    return fb;
}

void fb_destroy(Framebuffer fb) {
    free(fb.pixels);
}