#include <stdlib.h>
#include "pqueue.h"

Bno *btree_initialize()
{
  int i = 2;

  // Aloca e inicializa a raiz
  Bno *raiz = malloc(sizeof(Bno));
  raiz->value = 4;
  raiz->esquerda = NULL;
  raiz->direita = NULL;

  while (i <= 6)
  {
    Bno *bnode = raiz;

    while (1)
    {
      if (i < bnode->value)
      {
        if (bnode->esquerda == NULL)
        {
          bnode->esquerda = malloc(sizeof(Bno));
          bnode->esquerda->value = i;
          bnode->esquerda->esquerda = NULL;
          bnode->esquerda->direita = NULL;
          break;
        }
        else
        {
          bnode = bnode->esquerda;
        }
      }
      else if (i > bnode->value)
      {
        if (bnode->direita == NULL)
        {
          bnode->direita = malloc(sizeof(Bno));
          bnode->direita->value = i;
          bnode->direita->esquerda = NULL;
          bnode->direita->direita = NULL;
          break;
        }
        else
        {
          bnode = bnode->direita;
        }
      }
      else
      {
        break; // valor já existe, ignora
      }
    }

    i++;
  }

  return raiz;
}
