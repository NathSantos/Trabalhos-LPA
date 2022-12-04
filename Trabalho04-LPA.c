// ======================================== QUARTO TRABALHO ========================================
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct evento_t {
    double tempo;
    int alvo;
    int tipo;
}evento_t;

typedef struct lista_eventos_t {
    evento_t *evento;
    struct lista_eventos_t *proximo;
}lista_eventos_t;

typedef struct lista_vizinhos_t{
    int id;
    struct lista_vizinhos_t *proximo;
}lista_vizinhos_t;

typedef struct no_t {
    int id;
    double x;
    double y;
    bool pacote_enviado;
    lista_vizinhos_t *vizinhos;
}no_t;

typedef no_t* grafo_t;

bool lista_eventos_adicionar_ordenado(evento_t *evento, lista_eventos_t **lista) {
    lista_eventos_t *novo_evento = malloc(sizeof(lista_eventos_t));
    lista_eventos_t *item_atual;
    
    if (novo_evento == NULL) return false;
    novo_evento->evento = evento;
    novo_evento->proximo = NULL;

    // caso a lista esteja vazia
    if(*lista == NULL){
        *lista = novo_evento;
    }

    // caso a lista NÃO esteja vazia
    else{
        item_atual = *lista;
        // verifica se o tempo do novo evento for menor do que o tempo do primeiro item da lista
        if(novo_evento->evento->tempo  <  item_atual->evento->tempo) {
            novo_evento->proximo = *lista;
            *lista = novo_evento;
        }
        else {
            // percorre o resto da lista
            while((item_atual->proximo != NULL)  &&  (novo_evento->evento->tempo  >  item_atual->proximo->evento->tempo)) // vai até o último nó da lista
                item_atual = item_atual->proximo;
            novo_evento->proximo = item_atual->proximo;
            item_atual->proximo = novo_evento;

        }
    }
    return true;
}

bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos_t **lista) {
    lista_vizinhos_t *novo_no = (lista_vizinhos_t*)malloc(sizeof(lista_vizinhos_t));
    if (novo_no == NULL) return false;
    novo_no->id= vizinho;
    novo_no->proximo = *lista;
    *lista = novo_no;
    return true;
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

void simulacao_iniciar(lista_eventos_t **lista, grafo_t grafo) {
    while(*lista != NULL) {
        lista_eventos_t *lista_aux = (lista_eventos_t*) malloc(sizeof(lista_eventos_t));
        evento_t *evento_aux = (evento_t*) malloc(sizeof(evento_t));

        lista_aux = *lista;
        evento_aux = lista_aux->evento;
        *lista = lista_aux->proximo;

        printf("[%3.6f] Nó %d recebeu pacote\n", evento_aux->tempo, evento_aux->alvo);

        if(grafo[evento_aux->alvo].pacote_enviado == false) {
            lista_vizinhos_t *lista_vizinhos;
            for(lista_vizinhos = grafo[evento_aux->alvo].vizinhos; lista_vizinhos != NULL; lista_vizinhos = lista_vizinhos->proximo) {
                evento_t *novo_evento = (evento_t*) malloc(sizeof(evento_t));
                novo_evento->alvo = lista_vizinhos->id;
                novo_evento->tempo = evento_aux->tempo + (0.1 + (lista_vizinhos->id * 0.01));
                novo_evento->tipo = 1;
                lista_eventos_adicionar_ordenado(novo_evento, lista);
                printf("    --> Repassando pacote para o nó %d ...\n", novo_evento->alvo);
            }
            grafo[evento_aux->alvo].pacote_enviado = true;
        }
    }
}

// =====================================================================================

void main(int argc, char *argv[]) {
    FILE *arq;
    arq = fopen(argv[1], "r");
    lista_eventos_t *lista_eventos = NULL;

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
        
        evento_t *evento_aux = (evento_t*) malloc(sizeof(evento_t));
        evento_aux->tempo = 1;
        evento_aux->alvo = 0;
        evento_aux->tipo = 1;
        lista_eventos_adicionar_ordenado(evento_aux, &lista_eventos);

        simulacao_iniciar(&lista_eventos, grafo);
    }   
}   