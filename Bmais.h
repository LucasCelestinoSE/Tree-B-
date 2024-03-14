#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ORDEM_PADRAO 3

typedef struct registro {
  int item;
} registro;

typedef struct No {
  void **pont;
  int *chaves;
  struct No *pai;
  bool folha;
  int n_chaves;
  struct No *prox;
} No;

int ordem = ORDEM_PADRAO;
No *fila = NULL;
bool detalhes_de_saida = false;

void enfileirar(No *novo_No);
No *desenfileirar(void);
int altura(No *const raiz);
int caminho_para_raiz(No *const raiz, No *filho);
void print_folhas(No *const raiz);
void print_arvore(No *const raiz);
void encontrar_imprimir(No *const raiz, int chave, bool detalhe);
void encontrar_imprimir_distancia(No *const raiz, int distancia1,
                                  int distancia2, bool detalhe);
int encontrar_distancia(No *const raiz, int chave_inical, int chave_final,
                        bool detalhe, int chaves_retornadas[],
                        void *ponteiros_retornados[]);
No *encontrar_folha(No *const raiz, int chave, bool detalhe);
registro *encontrar(No *raiz, int chave, bool detalhe, No **return_folha);
int corte(int tam);

registro *criar_registro(int item);
No *criar_No(void);
No *criar_folha(void);
int index_esquerda(No *pai, No *esquerda);
No *inserir_na_folha(No *folha, int chave, registro *ponteiro);
No *inserir_na_folha_depois_do_corte(No *raiz, No *folha, int chave,
                                     registro *ponteiro);
No *inserir_no_No(No *raiz, No *pai, int index_esquerda, int chave,
                  No *direita);
No *inserir_no_No_depois_do_corte(No *raiz, No *pai, int index_esquerda,
                                  int chave, No *direita);
No *inserir_no_pai(No *raiz, No *esquerda, int chave, No *direita);
No *inserir_em_nova_raiz(No *esquerda, int chave, No *direita);
No *iniciar_nova_arvore(int chave, registro *ponteiro);
No *inserir(No *raiz, int chave, int item);

int pegar_index_vizinho(No *n);
No *ajustar_raiz(No *root);
No *juntar_Nos(No *raiz, No *n, No *vizinho, int index_vizinho,
 int k_prime);
No *redistribuicao_Nos(No *raiz, No *n, No *vizinho, int vizinho_index,
 int k_prime_index, int k_prime);
No *deletar_entrada(No *raiz, No *n, int chave, void *ponteiro);
No *delete (No *root, int key);
