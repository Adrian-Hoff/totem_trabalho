#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "tad_configs.h"
#include "queue_list/queue.h"
#include "fila/novo_cliente_fila.h"
#include "pqueue/pqueue.h"

pthread_mutex_t mutex_fila = PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
    QueueManager *fila_comum;
    Bno *raiz_btree;
    int *ultimo_id;
} PollArgs;

void *poll_thread(void *arg)
{
    PollArgs *data = (PollArgs *)arg;

    while (1)
    {
        usleep(100 * 1000);
        Item temp_item;
        item_ler(&temp_item);

        if (temp_item.id != *(data->ultimo_id) && temp_item.id > -1)
        {
            Item *novo_item = malloc(sizeof(Item));
            if (!novo_item)
                continue;

            *novo_item = temp_item;

            pthread_mutex_lock(&mutex_fila);
            if (novo_item->prioridade == 6)
            {
                adicionar_fim(novo_item, data->fila_comum);
            }
            else
            {
                Bno *no_prioridade = procurar_na_btree(data->raiz_btree, novo_item->prioridade);
                if (!no_prioridade || !no_prioridade->queueManager)
                {
                    printf("Erro: prioridade %d não está na árvore ou fila não inicializada!\n", novo_item->prioridade);
                    free(novo_item);
                    pthread_mutex_unlock(&mutex_fila); // 👈 OBRIGATÓRIO AQUI
                    continue;
                }

                adicionar_fim(novo_item, no_prioridade->queueManager);
            }
            pthread_mutex_unlock(&mutex_fila);

            *(data->ultimo_id) = temp_item.id;
            printf("Novo paciente %d adicionado à fila (thread).\n", novo_item->id);
        }
    }

    return NULL;
}

void simular(QueueManager *fila_comum, Bno **raiz_ptr)
{
    pthread_mutex_lock(&mutex_fila);

    // 1) espiar o próximo de cada fila
    Item *prox_prio = peek_paciente_prioridade(*raiz_ptr);
    Item *prox_comum = fila_comum->primeiro
                           ? fila_comum->primeiro->item
                           : NULL;

    if (prox_prio)
    {
        printf("Próximo com prioridade: F%d\n", prox_prio->id);
    }
    else
    {
        printf("Próximo com prioridade: Fila de prioridade vazia.\n");
    }

    if (prox_comum)
    {
        printf("Próximo sem prioridade: F%d\n", prox_comum->id);
    }
    else
    {
        printf("Próximo sem prioridade: Fila comum vazia.\n");
    }

    // 2) decidir de onde remover
    Item *atender = NULL;
    if (!btree_vazia(*raiz_ptr))
    {
        atender = remover_da_btree(raiz_ptr);
    }
    else if (fila_comum->primeiro)
    {
        atender = fila_comum->primeiro->item;
        remover_inicio(fila_comum);
    }

    pthread_mutex_unlock(&mutex_fila);

    // 3) simular o atendimento
    if (atender)
    {
        printf("Simulando paciente %d por %d segundos...\n",
               atender->id, atender->tempo_processamento);
        sleep(atender->tempo_processamento);
        printf("Atendimento finalizado!\n\n");
    }
    else
    {
        printf("Nenhum paciente para simular.\n\n");
    }
}

int main()
{
    TadConfigs *tad_configs;
    Bno *raiz_btree = btree_initialize();
    QueueManager *queueManager;
    Item *item_inicial;

    // Criar TAD e abrir arquivo
    tad_configs = configs_inicializar();
    queueManager = inicializar_queue();
    item_inicial = item_inicializar();

    // if (!tad_configs) {
    //   printf("Erro ao criar TAD\n");
    //   return 1;
    // }
    printf("Arquivo acessado!");

    printf("O ponteiro queueManager aponta para o endereço: %p\n", (void *)queueManager);

    // Carregar configurações
    configs_ler(tad_configs);
    configs_mostrar(tad_configs);
    item_ler(item_inicial);

    int ultimo_id_lido = item_inicial->id;
    printf("\nultimo id lido: %d\n", ultimo_id_lido);

    //--------------

    PollArgs poll_args = {
        .fila_comum = queueManager,
        .raiz_btree = raiz_btree,
        .ultimo_id = &ultimo_id_lido};
    pthread_t thread;
    pthread_create(&thread, NULL, poll_thread, &poll_args);
    //------------

    while (tad_configs->configs.status != TERMINAR)
    {
        usleep(1000 * 1000);
        // arquivo_abrir();
        printf("\n======== ÁRVORE DE PRIORIDADE ========\n");
        btree_print(raiz_btree);
        printf("======================================\n\n");

        if (tad_configs->configs.status == SIMULAR)
        {
            simular(queueManager, &raiz_btree);
            // test(1, queueManager);
        }
        else
        {
            printf("Aguardando...\n");
        }

        // Item temp_item;
        // item_ler(&temp_item);

        // if (temp_item.id != ultimo_id_lido && temp_item.id > -1)
        // {
        //     Item *novo_item = malloc(sizeof(Item));
        //     if (!novo_item)
        //     {
        //         perror("Erro ao alocar novo Item");
        //         break;
        //     }

        //     *novo_item = temp_item; // Copia os dados
        //     adicionar_fim(novo_item, queueManager);
        //     ultimo_id_lido = temp_item.id;

        //     printf("Novo paciente %d adicionado à fila.\n", novo_item->id);
        // }

        configs_ler(tad_configs);
    }

    return 0;
}
