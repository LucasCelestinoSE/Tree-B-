// Deletion operation on a B+ Tree in C++

#include "b_plus.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void enfileirar(No *novo_No) {
  No *c;
  if (fila == NULL) {
    fila = novo_No;
    fila->prox = NULL;
  } else {
    c = fila;
    while (c->prox != NULL) {
      c = c->prox;
    }
    c->prox = novo_No;
    novo_No->prox = NULL;
  }
}

No *desenfileirar(void) {
  No *n = fila;
  fila = fila->prox;
  n->prox = NULL;
  return n;
}

void print_folhas(No *const raiz) {
  if (raiz == NULL) {
    printf("Árvore Vazia.\n");
    return;
  }
  int i;
  No *c = raiz;
  while (!c->folha)
    c = c->pont[0];
  while (true) {
    for (i = 0; i < c->n_chaves; i++) {
      if (detalhes_de_saida)
        printf("%p ", c->pont[i]);
      printf("%d ", c->chaves[i]);
    }
    if (detalhes_de_saida)
      printf("%p ", c->pont[ordem - 1]);
    if (c->pont[ordem - 1] != NULL) {
      printf(" | ");
      c = c->pont[ordem - 1];
    } else
      break;
  }
  printf("\n");
}

int altura(No *const raiz) {
  int altura = 0;
  No *c = raiz;
  while (!c->folha) {
    c = c->pont[0];
    altura++;
  }
  return altura;
}
int caminho_para_raiz(No *const raiz, No *filho) {
  int tamanho = 0;
  No *c = filho;
  while (c != raiz) {
    c = c->pai;
    tamanho++;
  }
  return tamanho;
}

void print_arvore(No *const raiz) {
  No *n = NULL;
  int i = 0;
  int pontos = 0;
  int novo_pontos = 0;

  if (raiz == NULL) {
    printf("Empty tree.\n");
    return;
  }
  fila = NULL;
  enfileirar(raiz);
  while (fila != NULL) {
    n = desenfileirar();
    if (n->pai != NULL && n == n->pai->pont[0]) {
      novo_pontos = caminho_para_raiz(raiz, n);
      if (novo_pontos != pontos) {
        pontos = novo_pontos;
        printf("\n");
      }
    }
    if (detalhes_de_saida)
      printf("(%p)", n);
    for (i = 0; i < n->n_chaves; i++) {
      if (detalhes_de_saida)
        printf("%p ", n->pont[i]);
      printf("%d ", n->chaves[i]);
    }
    if (!n->folha)
      for (i = 0; i <= n->n_chaves; i++)
        enfileirar(n->pont[i]);
    if (detalhes_de_saida) {
      if (n->folha)
        printf("%p ", n->pont[ordem - 1]);
      else
        printf("%p ", n->pont[n->n_chaves]);
    }
    printf("| ");
  }
  printf("\n");
}

void encontrar_imprimir(No *const raiz, int chave, bool detalhe) {
  No *folha = NULL;
  registro *r = encontrar(raiz, chave, detalhe, NULL);
  if (r == NULL)
    printf("Não há registros para essa chave. %d.\n", chave);
  else
    printf("registro em %p -- chave %d, item %d.\n", r, chave, r->item);
}

void encontrar_imprimir_distancia(No *const raiz, int chave_inicial,
                                  int chave_final, bool detalhe) {
  int i;
  int array_tamanho = chave_final - chave_inicial + 1;
  int chaves_retornadas[array_tamanho];
  void *ponteiros_retornados[array_tamanho];
  int num_encontrado =
      encontrar_distancia(raiz, chave_inicial, chave_final, detalhe,
                          chaves_retornadas, ponteiros_retornados);
  if (!num_encontrado)
    printf("Nada encontrado.\n");
  else {
    for (i = 0; i < num_encontrado; i++)
      printf("Chave: %d   Local: %p  Item: %d\n", chaves_retornadas[i],
             ponteiros_retornados[i],
             ((registro *)ponteiros_retornados[i])->item);
  }
}

