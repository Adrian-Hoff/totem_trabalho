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

void btree_initialize();

#endif