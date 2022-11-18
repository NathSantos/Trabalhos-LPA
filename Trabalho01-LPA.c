// ======================================== PRIMEIRO TRABALHO ========================================
#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>

typedef struct evento_t {
    double tempo;
    int alvo;
    int tipo;
}evento_t;

typedef struct lista_eventos_t {
    evento_t *evento;
    struct lista_eventos_t *proximo;
}lista_eventos_t;

// ==================================== QUESTÃO 01 =====================================

bool lista_eventos_adicionar_inicio(evento_t *evento, lista_eventos_t **lista) {
    lista_eventos_t *novo_evento = malloc(sizeof(lista_eventos_t));
    if (novo_evento == NULL) return false;
    novo_evento->evento = evento;
    novo_evento->proximo = *lista;
    *lista = novo_evento;
    return true;
}

void lista_eventos_listar(lista_eventos_t *lista){
    lista_eventos_t *lista_aux;
    int i=1;
    for(lista_aux = lista; lista_aux != NULL; lista_aux = lista_aux->proximo, i++) {
        printf("Evento %d:\n", i);
        printf("Tempo = %f\n", lista_aux->evento->tempo);
        printf("Alvo = %d\n", lista_aux->evento->alvo);
        printf("Tipo = %d\n", lista_aux->evento->tipo);
    } 
}

// =====================================================================================
// ==================================== QUESTÃO 02 =====================================

bool lista_eventos_adicionar_fim(evento_t *evento, lista_eventos_t **lista){
    lista_eventos_t *novo_evento = malloc(sizeof(lista_eventos_t));
    lista_eventos_t *item_atual;

    if (novo_evento == NULL) return false;
    novo_evento->evento = evento;
    novo_evento->proximo = NULL;
    
    // caso a lista esteja vazia
    if(*lista == NULL)
        *lista = novo_evento;
    // caso a lista NÃO esteja vazia
    else{
        item_atual = *lista;
        while(item_atual->proximo) // vai até o último nó da lista
            item_atual = item_atual->proximo;
        item_atual->proximo = novo_evento;
    }
    return true;
}

// =====================================================================================
// ==================================== QUESTÃO 03 =====================================

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

// =====================================================================================

void main(int argc, char *argv[]) {
    FILE *arq;
    arq = fopen(argv[1], "r");
    lista_eventos_t *lista_questao1 = NULL;
    lista_eventos_t *lista_questao2 = NULL;
    lista_eventos_t *lista_questao3 = NULL;

    if (arq == NULL){
        printf("Problemas na leitura do arquivo\n");
        return;
    }

    while (feof(arq) == false){
        float tempo;
        int alvo;
        int tipo;
        fscanf(arq, "%f\t%d\t%d\n", &tempo, &alvo, &tipo);
        evento_t *novo_evento_t = malloc(sizeof(evento_t));
        novo_evento_t->tempo = tempo;
        novo_evento_t->alvo = alvo;
        novo_evento_t->tipo = tipo;
        lista_eventos_adicionar_inicio(novo_evento_t, &lista_questao1);
        lista_eventos_adicionar_fim(novo_evento_t, &lista_questao2);
        lista_eventos_adicionar_ordenado(novo_evento_t, &lista_questao3);
    }

    printf("================== PRIMEIRA QUESTÃO ==================\n");
    lista_eventos_listar(lista_questao1);
    printf("================== SEGUNDA QUESTÃO ==================\n");
    lista_eventos_listar(lista_questao2); 
    printf("================== TERCEIRA QUESTÃO ==================\n"); 
    lista_eventos_listar(lista_questao3);            
}