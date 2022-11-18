// ======================================== TERCEIRO TRABALHO ========================================
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct no_t {
    int id;
    double X;
    double Y;
}no_t;

typedef struct lista_vizinhos_t{
    no_t *no;
    struct lista_vizinhos_t *proximo;
}lista_vizinhos_t;

bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos_t **lista) {
    lista_vizinhos_t *novo_no = malloc(sizeof(lista_vizinhos_t));
    if (novo_no == NULL) return false;
    novo_no->id = vizinho;
    novo_no->proximo = *lista;
    *lista = novo_no;
    return true;
}

void lista_vizinhos_imprimir(lista_vizinhos_t *lista) {
    lista_vizinhos_t *lista_aux;
    int i=1;
    for(lista_aux = lista; lista_aux != NULL; lista_aux = lista_aux->proximo, i++) {
        printf("NÓ %d\n:", lista_aux->no->id);
    }
}

// =====================================================================================

void main(int argc, char **argv[]) {
    lista_vizinhos_t a = [];
    lista_vizinhos_adicionar(0, a);
    lista_vizinhos_adicionar(2, a);
    lista_vizinhos_adicionar(5, a);

    lista_vizinhos_imprimir(a);
}