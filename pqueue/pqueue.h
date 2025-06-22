#ifndef PQUEUE_H
#define PQUEUE_H

#include "../queue_list/queue.h"

typedef struct bno Bno;

typedef struct bno
{
  Bno *direita;
  Bno *esquerda;
  QueueManager *queueManager;
  int value;
} Bno;

Bno *btree_initialize();
Bno *procurar_na_btree(Bno *raiz, int prioridade);
int btree_vazia(Bno *raiz);
Item *peek_paciente_prioridade(Bno *raiz);
Bno *inserir_na_btree(Bno *raiz, Item *item);
Item *remover_da_btree(Bno **raiz_ptr);
Bno *remover_no_btree(Bno *no);
void btree_print(Bno *raiz);
void simular(QueueManager *fila_comum, Bno **raiz_ptr);
#endif