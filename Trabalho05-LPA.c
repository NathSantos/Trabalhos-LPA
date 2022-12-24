// ======================================== QUINTO TRABALHO ========================================
#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char* chars;

void generateAllCaracteres(char* sequence, int pos, int size) {
    if(pos < size) {
        for(int i=0; i<strlen(chars); i++) {
            char *tmp = malloc(sizeof(char)*size + 1);
            sprintf(tmp, "%s%c", sequence, chars[i]);
            generateAllCaracteres(tmp, pos+1, size);
        }
    }
    else {
        printf("%s\n", sequence);
    }
}

void main(int argc, char *argv[]) {
    
    int tam_maximo = atoi(argv[1]);
    chars = argv[2];

    for(int i=1; i<=tam_maximo; i++) {
        generateAllCaracteres("", 0, i);
    }
}