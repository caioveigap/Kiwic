#ifndef FB_H
#define FB_H

#include <stdint.h>

typedef struct { uint32_t *pixels; int width, height, stride; } Framebuffer;


void fb_clear(Framebuffer fb, uint32_t color);
Framebuffer fb_view(Framebuffer src, int x0, int y0, int view_width, int view_height);
Framebuffer fb_create_from_memory(uint32_t *pixel_data, int width, int height, int stride);
Framebuffer fb_create(int width, int height);
void fb_destroy(Framebuffer fb);


#endif /* FB_H */