int encontrar_distancia(No *const raiz, int chave_inicial, int chave_final,
                        bool detalhe, int chaves_retornadas[],
                        void *ponteiros_retornados[]) {
  int i, num_encontrado;
  num_encontrado = 0;
  No *n = encontrar_folha(raiz, chave_inicial, detalhe);
  if (n == NULL)
    return 0;
  for (i = 0; i < n->n_chaves && n->chaves[i] < chave_inicial; i++)
    ;
  if (i == n->n_chaves)
    return 0;
  while (n != NULL) {
    for (; i < n->n_chaves && n->chaves[i] <= chave_final; i++) {
      chaves_retornadas[num_encontrado] = n->chaves[i];
      ponteiros_retornados[num_encontrado] = n->pont[i];
      num_encontrado++;
    }
    n = n->pont[ordem - 1];
    i = 0;
  }
  return num_encontrado;
}

No *encontrar_folha(No *const raiz, int chave, bool detalhe) {
  if (raiz == NULL) {
    if (detalhe)
      printf("Arvore vazia.\n");
    return raiz;
  }
  int i = 0;
  No *c = raiz;
  while (!c->folha) {
    if (detalhe) {
      printf("[");
      for (i = 0; i < c->n_chaves - 1; i++)
        printf("%d ", c->chaves[i]);
      printf("%d] ", c->chaves[i]);
    }
    i = 0;
    while (i < c->n_chaves) {
      if (chave >= c->chaves[i])
        i++;
      else
        break;
    }
    if (detalhe)
      printf("%d ->\n", i);
    c = (No *)c->pont[i];
  }
  if (detalhe) {
    printf("Folha [");
    for (i = 0; i < c->n_chaves - 1; i++)
      printf("%d ", c->chaves[i]);
    printf("%d] ->\n", c->chaves[i]);
  }
  return c;
}

registro *encontrar(No *raiz, int chave, bool detalhe, No **return_folha) {
  if (raiz == NULL) {
    if (return_folha != NULL) {
      *return_folha = NULL;
    }
    return NULL;
  }

  int i = 0;
  No *folha = NULL;

  folha = encontrar_folha(raiz, chave, detalhe);

  for (i = 0; i < folha->n_chaves; i++)
    if (folha->chaves[i] == chave)
      break;
  if (return_folha != NULL) {
    *return_folha = folha;
  }
  if (i == folha->n_chaves)
    return NULL;
  else
    return (registro *)folha->pont[i];
}

int corte(int tam) {
  if (tam % 2 == 0)
    return tam / 2;
  else
    return tam / 2 + 1;
}

registro *criar_registro(int item) {
  registro *novo_registro = (registro *)malloc(sizeof(registro));
  if (novo_registro == NULL) {
    perror("Error em criar registro.");
    exit(EXIT_FAILURE);
  } else {
    novo_registro->item = item;
  }
  return novo_registro;
}

No *criar_No(void) {
  No *novo_No;
  novo_No = malloc(sizeof(No));
  if (novo_No == NULL) {
    perror("No nao criado.");
    exit(EXIT_FAILURE);
  }
  novo_No->chaves = malloc((ordem - 1) * sizeof(int));
  if (novo_No->chaves == NULL) {
    perror("Error nas chaves - fazerNo.");
    exit(EXIT_FAILURE);
  }
  novo_No->pont = malloc(ordem * sizeof(void *));
  if (novo_No->pont == NULL) {
    perror(" array de nós de ponteiros.");
    exit(EXIT_FAILURE);
  }
  novo_No->folha = false;
  novo_No->n_chaves = 0;
  novo_No->pai = NULL;
  novo_No->prox = NULL;
  return novo_No;
}

No *criar_folha(void) {
  No *folha = criar_No();
  folha->folha = true;
  return folha;
}

