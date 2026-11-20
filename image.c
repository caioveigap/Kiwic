#include "image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>



PPM_Image* ppm_read_image(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error: Could not open file");
        return NULL;
    }

    // get file size
    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);

    // get file descriptor
    int file_desc = fileno(file);

    //go back to beginning of file
    rewind(file);

    PPM_Image *result = malloc(sizeof(PPM_Image));

    char type[3] = {0};
    fscanf(file, "%2s", type);
    if (strcmp(type, "P6") != 0) {
        // fprintf em vez de perror: aqui nenhuma chamada de sistema falhou, então o errno
        // não diz nada sobre o erro (o perror imprimiria algo como "...: Success").
        fprintf(stderr, "Error: This is not a .ppm (P6) file!\n");
        free(result); // libera o PPM_Image alocado acima, que vazava neste caminho de erro
        fclose(file);
        return NULL;
    }

    fscanf(file, "%u %u", &(result->cols), &(result->rows));
    fscanf(file, "%u", &(result->maxval));

    off_t offset = ftell(file);

    uint8_t *rawdata = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, file_desc, 0);

    if(rawdata == MAP_FAILED) {
        perror("Error: mmap error.");
        free(result); // libera o PPM_Image, que vazava neste caminho de erro
        fclose(file);
        return NULL;
    }

    uint8_t bytes_per_value = (result->maxval > 256 ? 2 : 1);
    const int values_per_pixel = 3;

    uint8_t *pixeldata = rawdata + offset + 1;

    // Validação de tamanho: o arquivo precisa conter todos os bytes de pixel que o
    // cabeçalho promete. Sem isso, um arquivo truncado faria o loop abaixo ler além do
    // fim do mapeamento (SIGBUS/SIGSEGV ou lixo). Conta feita em size_t para não
    // estourar em uint32_t com imagens grandes.
    size_t header_size = (size_t)offset + 1;
    size_t data_size = (size_t)result->rows * result->cols * values_per_pixel * bytes_per_value;
    if (header_size + data_size > filesize) {
        fprintf(stderr, "Error: file is smaller than its header says (truncated?)\n");
        munmap(rawdata, filesize);
        free(result);
        fclose(file);
        return NULL;
    }

    result->pixels = calloc(result->rows, sizeof(Pixel*));
    for (int y=0; y < result->rows; y++) {
        result->pixels[y] = calloc(result->cols, sizeof(Pixel));
        for (int x=0; x < result->cols; x++) {
            uint8_t *pixel_start = pixeldata + (y * result->cols * values_per_pixel * bytes_per_value) + (x * values_per_pixel * bytes_per_value);

            if (bytes_per_value == 2) {
                // 16 bits: cada amostra ocupa 2 bytes em big-endian (byte mais
                // significativo primeiro), como manda a spec Netpbm. Montamos o valor
                // manualmente para não depender do endianness da máquina (x86 é little-endian).
                result->pixels[y][x].r = (pixel_start[0] << 8) | pixel_start[1];
                result->pixels[y][x].g = (pixel_start[2] << 8) | pixel_start[3];
                result->pixels[y][x].b = (pixel_start[4] << 8) | pixel_start[5];
            } else {
                result->pixels[y][x].r = pixel_start[0];
                result->pixels[y][x].g = pixel_start[1];
                result->pixels[y][x].b = pixel_start[2];
            }
        }
    }

    // Os pixels já foram copiados para result->pixels, então o mapeamento e o arquivo
    // não são mais necessários. Antes, os dois vazavam a cada leitura.
    munmap(rawdata, filesize);
    fclose(file);

    return result;
}
bool ppm_write_image(const char *filename, PPM_Image *image) {
    FILE *file = fopen(filename, "wb");

    if (file == NULL) {
        perror("Error: could not open file");
        return false;
    }

    fprintf(file, "P6\n%u %u\n%u\n", image->cols, image->rows, image->maxval);

    uint8_t bytes_per_value = (image->maxval > 256 ? 2 : 1);
    for (int y=0; y < image->rows; y++) {
        for (int x=0; x < image->cols; x++) {
            Pixel p = image->pixels[y][x];
            if (bytes_per_value == 2) {
                // 16 bits: gravamos cada amostra em big-endian (byte alto primeiro), como
                // manda a spec. Um fwrite direto do uint16_t gravaria na ordem da máquina
                // (little-endian no x86), invertendo os bytes.
                uint8_t bytes[6] = {
                    p.r >> 8, p.r & 0xFF,
                    p.g >> 8, p.g & 0xFF,
                    p.b >> 8, p.b & 0xFF,
                };
                fwrite(bytes, 1, sizeof(bytes), file);
            } else {
                // 8 bits: as amostras agora são uint16_t, então convertemos para 1 byte
                // antes de gravar (o fwrite antigo de &r gravaria o byte errado/metade do valor).
                uint8_t bytes[3] = { p.r, p.g, p.b };
                fwrite(bytes, 1, sizeof(bytes), file);
            }
        }
    }

    // fclose descarrega o buffer do FILE no disco e libera o descritor. Antes isso
    // vazava e só funcionava porque o exit() do programa descarrega os buffers.
    fclose(file);
    return true;
}

void ppm_destroy(PPM_Image *image) {
    for (int y=0; y < image->rows; y++) {
        free(image->pixels[y]);
    }
    // Antes só as linhas eram liberadas; o array de ponteiros e a própria struct vazavam.
    free(image->pixels);
    free(image);
}