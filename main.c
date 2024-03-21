// Deletion operation on a B+ Tree in C++

#include "Bmais.h"
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

//FUNÇÃO PARA IMPRIMIR O REGISTRO DE UMA CHAVE ESPECÍFICA
void encontrar_imprimir(No *const raiz, int chave, bool detalhe) {
  No *folha = NULL;
  registro *r = encontrar(raiz, chave, detalhe, NULL);
  if (r == NULL)
    printf("Não há registros para essa chave. %d.\n", chave);
  else
    printf("registro em %p -- chave %d, item %d.\n", r, chave, r->item);
  //se a chave for encontrada, o registro dessa chave será impresso, se não, uma mensagem é impressa dizendo que não há registro.
}

//FUNÇÃO PARA LOCALIZAR E IMPRIMIR PARES CHAVE-VALOR DENTRO DE UM INTERVALO ESPECIFICADO DA ÁRVORE.
void encontrar_imprimir_intervalo(No *const raiz, int chave_inicial,int chave_final, bool detalhe) {
  int i;
  int array_tamanho = chave_final - chave_inicial + 1;
  int chaves_retornadas[array_tamanho];//array pra armazenar as chaves
  void *ponteiros_retornados[array_tamanho];//array pra armazenar os endereços das chaves
  int num_encontrado =encontrar_distancia(raiz, chave_inicial, chave_final, detalhe,chaves_retornadas, ponteiros_retornados);//retorna o registro da chave(se achar), ou 0 (se não achar)
  if (!num_encontrado)//se falso (se 0)
    printf("Nada encontrado.\n");
  else {
    for (i = 0; i < num_encontrado; i++)
      printf("Chave: %d   Local: %p  Item: %d\n", chaves_retornadas[i],ponteiros_retornados[i],((registro *)ponteiros_retornados[i])->item);
    }
    //imprime registro por registro, caso "num_encontrado seja diferente de 0".
}

//FUNÇÃO PARA LOCALIZAR E RECUPERAR AS CHAVES E PONTEIROS ASSOCIADOS DENTRO DE UM INTERVALO ESPECIFICADO EM UMA ÁRVORE
int encontrar_distancia(No *const raiz, int chave_inicial, int chave_final,bool detalhe, int chaves_retornadas[],void *ponteiros_retornados[]) {
  int i, num_encontrado;
  num_encontrado = 0;

  No *n = encontrar_folha(raiz, chave_inicial, detalhe);

  if (n == NULL)
    return 0;

  for (i = 0; i < n->n_chaves && n->chaves[i] < chave_inicial; i++);
  //O laço percorre a árvore para encontrar o indice da "chave_inicial"

  if (i == n->n_chaves)
    return 0;
  while (n != NULL) {
    for (; i < n->n_chaves && n->chaves[i] <= chave_final; i++) {
      chaves_retornadas[num_encontrado] = n->chaves[i];
      ponteiros_retornados[num_encontrado] = n->pont[i];
      num_encontrado++;
      //Armazene as chaves e ponteiros associados encontrados no intervalo especificado nos respectivos arrays.
    }
    n = n->pont[ordem - 1];
    i = 0;
  }
  return num_encontrado;
  //retorna o número total de chaves encontradas dentro do intervalo especificado
}

