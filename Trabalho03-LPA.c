// ======================================== TERCEIRO TRABALHO ========================================
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct lista_vizinhos_t{
    int id;
    struct lista_vizinhos_t *proximo;
}lista_vizinhos_t;

typedef struct no_t {
    int id;
    double x;
    double y;
    lista_vizinhos_t *vizinhos;
}no_t;

typedef no_t* grafo_t;

bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos_t **lista) {
    lista_vizinhos_t *novo_no = (lista_vizinhos_t*)malloc(sizeof(lista_vizinhos_t));
    if (novo_no == NULL) return false;
    novo_no->id= vizinho;
    novo_no->proximo = *lista;
    *lista = novo_no;
    return true;
}

void lista_vizinhos_imprimir(lista_vizinhos_t *lista) {
    lista_vizinhos_t *lista_aux;
    for(lista_aux = lista; lista_aux != NULL; lista_aux = lista_aux->proximo) {
        printf("%d ", lista_aux->id);
    }
    printf("\n");
}

grafo_t grafo_criar(int tam) {
    grafo_t grafo = (grafo_t) malloc(tam*sizeof(no_t));
    return grafo;
}

void grafo_atualizar_vizinhos(int tam, double raio_comunicacao, grafo_t grafo) {
    for(int i=0;i<tam;i++) {    // i -> id do nó principal
        for(int j=0;j<tam;j++) {    // j -> id dos outros nós que estamos comparando com o principal
            if(i != j){ // verificando se não é o mesmo nó (não faz sentido comparar dois nós iguais)
                double distancia = sqrt(pow(grafo[i].x - grafo[j].x, 2) + pow(grafo[i].y - grafo[j].y, 2));
                if(distancia < raio_comunicacao) {
                    lista_vizinhos_adicionar(grafo[i].id, &grafo[j].vizinhos);
                }
            }
        }
    }
}

void grafo_imprimir(int tam, grafo_t grafo){
    grafo_t grafo_aux;
    for(int i = 0; i<tam; i++) {
        printf("NÓ %d: ", i);
        lista_vizinhos_imprimir(grafo[i].vizinhos);
    }
}

// =====================================================================================

void main(int argc, char *argv[]) {
    FILE *arq;
    arq = fopen(argv[1], "r");

    if (arq == NULL){
        printf("Problemas na leitura do arquivo\n");
        return;
    }

    while (feof(arq) == false){
        // lendo quantidade de nós e raio de comunicação (primeira linha do arquivo)
        int quant_nos;
        double raio_de_comunicacao;
        fscanf(arq, "%d\t%lf\n", &quant_nos, &raio_de_comunicacao);

        // criando o grafo
        grafo_t grafo = grafo_criar(quant_nos);

        // lendo os dados das outras linhas do arquivo
        for(int i=0;i<quant_nos;i++) {
            fscanf(arq, "%d\t%lf\t%lf\n", &grafo[i].id, &grafo[i].x, &grafo[i].y);
            grafo[i].vizinhos = NULL;
        }

        grafo_atualizar_vizinhos(quant_nos, raio_de_comunicacao, grafo);
        grafo_imprimir(quant_nos, grafo);
    }   
}   