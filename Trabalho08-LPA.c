// ======================================== OITAVO TRABALHO ========================================
#include <stdio.h> 
#include <string.h>
#include <stdbool.h>

// =====================================================================================

void main(int argc, char *argv[]) {

    FILE *mpeg_file = fopen(argv[1], "rb");
    unsigned char byte[4];

    while ((feof(mpeg_file) == false)){

        fread(byte, sizeof(unsigned char), 4, mpeg_file);

        if(memcmp(byte, "\x00\x00\x01", 3) == 0) {  // verificando se temos um start code prefix

            if(byte[3] == 0xB3) {   // se for stream do tipo Sequence
                unsigned char byte1 = fgetc(mpeg_file);
                unsigned char byte2 = fgetc(mpeg_file);
                unsigned char byte3 = fgetc(mpeg_file);
                unsigned char byte4 = fgetc(mpeg_file);
                unsigned int largura = byte1 * 16 + (byte2 >> 4);
                unsigned int altura = ( byte2 & 0x0F) * 256 + byte3;
                unsigned int frame_rate = byte4 & 0x0f;
                float valores_fps[8] = {23.976, 24.000, 25.000, 29.970, 30.000, 50.000, 59.940, 60.000};
                printf("--> Código: %.2x -- Sequence Header -- Width = %d, Height = %d -- Frame rate = %.3ffps\n", byte[3], largura, altura, valores_fps[frame_rate-1]);
            }
            else if(byte[3] == 0x00) {   // se for stream do tipo Picture
                fseek(mpeg_file, 1, SEEK_CUR);
                unsigned char byte2 = fgetc(mpeg_file);
                unsigned char valor = byte2 << 2;
                unsigned int tipo = valor >> 5;
                char valores_tipo[3] = {'I', 'P', 'B'};
                printf("--> Código: %.2x -- Picture -- Tipo = %c\n", byte[3], valores_tipo[tipo-1]);
            }
            else if(byte[3] == 0xBA) {printf("--> Código: %.2x -- Pack\n", byte[3]);}
            else if(byte[3] == 0xBB) {printf("--> Código: %.2x -- System\n", byte[3]);}
            else if(byte[3] == 0xB8) {printf("--> Código: %.2x -- Group of Pictures\n", byte[3]);}
            else if(byte[3] >= 0x01 && byte[3] <= 0xAF) {printf("--> Código: %.2x -- Slice\n", byte[3]);}
            else if(byte[3] >= 0xC0 && byte[3] <= 0xDF) {printf("--> Código: %.2x -- Packet Video\n", byte[3]);}
            else if(byte[3] >= 0xE0 && byte[3] <= 0xEF) {printf("--> Código: %.2x -- Packet Audio\n", byte[3]);}
        }
        else{fseek(mpeg_file, -3, SEEK_CUR);}  
    }
}   