//FUNÇÃO PARA LOCALIZAR UM NÓ FOLHA NA ÁRVORE
No *encontrar_folha(No *const raiz, int chave, bool detalhe) {

 /*
  a função leva três parametros: 
  "raiz": que é o nó raiz da arvore, 
  "chave": que é a chave a ser pesquisada e 
  "detalhe": que é uma flag booleana para determinar se informações detalhadas deve ser impressas.
  */
  if (raiz == NULL) {
    if (detalhe)
      printf("Arvore vazia.\n");
    return raiz;
  }
  /*
  condição que verifica se a árvore está vazia, se informações detalhadas forem solicitadas, ela imprime "Árvore vazia" e retorna a "raiz" igual a NULL.
  */

  int i = 0;
  No *c = raiz;
  /*
  2ª inicializa uma variável interia i com 0 e um ponteiro c para o nó raiz
  */

  while (!c->folha) {
    //laço para percorrer a árvore até chegar no nó da folha.

    if (detalhe) {
      printf("[");
      for (i = 0; i < c->n_chaves - 1; i++)
        printf("%d ", c->chaves[i]);
      printf("%d] ", c->chaves[i]);
    }
    //se informações detalhadas forem solicitadas, ele imprimirá as chaves dos Nós internos.

    i = 0;

    while (i < c->n_chaves) {
      if (chave >= c->chaves[i])
        i++;
      else
        break;
    }
    //laço que verifica cada chave no nó atual, para determinar o nó filho correto que deva seguir com base na chave de pesquisa.

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
  //Uma vez que um nó folha é alcançado, a função imprime as chaves no nó folha se informações detalhadas forem solicitadas.

  return c;
  //retorna o nó folha que pesquisado

}

//FUNÇÃO PARA ENCONTRAR UMA CHAVE NA ÁRVORE B+ E RETORNAR O REGISTRO CORRESPONDENTE.
registro *encontrar(No *raiz, int chave, bool detalhe, No **return_folha) {
  /*
  Parametros:
  raiz: ponteiro para o Nó raiz
  chave: valor da chave a ser pesquisado
  detalhe: sinalizador booleano que indica se é preciso mostrar informações detalhadas
  return_folha: Um ponteiro para um ponteiro para um tipo No, usado para retornar o Nó da folha onde a chave foi encontrada
  */

  if (raiz == NULL) {
    if (return_folha != NULL) {
      *return_folha = NULL;
    }
    return NULL;
  }
  // verifica se a "raiz" é "NULL", se Sim retorna "NULL"

  int i = 0;
  No *folha = NULL;

  folha = encontrar_folha(raiz, chave, detalhe);
  //chama a função "encontrar_folha" para buscar o No folha e guardar em "folha". Essa é uma função auxiliar que é assumida para localizar e retornar o nó da folha onde uma chave deve estar presente.

  for (i = 0; i < folha->n_chaves; i++)
    if (folha->chaves[i] == chave)
      break;
  //laço para percorrer o Nó folha buscando a chave exata, se for encontrada ele quebra e sai do laço
  if (return_folha != NULL) {
    *return_folha = folha;
  }
  if (i == folha->n_chaves)
    return NULL;
    //retorna "NULL" caso a chave não seja encontrada
  else
    return (registro *)folha->pont[i];
    //retorna o ponteiro do registro associado à chave

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
    perror("Error nas chaves - criar_No.");
    exit(EXIT_FAILURE);
  }
  novo_No->pont = malloc(ordem * sizeof(void *));
  if (novo_No->pont == NULL) {
    perror(" Error no array de nos de ponteiros - criar_No.");
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

No *inserir_na_folha(No *folha, int chave, registro *ponteiro_de_registro) {
  int i, ponto_insercao;

  ponto_insercao = 0;
  // Ponto de inserção menor que o número de chaves da folha ? 
  // A chave na da folha posição[0] é menor que a chave quer quer inserir ? 
  while (ponto_insercao < folha->n_chaves &&
         folha->chaves[ponto_insercao] < chave)
    ponto_insercao++;
  // Operação custosa de ordenação/translocação. 
  // i igual a num de chaves da folha, até i > ponto_inserção, decremente i.
  for (i = folha->n_chaves; i > ponto_insercao; i--) {
    folha->chaves[i] = folha->chaves[i - 1];
    folha->pont[i] = folha->pont[i - 1];
  }
  // Colocando a chave em sua posição correta.
  folha->chaves[ponto_insercao] = chave;
  // Colocando o registro no array de ponteiros.
  // A posição do array de ponteiros possui relação com a posição das chaves dentro das folhas.
  folha->pont[ponto_insercao] = ponteiro_de_registro;
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
  // Rotina
  temp_chaves = malloc(ordem * sizeof(int));
  if (temp_chaves == NULL) {
    perror("temporaria chaves de array.");
    exit(EXIT_FAILURE);
  }
  // Rotina
  temp_ponteiros = malloc(ordem * sizeof(void *));
  if (temp_ponteiros == NULL) {
    perror("Temporario array de ponteiro.");
    exit(EXIT_FAILURE);
  }

  insertion_index = 0;
  // index de inserção é menor que ordem - 1 = 2? 
  // A chave que está na folha em posição chave[insercao_index] é menor que a chave atual ?
  // Primeiro caso: folha->chaves[0] = 5
  // Segundo caso:  folha->chaves[1] = 15
  // chave = 25
  while (insertion_index < ordem - 1 && folha->chaves[insertion_index] < chave){
    insertion_index++;
  }
    
  // insertion_index = 2 passando do while.
  for (i = 0, j = 0; i < folha->n_chaves; i++, j++) {
    if (j == insertion_index){
      j++;
    }
    // temp_chaves[0] = folha->chaves[0];
    // temp_ponteiros[0] = folha->pont[0];
    // temp_chaves[1] = folha->chaves[1];
    // temp_ponteiros[1] = folha->pont[1];
    temp_chaves[j] = folha->chaves[i];
    temp_ponteiros[j] = folha->pont[i];
  }
  // temp_chaves[2] = 25
  // temp_ponteiros[2] = registro_ponteiro
  temp_chaves[insertion_index] = chave;
  temp_ponteiros[insertion_index] = ponteiro;

  folha->n_chaves = 0;
  // ordem 3 = split = 1
  split = corte(ordem - 1);

  for (i = 0; i < split; i++) {
    // folha->pont[0] = temp__ponteiros[0] // pont_para_reg1
    folha->pont[i] = temp_ponteiros[i];
    // folha->chaves[0] = temp_chaves[0]; // 5
    folha->chaves[i] = temp_chaves[i];
    folha->n_chaves++;
  }
  // folha->n_chaves = 1
  for (i = split, j = 0; i < ordem; i++, j++) {
    // nova_folha->pont[0] = temp_ponteiros[1]; // registro pont 1 
    // nova_folha->pont[1] = temp_ponteiros[2]; // registro ponteiro 2
    nova_folha->pont[j] = temp_ponteiros[i];
    // nova_folha->chaves[0] = temp_chaves[1]; // 15
    // nova_folha->chaves[1] = temp_chaves[2]; // 25
    
    nova_folha->chaves[j] = temp_chaves[i];
    nova_folha->n_chaves++; // 2

  }

  free(temp_ponteiros);
  free(temp_chaves);
  // nova_folha->pont[2] = folha->pont[2]; // null
  nova_folha->pont[ordem - 1] = folha->pont[ordem - 1];
  // folha->pont[2] = nova_folha;
  folha->pont[ordem - 1] = nova_folha; // folha->pont[2]-> nova_folha
  // Zerando os ponteiros da folha que não é folha mais
  for (i = folha->n_chaves; i < ordem - 1; i++) 
    // i = 1
    folha->pont[i] = NULL;
    // folha->pont[1] = NULL;
  for (i = nova_folha->n_chaves; i < ordem - 1; i++)
    // nova_folha->pont[1] = NULL;
    nova_folha->pont[i] = NULL;
  
  nova_folha->pai = folha->pai; // Nulo
  nova_chave = nova_folha->chaves[0];  // Nulo
  // folha = esquerda
  // nova_folha = direita

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

  pai = esquerda->pai; // Nulo
  // Caso do 25 cai aqui.
  if (pai == NULL)
    return inserir_em_nova_raiz(esquerda, chave, direita);

  index_esquerda_ = index_esquerda(pai, esquerda);

  if (pai->n_chaves < ordem - 1)
    return inserir_no_No(raiz, pai, index_esquerda_, chave, direita);

  return inserir_no_No_depois_do_corte(raiz, pai, index_esquerda_, chave,
                                       direita);
}
// Inserção em uma nova raiz, caso de balanceamento.
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
// Inicialização de uma árvore.
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
  // Se houver a chave dentro da árvore
  if (registro_pont != NULL) {
    registro_pont->item = item;
    return raiz;
  }
  // Função de alocação de espaço de memoria para registro.
  registro_pont = criar_registro(item);
  // Primeira entrada na árvore, nesse caso o número 5. 
  if (raiz == NULL)
    return iniciar_nova_arvore(chave, registro_pont);
 
  folha = encontrar_folha(raiz, chave, false);
   // Inseração da chave 15, segunda entrada, nesse caso.
  if (folha->n_chaves < ordem - 1) {
    folha = inserir_na_folha(folha, chave, registro_pont);
    return raiz;
  }
  // Inserção da chave 25, terceira entrada, nesse caso.
  return inserir_na_folha_depois_do_corte(raiz, folha, chave, registro_pont);
}

//FUNÇÃO PARA ENCONTRAR O INDEX DO NÓ VIZINHO
int pegar_index_vizinho(No *n) {
  int i; // Iteração sobre os ponteiros do nó pai
  for (i = 0; i <= n->pai->n_chaves; i++)
  //o laço percorre todos os ponteiros do nó pai
    if (n->pai->pont[i] == n)
      return i - 1;
    //se o ponteiro atual for igual ao nó fornecido, ele retornará o índice do vizinho

  printf("Procura de ponteiros nao existentes no pai.\n");
  printf("No:  %#lx\n", (unsigned long)n);
  exit(EXIT_FAILURE);
  //se o vizinho não for encontrado, uma mensagem de erro é impressa.
}

// Remove a chave do Nó.
No *remover_entrada_do_No(No *n, int chave, No *pointer) {
  int i, num_pointers;
  i = 0;
  while (n->chaves[i] != chave)
    i++;
    // esse laço "while" compara cada chave do "No" com a "chave" e incrementa +1 cada vez que não encontra. Até encontrar
  for (++i; i < n->n_chaves; i++)
    n->chaves[i - 1] = n->chaves[i];
    // esse laço "for" começa a partir da chave vizinha da chave encontrada, percorre o "No" a partir dai e move todos os elementos para a esquerda.

  num_pointers = n->folha ? n->n_chaves : n->n_chaves + 1;
  // "num_pointers" verifica se é um "No" folha ou não, para poder guardar o numero máximo de filhos.

  i = 0;

  while (n->pont[i] != pointer)
    i++;
  for (++i; i < num_pointers; i++)
    n->pont[i - 1] = n->pont[i];
  //laços para buscar o índice do ponteiro a ser removido, em seguida faz o deslocamento de todos os ponteiros para esquerda

  n->n_chaves--; // diminui o número de chaves do nó

  if (n->folha)
    for (i = n->n_chaves; i < ordem - 1; i++)
      n->pont[i] = NULL;
  else
    for (i = n->n_chaves + 1; i < ordem; i++)
      n->pont[i] = NULL;
  //condicional para manipular os ponteiros como NULL, em caso de nós folha ou nós internos

  return n;
  //retorna o nó modificado
}

//FUNÇÃO PARA AJUSTAR O NÓ RAIZ DE UMA ARVORE APÓS REMOÇÃO DE UMA ENTRADA
No *ajustar_raiz(No *raiz) {
  No *nova_raiz;

  if (raiz->n_chaves > 0)
    return raiz;
    // Se o número de chaves da raiz for maior que 0, ele retorna a raiz (não precisa ajustar)

  if (!raiz->folha) {
    nova_raiz = raiz->pont[0];
    nova_raiz->pai = NULL;
  }
  //se o nó raiz não for uma folha (tiver filhos), ele definirá o ponteiro para o primeiro filho do nó raiz e definirá o pai da nova raiz como "NULL"

  else
    nova_raiz = NULL;
  // se o nó raiz for um nó folha, ele definirá o ponteiro como "NULL"

  free(raiz->chaves);
  free(raiz->pont);
  free(raiz);
  //libera a memória alocada para as chaves, ponteiros e o próprio nó raiz antigo, usando o "free()""

  return nova_raiz;
  //retorna a nova raiz
}


// FUNÇÃO PARA CONCATENAR DOIS NÓS
No *juntar_Nos(No *raiz, No *n, No *vizinho, int index_vizinho, int k_prime) {
  int i, j, vizinho_index_insec, n_end;
  No *tmp;

  if (index_vizinho == -1) {
    tmp = n;
    n = vizinho;
    vizinho = tmp;
  }
  //verifica se o vizinho está a esquerda, se estiver troca o Nó atual pelo vizinho.

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
  //condição para Nós internos, move as chaves e ponteiros de "n" para "vizinho". Ajusta o número de chaves em "vizinho" e ajusta os ponteiros

  else {
    for (i = vizinho_index_insec, j = 0; j < n->n_chaves; i++, j++) {
      vizinho->chaves[i] = n->chaves[j];
      vizinho->pont[i] = n->pont[j];
      vizinho->n_chaves++;
    }
    vizinho->pont[ordem - 1] = n->pont[ordem - 1];
  }
  //Se os Nós forem Nós folha, ele move as chaves e ponteiros de "n" para "vizinho" e atualiza os ponteiros.

  raiz = remover_entrada(raiz, n->pai, k_prime, n);
  //chamada de função para apagar chave correspondente ao nó pai
  free(n->chaves);
  free(n->pont);
  free(n);
  return raiz;
  //passando free() para liberar a memória das chaves,ponteiros e retorna a raiz atualizada.
}

// FUNÇÃO PARA REDISTRIBUIR CHAVES ENTRE DOIS NÓS
No *redistribuicao_Nos(No *raiz, No *n, No *vizinho, int vizinho_index,int k_prime_index, int k_prime) {
  int i;
  No *tmp;

  if (vizinho_index != -1) {
    if (!n->folha)
      n->pont[n->n_chaves + 1] = n->pont[n->n_chaves];
    for (i = n->n_chaves; i > 0; i--) {
      n->chaves[i] = n->chaves[i - 1];
      n->pont[i] = n->pont[i - 1];
    }
    //se for true, ele manipula a redistribuição entre os nós
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
      //se "n" não for um nó folha, ele ajusta os ponteiros para manter a ordem dos nós filhos
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
      //se for um nó folha, ele ajusta as chaves e os ponteiros
      //atualiza as chaves pai com base na redistribuição 
  }

  n->n_chaves++;
  vizinho->n_chaves--;

  return raiz;
  //retorna a raiz atualizada
}

// FUNÇÃO PARA MANIPULAR O PROCESSO DE REMOÇÃO DA ÁRVORE B+ (removendo uma entrada (par chave-valor) de um nó.)
No *remover_entrada(No *raiz, No *n, int chave, void *pointer) {

  int min_chaves; //quantidade minima de chaves por No
  No *vizinho; // No adjacente
  int vizinho_index;
  int k_prime_index, k_prime;
  int capacidade;

  n = remover_entrada_do_No(n, chave, pointer);
  //função para remover a entrada do nó

  if (n == raiz)
    return ajustar_raiz(raiz);
  //verifica se n é o nó raiz, se sim ele ajusta a raiz

  min_chaves = n->folha ? corte(ordem - 1) : corte(ordem) - 1;
  //calculo o número mínimo de chaves necessárias em um nó com base em se ele é um nó folha ou um nó interno
  if (n->n_chaves >= min_chaves)
    return raiz;

  vizinho_index = pegar_index_vizinho(n);
  // guarda o index do nó vizinho
  k_prime_index = vizinho_index == -1 ? 0 : vizinho_index;
  
  k_prime = n->pai->chaves[k_prime_index];
  //acessa e guarda a chave no nó pai
  vizinho = vizinho_index == -1 ? n->pai->pont[1] : n->pai->pont[vizinho_index];

  capacidade = n->folha ? ordem : ordem - 1;

  if (vizinho->n_chaves + n->n_chaves < capacidade)
    // n_chaves: quantidade de chaves do nó.
    // se a soma das chaves adjacentes for menor que a capacidade
    
    return juntar_Nos(raiz, n, vizinho, vizinho_index, k_prime);
    // retorna a concatenação feita entre os dois nós adjacentes
  else
    return redistribuicao_Nos(raiz, n, vizinho, vizinho_index, k_prime_index, k_prime);
    // retorna a redistribuicao feita entre os dois nós adjacentes
}

//FUNÇÃO PARA REMOVER CHAVE DA ARVORE B+
No *remover(No *raiz, int chave) {
  No *chave_folha = NULL; 
  registro *chave_registro = NULL;

  // cria 2 novas varáveis, uma do tipo No e outra do tipo Registro, ambas iniciadas como NULL

  chave_registro = encontrar(raiz, chave, false, &chave_folha);

  // usa a função encontrar() para encontrar o endereço da chave na árvore e guarda em chave_registro
  
  if (chave_registro != NULL && chave_folha != NULL) {
    raiz = remover_entrada(raiz, chave_folha, chave, chave_registro);
    free(chave_registro);

    // se a chave for encontrada e a chave estiver na folha, a chave é apagada da raiz a partir da função remover_entrada(), após isso passa um free na chave para liberar o endereço de memória.
    
  }
  return raiz;
  // retorna a raiz, com a chave removida ou não.
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

  raiz = remover (raiz, 5);
  printf("Depois de deletar o 5 \n");
  print_arvore(raiz);
}
