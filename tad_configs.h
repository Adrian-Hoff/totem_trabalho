#ifndef TAD_CONFIGS_H
#define TAD_CONFIGS_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    AGUARDAR,
    SIMULAR,
    TERMINAR
} statusProcessamento;

typedef struct conf {
    statusProcessamento status;
    int intervalo;
} Configs;

typedef struct tad_configs {
  FILE *arquivo;
  Configs configs;
} TadConfigs;


FILE *configs_abrir();
TadConfigs *configs_inicializar();
void configs_fechar(FILE *arquivo);
void configs_destruir(TadConfigs *tad);
void configs_salvar(TadConfigs *tad);
void configs_ler(TadConfigs *tad);
void configs_mostrar(TadConfigs *tad);
void configs_atualizar(TadConfigs *tad, statusProcessamento status, int intervalo);

#endif

