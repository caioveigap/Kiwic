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
#include <string.h>
#include "image.h"



void get_output_filename(const char *filename_in, char *filename_out) {
    const char *dot = strrchr(filename_in, '.');
    const char *slash = strrchr(filename_in, '/');
    int base_len = (dot && (!slash || dot > slash))
        ? (int)(dot - filename_in)
        : (int)strlen(filename_in);
    snprintf(filename_out, 128, "%.*s_out.ppm", base_len, filename_in);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char filename_out[128];
    get_output_filename(argv[1], filename_out);

    PPM_Image *image = ppm_read_image(argv[1]);
    ppm_write_image(filename_out, image);

    
    return EXIT_SUCCESS;
}