int index_esquerda(No *pai, No *esquerda) {
  int index_esquerda = 0;
  while (index_esquerda <= pai->n_chaves &&
         pai->pont[index_esquerda] != esquerda)
    index_esquerda++;
  return index_esquerda;
}

No *inserir_na_folha(No *folha, int chave, registro *ponteiro) {
  int i, ponto_insercao;

  ponto_insercao = 0;
  while (ponto_insercao < folha->n_chaves &&
         folha->chaves[ponto_insercao] < chave)
    ponto_insercao++;

  for (i = folha->n_chaves; i > ponto_insercao; i--) {
    folha->chaves[i] = folha->chaves[i - 1];
    folha->pont[i] = folha->pont[i - 1];
  }
  folha->chaves[ponto_insercao] = chave;
  folha->pont[ponto_insercao] = ponteiro;
  folha->n_chaves++;
  return folha;
}

No *inserir_na_folha_depois_do_corte(No *raiz, No *folha, int chave,
                                     registro *ponteiro) {
  No *nova_folha;
  int *temp_chaves;
  void **temp_ponteiros;
  int insertion_index, split, nova_chave, i, j;

  nova_folha = criar_folha();

  temp_chaves = malloc(ordem * sizeof(int));
  if (temp_chaves == NULL) {
    perror("temporaria chaves de array.");
    exit(EXIT_FAILURE);
  }

  temp_ponteiros = malloc(ordem * sizeof(void *));
  if (temp_ponteiros == NULL) {
    perror("Temporario array de ponteiro.");
    exit(EXIT_FAILURE);
  }

  insertion_index = 0;
  while (insertion_index < ordem - 1 && folha->chaves[insertion_index] < chave)
    insertion_index++;

  for (i = 0, j = 0; i < folha->n_chaves; i++, j++) {
    if (j == insertion_index)
      j++;
    temp_chaves[j] = folha->chaves[i];
    temp_ponteiros[j] = folha->pont[i];
  }

  temp_chaves[insertion_index] = chave;
  temp_ponteiros[insertion_index] = ponteiro;

  folha->n_chaves = 0;

  split = corte(ordem - 1);

  for (i = 0; i < split; i++) {
    folha->pont[i] = temp_ponteiros[i];
    folha->chaves[i] = temp_chaves[i];
    folha->n_chaves++;
  }

  for (i = split, j = 0; i < ordem; i++, j++) {
    nova_folha->pont[j] = temp_ponteiros[i];
    nova_folha->chaves[j] = temp_chaves[i];
    nova_folha->n_chaves++;
  }

  free(temp_ponteiros);
  free(temp_chaves);

  nova_folha->pont[ordem - 1] = folha->pont[ordem - 1];
  folha->pont[ordem - 1] = nova_folha;

  for (i = folha->n_chaves; i < ordem - 1; i++)
    folha->pont[i] = NULL;
  for (i = nova_folha->n_chaves; i < ordem - 1; i++)
    nova_folha->pont[i] = NULL;

  nova_folha->pai = folha->pai;
  nova_chave = nova_folha->chaves[0];

  return inserir_no_pai(raiz, folha, nova_chave, nova_folha);
}

No *inserir_no_No(No *raiz, No *n, int index_esquerda, int chave, No *direita) {
  int i;

  for (i = n->n_chaves; i > index_esquerda; i--) {
    n->pont[i + 1] = n->pont[i];
    n->chaves[i] = n->chaves[i - 1];
  }
  n->pont[index_esquerda + 1] = direita;
  n->chaves[index_esquerda] = chave;
  n->n_chaves++;
  return raiz;
}

