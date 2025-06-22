#include <stdlib.h>
#include <stdio.h>
#include "pqueue.h"

// Helper: cria um nó pronto para uso
static Bno *criar_no(int prioridade)
{
  Bno *no = malloc(sizeof(Bno));
  if (!no)
    return NULL;
  no->value = prioridade;
  no->esquerda = NULL;
  no->direita = NULL;
  no->queueManager = inicializar_queue();
  return no;
}

// Inicializa a BST estática com prioridades de 1 a 5
Bno *btree_initialize(void)
{
  Bno *n1 = criar_no(1);
  Bno *n2 = criar_no(2);
  Bno *n3 = criar_no(3);
  Bno *n4 = criar_no(4);
  Bno *n5 = criar_no(5);
  if (!n1 || !n2 || !n3 || !n4 || !n5)
    return NULL;

  // Monta uma BST balanceada simples:
  //        (3)
  //       /   \
    //     (2)   (5)
  //     /     /
  //   (1)   (4)
  n3->esquerda = n2;
  n3->direita = n5;
  n2->esquerda = n1;
  n5->esquerda = n4;

  return n3;
}

// Busca nó com prioridade específica
Bno *procurar_na_btree(Bno *raiz, int prioridade)
{
  if (!raiz)
    return NULL;
  if (prioridade < raiz->value)
    return procurar_na_btree(raiz->esquerda, prioridade);
  if (prioridade > raiz->value)
    return procurar_na_btree(raiz->direita, prioridade);
  return raiz;
}

// Verifica se a árvore está vazia
int btree_vazia(Bno *raiz)
{
  return raiz == NULL;
}

// Espia o próximo paciente da fila de maior prioridade
Item *peek_paciente_prioridade(Bno *raiz)
{
  if (!raiz)
    return NULL;
  while (raiz->esquerda)
    raiz = raiz->esquerda;
  if (raiz->queueManager && raiz->queueManager->primeiro)
    return raiz->queueManager->primeiro->item;
  return NULL;
}

// Insere um item na BST
Bno *inserir_na_btree(Bno *raiz, Item *item)
{
  if (item->prioridade < 1 || item->prioridade > 5)
    return raiz;
  if (!raiz)
  {
    Bno *no = criar_no(item->prioridade);
    adicionar_fim(item, no->queueManager);
    return no;
  }
  if (item->prioridade < raiz->value)
    raiz->esquerda = inserir_na_btree(raiz->esquerda, item);
  else if (item->prioridade > raiz->value)
    raiz->direita = inserir_na_btree(raiz->direita, item);
  else
    adicionar_fim(item, raiz->queueManager);
  return raiz;
}

// Remove nó da BST retornando subárvore substituta
Bno *remover_no_btree(Bno *no)
{
  if (!no->esquerda)
    return no->direita;
  if (!no->direita)
    return no->esquerda;
  Bno *pai_suc = no;
  Bno *suc = no->direita;
  while (suc->esquerda)
  {
    pai_suc = suc;
    suc = suc->esquerda;
  }
  if (pai_suc != no)
  {
    pai_suc->esquerda = suc->direita;
    suc->direita = no->direita;
  }
  suc->esquerda = no->esquerda;
  return suc;
}

// Remove o próximo paciente da menor prioridade e ajusta a árvore
Item *remover_da_btree(Bno **raiz_ptr)
{
  if (!*raiz_ptr)
    return NULL;
  Bno *pai = NULL;
  Bno *atual = *raiz_ptr;
  // Encontra nó de menor prioridade
  while (atual->esquerda)
  {
    pai = atual;
    atual = atual->esquerda;
  }
  // Remove da fila interna, mas não libera o Item
  if (atual->queueManager && atual->queueManager->primeiro)
  {
    No *fila_no = atual->queueManager->primeiro;
    Item *item_removido = fila_no->item;
    // Ajusta cabeça da fila interna
    atual->queueManager->primeiro = fila_no->proximo;
    if (!atual->queueManager->primeiro)
      atual->queueManager->ultimo = NULL;
    free(fila_no);
    // Se fila interna vazia, remove o nó da árvore
    if (!atual->queueManager->primeiro)
    {
      Bno *sub = remover_no_btree(atual);
      if (pai)
      {
        if (pai->esquerda == atual)
          pai->esquerda = sub;
        else
          pai->direita = sub;
      }
      else
      {
        *raiz_ptr = sub;
      }
      free(atual->queueManager);
      free(atual);
    }
    return item_removido;
  }
  return NULL;
}

// Imprime uma fila encadeada
static void imprimir_fila(QueueManager *fila)
{
  No *no = fila->primeiro;
  if (!no)
  {
    printf("  [Fila] -> (vazia)\n");
    return;
  }
  printf("  [Fila] -> ");
  while (no)
  {
    printf("F%d", no->item->id);
    if (no->proximo)
      printf(" -> ");
    no = no->proximo;
  }
  printf("\n");
}

// Impressão em ordem da árvore e filas internas
void btree_print(Bno *raiz)
{
  if (!raiz)
    return;
  btree_print(raiz->esquerda);
  printf("Prioridade %d:\n", raiz->value);
  imprimir_fila(raiz->queueManager);
  printf("\n");
  btree_print(raiz->direita);
}
