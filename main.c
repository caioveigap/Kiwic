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
#include <libgen.h>
#include <math.h>
#include "image.h"

/* CONSTANTS */
#define WIDTH 400
#define HEIGHT 400

// hue é uma fração da volta no círculo de cores, em [0, 1): 0 = vermelho,
// 1/3 = verde, 2/3 = azul. Antes a função esperava graus [0, 360), mas quem chama
// passa y / rows (0..1), então só o começo do setor vermelho era usado.
Pixel hue_to_rgb(float hue) {
    // Wrap hue into [0, 1) — valores fora disso dão a volta no círculo
    hue = fmodf(hue, 1.0f);
    if (hue < 0.0f) {
        hue += 1.0f;
    }

    // h_prime em [0, 6): índice do setor (cada setor = 1/6 da volta, ou 60°)
    float h_prime = hue * 6.0f;
    float x = 1.0f - fabsf(fmodf(h_prime, 2.0f) - 1.0f);
    float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f;

    if (h_prime < 1.0f)      { r1 = 1.0f; g1 = x;    b1 = 0.0f; }
    else if (h_prime < 2.0f) { r1 = x;    g1 = 1.0f; b1 = 0.0f; }
    else if (h_prime < 3.0f) { r1 = 0.0f; g1 = 1.0f; b1 = x;    }
    else if (h_prime < 4.0f) { r1 = 0.0f; g1 = x;    b1 = 1.0f; }
    else if (h_prime < 5.0f) { r1 = x;    g1 = 0.0f; b1 = 1.0f; }
    else                     { r1 = 1.0f; g1 = 0.0f; b1 = x;    }

    Pixel color = {
        .r = (uint8_t)roundf(r1 * 255.0f),
        .g = (uint8_t)roundf(g1 * 255.0f),
        .b = (uint8_t)roundf(b1 * 255.0f)
    };
    return color;
}

// static uint32_t pixels[WIDTH][HEIGHT];

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *filename = argv[1];
    char *output_filename = NULL;
    asprintf(&(output_filename), "rainbow_%s", basename(filename));

    PPM_Image *image = ppm_read_image(filename);

    for (int x=0; x < image->cols; x++) {
        for (int y=0; y < image->rows; y++) {
            float hue = ((float)y) / ((float)image->rows);

            Pixel color = hue_to_rgb(hue);
            const float weight = 0.5;

            image->pixels[y][x].r = (image->pixels[y][x].r * (1-weight)) + (color.r * weight);
            image->pixels[y][x].g = (image->pixels[y][x].g * (1-weight)) + (color.g * weight);
            image->pixels[y][x].b = (image->pixels[y][x].b * (1-weight)) + (color.b * weight);
        }
    }

    ppm_write_image(output_filename, image);
    ppm_destroy(image);

    return EXIT_SUCCESS;
}