// ======================================== NONO TRABALHO ========================================
#include <stdio.h> 
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

// =====================================================================================

void main(int argc, char *argv[]) {

    FILE *mpeg_file = fopen(argv[1], "rb");

    int i = 1;
    char nome[20];
    sprintf(nome, "video_parte_%d.mpg", i);
    FILE *arq_saida = fopen(nome, "wb");
    int tam_saida = 0;
    int max_size = 0;

    printf("Criando arquivo %s ..\n", nome);

    sscanf(argv[2], "%d", &max_size);
    
    unsigned char *buffer = malloc(sizeof(unsigned char)*(max_size*1048576));
    int buffer_size = 0;    // contando a quantidade de bytes adicionados ao buffer
   
    unsigned char byte[4];

    while (true){

        fread(byte, sizeof(unsigned char), 4, mpeg_file);

        if(memcmp(byte, "\x00\x00\x01\xB3", 4) != 0 && feof(mpeg_file) == false) { 
            buffer[buffer_size] = byte[0];
            fseek(mpeg_file, -3, SEEK_CUR);
            buffer_size += 1;
            continue;
        }
        
        if((tam_saida + buffer_size) > (max_size*1048576)) {
            fclose(arq_saida);
            i++;
            sprintf(nome, "video_parte_%d.mpg", i);
            arq_saida = fopen(nome, "wb");
            printf("Criando arquivo %s ..\n", nome);
            fwrite(buffer, sizeof(unsigned char), buffer_size, arq_saida);
            tam_saida = buffer_size;
        }
        else {
            fwrite(buffer, sizeof(unsigned char), buffer_size, arq_saida);
            tam_saida += buffer_size;
        }
        
        if(feof(mpeg_file) == true) {
            fclose(arq_saida);
            break;
        }

        memcpy(buffer, "\x00\x00\x01\xB3", 4);
        buffer_size = 4;
    }
    free(buffer);
    fclose(mpeg_file);
}   