No *inserir_no_No_depois_do_corte(No *raiz, No *no_antigo, int index_esquerda,
                                  int chave, No *direita) {
  int i, j, split, k_prime;
  No *novo_No, *filho;
  int *temp_chave;
  No **temp_pointers;

  temp_pointers = malloc((ordem + 1) * sizeof(No *));
  if (temp_pointers == NULL) {
    perror("Erro em array temporarios inserir_into_No_depois_do_corte.");
    exit(EXIT_FAILURE);
  }
  temp_chave = malloc(ordem * sizeof(int));
  if (temp_chave == NULL) {
    perror("Error em temporarias chaves inserir_into_No_depois_do_corte.");
    exit(EXIT_FAILURE);
  }

  for (i = 0, j = 0; i < no_antigo->n_chaves + 1; i++, j++) {
    if (j == index_esquerda + 1)
      j++;
    temp_pointers[j] = no_antigo->pont[i];
  }

  for (i = 0, j = 0; i < no_antigo->n_chaves; i++, j++) {
    if (j == index_esquerda)
      j++;
    temp_chave[j] = no_antigo->chaves[i];
  }

  temp_pointers[index_esquerda + 1] = direita;
  temp_chave[index_esquerda] = chave;

  split = corte(ordem);
  novo_No = criar_No();
  no_antigo->n_chaves = 0;
  for (i = 0; i < split - 1; i++) {
    no_antigo->pont[i] = temp_pointers[i];
    no_antigo->chaves[i] = temp_chave[i];
    no_antigo->n_chaves++;
  }
  no_antigo->pont[i] = temp_pointers[i];
  k_prime = temp_chave[split - 1];
  for (++i, j = 0; i < ordem; i++, j++) {
    novo_No->pont[j] = temp_pointers[i];
    novo_No->chaves[j] = temp_chave[i];
    novo_No->n_chaves++;
  }
  novo_No->pont[j] = temp_pointers[i];
  free(temp_pointers);
  free(temp_chave);
  novo_No->pai = no_antigo->pai;
  for (i = 0; i <= novo_No->n_chaves; i++) {
    filho = novo_No->pont[i];
    filho->pai = novo_No;
  }

  return inserir_no_pai(raiz, no_antigo, k_prime, novo_No);
}

No *inserir_no_pai(No *raiz, No *esquerda, int chave, No *direita) {
  int index_esquerda_;
  No *pai;

  pai = esquerda->pai;

  if (pai == NULL)
    return inserir_em_nova_raiz(esquerda, chave, direita);

  index_esquerda_ = index_esquerda(pai, esquerda);

  if (pai->n_chaves < ordem - 1)
    return inserir_no_No(raiz, pai, index_esquerda_, chave, direita);

  return inserir_no_No_depois_do_corte(raiz, pai, index_esquerda_, chave,
                                       direita);
}

No *inserir_em_nova_raiz(No *esquerda, int chave, No *direita) {
  No *raiz = criar_No();
  raiz->chaves[0] = chave;
  raiz->pont[0] = esquerda;
  raiz->pont[1] = direita;
  raiz->n_chaves++;
  raiz->pai = NULL;
  esquerda->pai = raiz;
  direita->pai = raiz;
  return raiz;
}

No *iniciar_nova_arvore(int chave, registro *ponteiro) {
  No *raiz = criar_folha();
  raiz->chaves[0] = chave;
  raiz->pont[0] = ponteiro;
  raiz->pont[ordem - 1] = NULL;
  raiz->pai = NULL;
  raiz->n_chaves++;
  return raiz;
}

No *inserir(No *raiz, int chave, int item) {
  registro *registro_pont = NULL;
  No *folha = NULL;

  registro_pont = encontrar(raiz, chave, false, NULL);
  if (registro_pont != NULL) {
    registro_pont->item = item;
    return raiz;
  }

  registro_pont = criar_registro(item);

  if (raiz == NULL)
    return iniciar_nova_arvore(chave, registro_pont);

  folha = encontrar_folha(raiz, chave, false);

  if (folha->n_chaves < ordem - 1) {
    folha = inserir_na_folha(folha, chave, registro_pont);
    return raiz;
  }

  return inserir_na_folha_depois_do_corte(raiz, folha, chave, registro_pont);
}

