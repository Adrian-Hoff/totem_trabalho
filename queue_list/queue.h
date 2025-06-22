#ifndef QUEUE_H
#define QUEUE_H

#include "../fila/novo_cliente_fila.h"

typedef struct no No;

typedef struct no
{
  No *proximo;
  Item *item;
} No;

typedef struct queueManager
{
  No *primeiro;
  No *ultimo;
} QueueManager;

// lista encadeada
QueueManager *inicializar_queue();
No *adicionar_fim(Item *item, QueueManager *queueManager);
int remover_inicio(QueueManager *queueManager);

#endif