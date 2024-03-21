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
//FUNÇÃO PARA LOCALIZAR E IMPRIMIR PARES CHAVE-VALOR DENTRO DE UM INTERVALO ESPECIFICADO DA ÁRVORE.
void encontrar_imprimir_intervalo(No *const raiz, int distancia1,
                                  int distancia2, bool detalhe);
//FUNÇÃO PARA LOCALIZAR E RECUPERAR AS CHAVES E PONTEIROS ASSOCIADOS DENTRO DE UM INTERVALO ESPECIFICADO EM UMA ÁRVORE
int encontrar_distancia(No *const raiz, int chave_inical, int chave_final,
                        bool detalhe, int chaves_retornadas[],
                        void *ponteiros_retornados[]);
//FUNÇÃO PARA LOCALIZAR UM NÓ FOLHA NA ÁRVORE
No *encontrar_folha(No *const raiz, int chave, bool detalhe);
//FUNÇÃO PARA ENCONTRAR UMA CHAVE NA ÁRVORE B+ E RETORNAR O REGISTRO CORRESPONDENTE.
registro *encontrar(No *raiz, int chave, bool detalhe, No **return_folha);
int corte(int tam);
// Cria um Registro
registro *criar_registro(int item);
// Função de criar no
No *criar_No(void);
// Criar Função que usa criar No aplicando folha como true
No *criar_folha(void);
int index_esquerda(No *pai, No *esquerda);
// Insere uma chave no vetor presente na folha
No *inserir_na_folha(No *folha, int chave, registro *ponteiro);
// Insere na folha caso a quantidade de itens dento do vetor for maior que ordem - 1
No *inserir_na_folha_depois_do_corte(No *raiz, No *folha, int chave,
                                     registro *ponteiro);
// insere uma chave e um ponteiro à direita dessa chave em um nó n em uma posição específica index_esquerda
No *inserir_no_No(No *raiz, No *pai, int index_esquerda, int chave,
                  No *direita);
// Prepara arrays temporários para manipulação das chaves e ponteiros do nó, a fim de realizar a inserção da nova chave e ponteiro.
// Realiza a divisão do nó, criando um novo nó e redistribuindo as chaves e ponteiros entre o nó antigo e o novo nó.
No *inserir_no_No_depois_do_corte(No *raiz, No *pai, int index_esquerda,
                                  int chave, No *direita);
// Insere no pai alocando a direita e esquerda usando os vetores de ponteiros.
No *inserir_no_pai(No *raiz, No *esquerda, int chave, No *direita);
// Inserção em uma nova raiz, caso de balanceamento.
No *inserir_em_nova_raiz(No *esquerda, int chave, No *direita);
// Inicia os valores default em uma árvore.
No *iniciar_nova_arvore(int chave, registro *ponteiro);
// Função main que usa as funções acima para inserção.
No *inserir(No *raiz, int chave, int item);
// Função para pegar index vizinho
int pegar_index_vizinho(No *n);
//FUNÇÃO PARA AJUSTAR O NÓ RAIZ DE UMA ARVORE APÓS REMOÇÃO DE UMA ENTRADA
No *ajustar_raiz(No *root);
// FUNÇÃO PARA CONCATENAR DOIS NÓS
No *juntar_Nos(No *raiz, No *n, No *vizinho, int index_vizinho,
 int k_prime);
 // FUNÇÃO PARA REDISTRIBUIR CHAVES ENTRE DOIS NÓS
No *redistribuicao_Nos(No *raiz, No *n, No *vizinho, int vizinho_index,
 int k_prime_index, int k_prime);
// FUNÇÃO PARA MANIPULAR O PROCESSO DE REMOÇÃO DA ÁRVORE B+ (removendo uma entrada (par chave-valor) de um nó.)
No *remover_entrada(No *raiz, No *n, int chave, void *ponteiro);
// Remove a chave do Nó.
No *remover_entrada_do_No(No *n, int chave, No *pointer);
//FUNÇÃO PARA REMOVER CHAVE DA ARVORE B+
No *remover (No *root, int key);
