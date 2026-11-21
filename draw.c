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
#include <stdint.h>
#include <assert.h>
#include "image.h"

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768

/* COLORS */

/* RGB FORMAT: 0xAABBGGRR*/

#define RED     0x000000FF
#define GREEN   0x0000FF00
#define BLUE    0x00FF0000

#define GRAY    0x22252b

typedef struct { uint32_t *pixels; int width, height, stride; } Bitmap;

// static uint32_t pixels[DEFAULT_HEIGHT][DEFAULT_WIDTH];

void draw_pixel(Bitmap dst, int x, int y, uint32_t color) {
    size_t offset = y * dst.stride + x;
    dst.pixels[offset] = color;
}

void fill_rect(Bitmap dst, int x0, int y0, int w, int h, uint32_t color) {
    assert((x0 + w) <= dst.width && "ASSERT: invalid width");
    assert((y0 + h) <= dst.height && "ASSERT: invalid height");

    for (int y = y0; y < y0 + h; y++) {
        for (int x = x0; x < x0 + w; x++) {
            dst.pixels[y*dst.stride + x] = color;
        }
    }
}

Bitmap bitmap_view(Bitmap src, int x0, int y0, int view_width, int view_height) {
    if ((!src.pixels)) return (Bitmap){0};

    Bitmap dst;
    uintptr_t offset_view = y0 * src.stride + x0;
    dst.width = view_width;
    dst.height = view_height;
    dst.pixels = src.pixels + offset_view;
    dst.stride = src.stride;

    return dst;
}

Bitmap bitmap_create_from_memory(uint32_t *pixel_data, int width, int height, int stride) {
    Bitmap bmp = {.width = width, .height = height, .stride = stride};
    bmp.pixels = pixel_data;
    return bmp;
}

Bitmap bitmap_create(int width, int height) {
    // Bitmap *bmp = malloc(sizeof(Bitmap));
    Bitmap bmp = {.width = width, .height = height, .stride = width};
    bmp.pixels = malloc(width * height * sizeof(uint32_t));

    if (!bmp.pixels) return (Bitmap){0};

    return bmp;
}

void bitmap_destroy(Bitmap bmp) {
    free(bmp.pixels);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: %s <filepath>\n", argv[0]);
        return EXIT_FAILURE;       
    }

    Bitmap dst = bitmap_create(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    fill_rect(dst, 0, 0, dst.width, dst.height, GRAY);

    Bitmap view = bitmap_view(dst, 0, 0, dst.width, dst.height);
    uint32_t color = RED;
    for (int i = 0; i < 8; i++) {
        int x0 = (view.width/2 - (view.width/2)/2);
        int y0 = (view.height/2 - (view.height/2)/2);
        view = bitmap_view(view, x0, y0, view.width/2, view.height/2);
        color = (color == RED) ? GREEN : RED;
        fill_rect(view, 0, 0, view.width, view.height, color);
    }

    ppm_save_pixels(dst.pixels, dst.width, dst.height, argv[1]);

    bitmap_destroy(dst);

    return EXIT_SUCCESS;
}