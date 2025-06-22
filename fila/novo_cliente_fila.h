#ifndef TAD_ITEM_H
#define TAD_ITEM_H

#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    NENHUMA,
    BAIXA,
    MEDIA,
    ALTA
} PrioridadeItem;

typedef struct item
{
    int id;
    int tempo_processamento;
    PrioridadeItem prioridade;
    int finalizado;
    char especialidade[100];
} Item;

typedef struct
{
    char nome[100];
    int quantidade;
} RegistroEspecialidade;

RegistroEspecialidade especialidades[50]; // quantidade arbitrária
int total_especialidades = 0;

// Operações relacionadas ao arquivo .dat contendo os Items
FILE *arquivo_abrir();
void arquivo_fechar(FILE *arquivo);
void item_fechar(FILE *arquivo);

// Leitura/escrita de itens no arquivo
Item *item_inicializar();
void item_salvar(Item *item);
void item_ler(Item *item);
// void item_listar_todos(FILE *arquivo);  // opcional para debug

#endif
