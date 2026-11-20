#include "image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#define SUPPORTED_MAXVAL 255

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

    int c = fgetc(file);
    if (c == '#') {
        fscanf(file, "%*[^ \t\n\r\v\f]");
    } else { 
        ungetc(c, file); 
    }

    fscanf(file, "%2s", type);
    if (strcmp(type, "P6") != 0) {
        // fprintf em vez de perror: aqui nenhuma chamada de sistema falhou, então o errno
        // não diz nada sobre o erro (o perror imprimiria algo como "...: Success").
        fprintf(stderr, "Error: This is not a .ppm (P6) file!\n");
        goto error;
    }

    fscanf(file, "%u %u", &(result->cols), &(result->rows));
    fscanf(file, "%u", &(result->maxval));
    if (result->maxval != SUPPORTED_MAXVAL) {
        fprintf(stdout, "Error: unsupported maxval\n");
        goto error;
    }

    off_t offset = ftell(file);

    uint8_t *rawdata = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, file_desc, 0);

    if(rawdata == MAP_FAILED) {
        perror("Error: mmap error.");
        goto error;
    }

    uint8_t *pixeldata = rawdata + offset + 1;

    // Validação de tamanho: o arquivo precisa conter todos os bytes de pixel que o
    // cabeçalho promete. Sem isso, um arquivo truncado faria o loop abaixo ler além do
    // fim do mapeamento (SIGBUS/SIGSEGV ou lixo). Conta feita em size_t para não
    // estourar em uint32_t com imagens grandes.
    size_t data_size = (size_t)result->rows * (size_t)result->cols * sizeof(Pixel);
    size_t header_size = (size_t)offset + 1;
    
    if (header_size + data_size > filesize) {
        fprintf(stderr, "Error: file is smaller than its header says (truncated?)\n");
        munmap(rawdata, filesize);
        goto error;
    }
    
    result->pixels = malloc(data_size);
    memcpy(result->pixels, pixeldata, data_size);
    


    // Os pixels já foram copiados para result->pixels, então o mapeamento e o arquivo
    // não são mais necessários. Antes, os dois vazavam a cada leitura.
    munmap(rawdata, filesize);
    fclose(file);
    return result;

    error:
        free(result);
        if (file) fclose(file);
        return NULL;
}


bool ppm_write_image(const char *filename, const PPM_Image *image) {
    if (image == NULL) {
        fprintf(stdout, "Error: invalid image\n");
        return false;
    }

    FILE *file = fopen(filename, "wb");

    if (file == NULL) {
        perror("Error: could not open file");
        return false;
    }


    fprintf(file, "P6\n%u %u\n%u\n", image->cols, image->rows, image->maxval);

    size_t pixel_array_size = (size_t)image->cols * image->rows;
    
    fwrite(image->pixels, sizeof(Pixel), pixel_array_size, file);

    fclose(file);
    return true;
}


void ppm_destroy(PPM_Image *image) {
    if (image == NULL) return;
    free(image->pixels);
    free(image);
}
