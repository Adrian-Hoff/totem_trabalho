#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

QueueManager *inicializar_queue()
{
  QueueManager *queueManager = malloc(sizeof(QueueManager));
  queueManager->primeiro = NULL;
  queueManager->ultimo = NULL;
  return queueManager;
}

No *adicionar_fim(Item *item, QueueManager *queueManager)
{
  No *novo = malloc(sizeof(No));
  novo->item = item;
  novo->proximo = NULL;

  if (queueManager->primeiro == NULL)
  {
    queueManager->primeiro = novo;
    queueManager->ultimo = novo;
  }
  else
  {
    queueManager->ultimo->proximo = novo;
    queueManager->ultimo = novo;
  }

  return novo;
}

int remover_inicio(QueueManager *queueManager)
{
  if (queueManager->primeiro != NULL)
  {
    No *heap_no = queueManager->primeiro;
    queueManager->primeiro = heap_no->proximo;

    // Liberar o item, se foi alocado com malloc
    if (heap_no->item != NULL)
    {
      free(heap_no->item);
    }

    free(heap_no);

    // Atualizar ponteiro 'ultimo' se a fila ficou vazia
    if (queueManager->primeiro == NULL)
    {
      queueManager->ultimo = NULL;
    }

    return 1; // ou heap_item->tempo_processamento
  }
  else
  {
    return -1;
  }
}