int pegar_index_vizinho(No *n) {
  int i;
  for (i = 0; i <= n->pai->n_chaves; i++)
    if (n->pai->pont[i] == n)
      return i - 1;

  printf("Procura de ponteiros nao existentes no pai.\n");
  printf("No:  %#lx\n", (unsigned long)n);
  exit(EXIT_FAILURE);
}

No *remove_entry_from_No(No *n, int chave, No *pointer) {
  int i, num_pointers;
  i = 0;
  while (n->chaves[i] != chave)
    i++;
  for (++i; i < n->n_chaves; i++)
    n->chaves[i - 1] = n->chaves[i];

  num_pointers = n->folha ? n->n_chaves : n->n_chaves + 1;
  i = 0;
  while (n->pont[i] != pointer)
    i++;
  for (++i; i < num_pointers; i++)
    n->pont[i - 1] = n->pont[i];

  n->n_chaves--;

  if (n->folha)
    for (i = n->n_chaves; i < ordem - 1; i++)
      n->pont[i] = NULL;
  else
    for (i = n->n_chaves + 1; i < ordem; i++)
      n->pont[i] = NULL;

  return n;
}

No *ajustar_raiz(No *raiz) {
  No *nova_raiz;

  if (raiz->n_chaves > 0)
    return raiz;

  if (!raiz->folha) {
    nova_raiz = raiz->pont[0];
    nova_raiz->pai = NULL;
  }

  else
    nova_raiz = NULL;

  free(raiz->chaves);
  free(raiz->pont);
  free(raiz);

  return nova_raiz;
}

No *juntar_Nos(No *raiz, No *n, No *vizinho, int index_vizinho, int k_prime) {
  int i, j, vizinho_index_insec, n_end;
  No *tmp;

  if (index_vizinho == -1) {
    tmp = n;
    n = vizinho;
    vizinho = tmp;
  }

  vizinho_index_insec = vizinho->n_chaves;

  if (!n->folha) {
    vizinho->chaves[vizinho_index_insec] = k_prime;
    vizinho->n_chaves++;

    n_end = n->n_chaves;

    for (i = vizinho_index_insec + 1, j = 0; j < n_end; i++, j++) {
      vizinho->chaves[i] = n->chaves[j];
      vizinho->pont[i] = n->pont[j];
      vizinho->n_chaves++;
      n->n_chaves--;
    }

    vizinho->pont[i] = n->pont[j];

    for (i = 0; i < vizinho->n_chaves + 1; i++) {
      tmp = (No *)vizinho->pont[i];
      tmp->pai = vizinho;
    }
  }

  else {
    for (i = vizinho_index_insec, j = 0; j < n->n_chaves; i++, j++) {
      vizinho->chaves[i] = n->chaves[j];
      vizinho->pont[i] = n->pont[j];
      vizinho->n_chaves++;
    }
    vizinho->pont[ordem - 1] = n->pont[ordem - 1];
  }

  raiz = deletar_entrada(raiz, n->pai, k_prime, n);
  free(n->chaves);
  free(n->pont);
  free(n);
  return raiz;
}

