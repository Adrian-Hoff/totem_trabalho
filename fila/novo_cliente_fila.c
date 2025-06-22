#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "novo_cliente_fila.h"

#define NOVO_CLIENTE_FILA_NAO_PRIORITARIA "./novo_cliente_fila.dat"

Item *item_inicializar()
{ // const char *nome_arquivo
  Item *item = malloc(sizeof(Item));
  if (!item)
  {
    return NULL;
  }
  FILE *arquivo = arquivo_abrir();
  if (arquivo)
  {
    // lê os dados
    item_ler(item);
    item->id = -1;
    arquivo_fechar(arquivo);
  }
  else
  {
    return NULL;
  }

  return item;
}

FILE *arquivo_abrir()
{
  FILE *arquivo;
  if (access(NOVO_CLIENTE_FILA_NAO_PRIORITARIA, F_OK) != -1)
  {                                                            // arquivo já existe
    arquivo = fopen(NOVO_CLIENTE_FILA_NAO_PRIORITARIA, "rb+"); // apenas abre o arquivo
  }
  else
  {
    arquivo = fopen(NOVO_CLIENTE_FILA_NAO_PRIORITARIA, "wb+"); // cria arquivo novo
    Item *dat_item = malloc(sizeof(Item));
    dat_item->id = -1;
    if (dat_item && arquivo)
    {
      fwrite(dat_item, sizeof(Item), 1, arquivo);
    }
  }
  return arquivo;
}

void arquivo_fechar(FILE *arquivo)
{
  fclose(arquivo);
}

void item_ler(Item *dat_item)
{
  FILE *arquivo = arquivo_abrir();
  if (dat_item && arquivo)
  {
    fread(dat_item, sizeof(Item), 1, arquivo);
  }
  arquivo_fechar(arquivo);
}

void item_fechar(FILE *arquivo)
{
  fclose(arquivo);
}

void item_salvar(Item *item)
{
  FILE *arquivo = arquivo_abrir();
  if (item && arquivo)
  {
    fwrite(item, sizeof(Item), 1, arquivo);
  }
  item_fechar(arquivo);
}
