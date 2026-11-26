/* 
Etapa 1 — O "Surface" e Primitivas Sólidas: Crie uma struct Canvas { uint32_t *pixels; int width, height, stride; };. 
Implemente clear_canvas(), draw_pixel() e draw_rect_filled() com clipping rigoroso nas bordas da tela.

Etapa 2 — Linhas, Círculos e Blitting de Sprites: 
Implemente o algoritmo de Bresenham para linhas, o algoritmo de Ponto Médio (ou distância quadrática) para círculos, e a função draw_surface()
que copia uma sub-região (src_rect) de uma imagem carregada via stb_image para o seu Canvas aplicando Alpha Blending.

Etapa 3 — Câmera 2D e Transformações: Crie uma estrutura Camera2D (posição x, y, zoom, rotation) 
que transforma as coordenadas do mundo antes de desenhar no canvas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include "image.h"
#include "types.h"
#include "fb.h"

#define DEFAULT_WIDTH 128
#define DEFAULT_HEIGHT 128



static inline put_pixel(Framebuffer fb, int x, int y, uint32_t color) {
    if (x < 0 || y < 0 || x >= fb.width || y >= fb.height) return; // bounds check
    fb.pixels[y * fb.stride + x] = color;
}


int round_f(double n) {
    return (int)floor((0.5 + n));
}

static inline void swap_int(int *a, int *b) { int t = *a; *a = *b; *b = t; }

void draw_rect(Framebuffer fb, int x0, int y0, int w, int h, uint32_t color) {
    assert((x0 + w) <= fb.width && "ASSERT: invalid width");
    assert((y0 + h) <= fb.height && "ASSERT: invalid height");

    for (int y = y0; y < y0 + h; y++) {
        line_v4(fb, x0, y, x0 + w, y, color);
    }
}


/* Different line drawing algorithms implementations */
void line_v1(Framebuffer fb, int x0, int y0, int x1, int y1, uint32_t color) {
    float m = (float)(y1 - y0) / (float)(x1 - x0);
    float b = y0 - m * x0;

    for (int x = x0; x <= x1; x++) {
        float y = m * x + b; // Classic formula: y = mx + b
        put_pixel(fb, x, (int)round_f(y), color);
    }
}

void line_v2(Framebuffer fb, int x0, int y0, int x1, int y1, uint32_t color) {
    if (x0 > x1) {
        swap_int(&x0, &x1);
        swap_int(&y0, &y1);
    }
    float m = (float)(y1 - y0) / (float)(x1 - x0);
    float b = y0 - m * x0;

    printf("Slope: %f (%d - %d/ %d - %d)\n", m, y1, y0, x1, x0);

    for (int x = x0; x <= x1; x++) {
        float y = m * x + b; // Classic formula: y = mx + b
        put_pixel(fb, x, (int)round_f(y), color);
    }
}

void line_v3(Framebuffer fb, int x0, int y0, int x1, int y1, uint32_t color) {
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        swap_int(&x0, &y0);
        swap_int(&x1, &y1);
    }

    if (x0 > x1) {
        swap_int(&x0, &x1);
        swap_int(&y0, &y1);
    }

    int dx = x1 - x0;
    for (int x = x0; x <= x1; x++) {
        float t = dx ? (float)(x - x0) / dx : 0.0f;
        int y = (int)round_f(y0 + t * (y1 - y0));
        if (steep) put_pixel(fb, y, x, color);
        else put_pixel(fb, x, y, color);
    }

}

// DDA Algorithm
void line_v4(Framebuffer fb, int x0, int y0, int x1, int y1, uint32_t color) {
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        swap_int(&x0, &y0);
        swap_int(&x1, &y1);
    }

    if (x0 > x1) {
        swap_int(&x0, &x1);
        swap_int(&y0, &y1);
    }

    int dx = x1 - x0;
    float m = dx ? (float)(y1 - y0) / dx : 0.0f;
    float y = (float)y0;
    for (int x = x0; x <= x1; x++) {
        int yi = (int)round_f(y);
        if (steep) put_pixel(fb, yi, x, color);
        else put_pixel(fb, x, yi, color);
        y += m;
    }

}


// Draw a triangle using lines.
void draw_triangle_lines(Framebuffer fb, Vec2 v1, Vec2 v2, Vec2 v3, uint32_t color) {
    line_v4(fb, v1.x, v1.y, v2.x, v2.y, color);
    line_v4(fb, v1.x, v1.y, v3.x, v3.y, color);
    line_v4(fb, v2.x, v2.y, v3.x, v3.y, color);
}




void draw_random_line(Framebuffer fb) {
    int rix, riy, rfx, rfy;

    rix = rand() % fb.width;
    riy = rand() % fb.height;
    rfx = rand() % fb.width;
    rfy = rand() % fb.height;

    size_t size = sizeof(G_COLORS) / sizeof(uint32_t);
    int color_idx = rand() % size;
    uint32_t color = G_COLORS[color_idx];

    line_v4(fb, rix, riy, rfx, rfy, color);

}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: %s <filepath>\n", argv[0]);
        return EXIT_FAILURE;       
    }

    uint32_t color = RED;
    Framebuffer fb = fb_create(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    fb_clear(fb, GRAY);

    srand((unsigned)time(NULL));
    // for (int i = 0; i < 50; i++) {
    //     draw_random_line(fb);
    // }

    Vec2 v1 = {30, 90};
    Vec2 v2 = {98, 90};
    Vec2 v3 = {64, 30};

    draw_triangle_lines(fb, v1, v2, v3, GREEN);
    draw_rect(fb, 30, 30, 60, 50, RED);
    ppm_save_pixels(fb.pixels, fb.width, fb.height, argv[1]);

    fb_destroy(fb);

    return EXIT_SUCCESS;
}