No *redistribuicao_Nos(No *raiz, No *n, No *vizinho, int vizinho_index,
                     int k_prime_index, int k_prime) {
  int i;
  No *tmp;

  if (vizinho_index != -1) {
    if (!n->folha)
      n->pont[n->n_chaves + 1] = n->pont[n->n_chaves];
    for (i = n->n_chaves; i > 0; i--) {
      n->chaves[i] = n->chaves[i - 1];
      n->pont[i] = n->pont[i - 1];
    }
    if (!n->folha) {
      n->pont[0] = vizinho->pont[vizinho->n_chaves];
      tmp = (No *)n->pont[0];
      tmp->pai = n;
      vizinho->pont[vizinho->n_chaves] = NULL;
      n->chaves[0] = k_prime;
      n->pai->chaves[k_prime_index] = vizinho->chaves[vizinho->n_chaves - 1];
    } else {
      n->pont[0] = vizinho->pont[vizinho->n_chaves - 1];
      vizinho->pont[vizinho->n_chaves - 1] = NULL;
      n->chaves[0] = vizinho->chaves[vizinho->n_chaves - 1];
      n->pai->chaves[k_prime_index] = n->chaves[0];
    }
  }

  else {
    if (n->folha) {
      n->chaves[n->n_chaves] = vizinho->chaves[0];
      n->pont[n->n_chaves] = vizinho->pont[0];
      n->pai->chaves[k_prime_index] = vizinho->chaves[1];
    } else {
      n->chaves[n->n_chaves] = k_prime;
      n->pont[n->n_chaves + 1] = vizinho->pont[0];
      tmp = (No *)n->pont[n->n_chaves + 1];
      tmp->pai = n;
      n->pai->chaves[k_prime_index] = vizinho->chaves[0];
    }
    for (i = 0; i < vizinho->n_chaves - 1; i++) {
      vizinho->chaves[i] = vizinho->chaves[i + 1];
      vizinho->pont[i] = vizinho->pont[i + 1];
    }
    if (!n->folha)
      vizinho->pont[i] = vizinho->pont[i + 1];
  }

  n->n_chaves++;
  vizinho->n_chaves--;

  return raiz;
}

No *deletar_entrada(No *raiz, No *n, int chave, void *pointer) {
  int min_chaves;
  No *vizinho;
  int vizinho_index;
  int k_prime_index, k_prime;
  int capacity;

  n = remove_entry_from_No(n, chave, pointer);

  if (n == raiz)
    return ajustar_raiz(raiz);

  min_chaves = n->folha ? corte(ordem - 1) : corte(ordem) - 1;

  if (n->n_chaves >= min_chaves)
    return raiz;

  vizinho_index = pegar_index_vizinho(n);
  k_prime_index = vizinho_index == -1 ? 0 : vizinho_index;
  k_prime = n->pai->chaves[k_prime_index];
  vizinho =
      vizinho_index == -1 ? n->pai->pont[1] : n->pai->pont[vizinho_index];

  capacity = n->folha ? ordem : ordem - 1;

  if (vizinho->n_chaves + n->n_chaves < capacity)
    return juntar_Nos(raiz, n, vizinho, vizinho_index, k_prime);
  else
    return redistribuicao_Nos(raiz, n, vizinho, vizinho_index, k_prime_index,
                            k_prime);
}

No *delete (No *raiz, int chave) {
  No *chave_folha = NULL;
  registro *chave_registro = NULL;

  chave_registro = encontrar(raiz, chave, false, &chave_folha);

  if (chave_registro != NULL && chave_folha != NULL) {
    raiz = deletar_entrada(raiz, chave_folha, chave, chave_registro);
    free(chave_registro);
  }
  return raiz;
}

void destroy_tree_Nos(No *raiz) {
  int i;
  if (raiz->folha)
    for (i = 0; i < raiz->n_chaves; i++)
      free(raiz->pont[i]);
  else
    for (i = 0; i < raiz->n_chaves + 1; i++)
      destroy_tree_Nos(raiz->pont[i]);
  free(raiz->pont);
  free(raiz->chaves);
  free(raiz);
}

No *destroy_tree(No *raiz) {
  destroy_tree_Nos(raiz);
  return NULL;
}

int main() {
  No *raiz;
  char instruction;

  raiz = NULL;

  raiz = inserir(raiz, 5, 33);
  raiz = inserir(raiz, 15, 21);
  raiz = inserir(raiz, 25, 31);
  raiz = inserir(raiz, 35, 41);
  raiz = inserir(raiz, 45, 10);

  print_arvore(raiz);

  raiz = delete (raiz, 5);
  printf("Depois de deletar o 5 \n");
  print_arvore(raiz);
}
