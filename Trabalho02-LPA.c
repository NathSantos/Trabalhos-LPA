// ======================================== SEGUNDO TRABALHO ========================================
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int tabela_hash_tam;

typedef struct pessoa_t {
    char nome[50];
    long long int cpf;
    int idade;
}pessoa_t;

typedef struct lista_pessoas_t{
    pessoa_t *pessoa;
    struct lista_pessoas_t*proximo;
}lista_pessoas_t;

typedef lista_pessoas_t** tabela_hash_t;

bool lista_pessoas_adicionar(pessoa_t *pessoa, lista_pessoas_t**lista) {
    lista_pessoas_t*nova_pessoa = malloc(sizeof(lista_pessoas_t));
    if (nova_pessoa == NULL) return false;
    nova_pessoa->pessoa = pessoa;
    nova_pessoa->proximo = *lista;
    *lista = nova_pessoa;
    return true;
}

void lista_pessoas_listar(lista_pessoas_t*lista){
    lista_pessoas_t*lista_aux;
    int i=1;
    for(lista_aux = lista; lista_aux != NULL; lista_aux = lista_aux->proximo, i++) {
        printf("- %s\n", lista_aux->pessoa->nome);
        printf("- %lld\n", lista_aux->pessoa->cpf);
        printf("- %d\n", lista_aux->pessoa->idade);
    } 
}

tabela_hash_t tabela_hash_pessoas_criar() {
    tabela_hash_t tabela = (tabela_hash_t) malloc(tabela_hash_tam*sizeof(lista_pessoas_t*));
    for(int i=0; i<tabela_hash_tam; i++) {
        tabela[i] = NULL;
    }

    return tabela;
}

int tabela_hash_pessoas_funcao(pessoa_t *pessoa) {
    return(pessoa->cpf % tabela_hash_tam);
}

bool tabela_hash_pessoas_adicionar(pessoa_t *pessoa, tabela_hash_t tabela_hash) {
    int indice_lista = tabela_hash_pessoas_funcao(pessoa);
    lista_pessoas_t *lista = tabela_hash[indice_lista];
    lista_pessoas_adicionar(pessoa, &tabela_hash[indice_lista]);
}

void tabela_hash_pessoas_listar(tabela_hash_t tabela_hash) {
    for(int i=0;i<tabela_hash_tam;i++) {
        printf("POSIÇÃO %d DA TABELA HASH:\n", i);
        lista_pessoas_listar(tabela_hash[i]);
    }
}

// =====================================================================================

void main(int argc, char *argv[]) {

    sscanf(argv[1], "%d", &tabela_hash_tam);

    FILE *arq;
    arq = fopen(argv[2], "r");
    tabela_hash_t tabela = tabela_hash_pessoas_criar();

    if (arq == NULL){
        printf("Problemas na leitura do arquivo\n");
        return;
    }

    while (feof(arq) == false){
        char nome[50];
        long long int cpf;
        int idade;
        fscanf(arq, "%50[^\t]\t%lld\t%d\n", &nome, &cpf, &idade);
        pessoa_t *nova_pessoa = malloc(sizeof(pessoa_t));
        strcpy(nova_pessoa->nome,nome);
        nova_pessoa->cpf = cpf;
        nova_pessoa->idade = idade;
        tabela_hash_pessoas_adicionar(nova_pessoa, tabela);
    }

    tabela_hash_pessoas_listar(tabela);       
}