#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "tad_configs.h"
#include "queue_list/queue.h"
#include "fila/novo_cliente_fila.h"
pthread_mutex_t mutex_fila = PTHREAD_MUTEX_INITIALIZER;

void *poll_thread(void *arg)
{
    struct
    {
        QueueManager *queueManager;
        int *ultimo_id;
    } *data = arg;

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
            adicionar_fim(novo_item, data->queueManager);
            pthread_mutex_unlock(&mutex_fila);

            *(data->ultimo_id) = temp_item.id;
            printf("Novo paciente %d adicionado à fila (thread).\n", novo_item->id);
        }
    }

    return NULL;
}

void simular(QueueManager *queueManager)
{
    if (queueManager->primeiro == NULL)
    {
        printf("Não há pacientes na lista.\n");
        return;
    }

    Item *simular_item = queueManager->primeiro->item;

    if (simular_item->id > -1)
    {
        printf("Simulando paciente %d, %dsegs...\n", simular_item->id, simular_item->tempo_processamento);

        No *fno = queueManager->primeiro;
        printf("  fila: ");

        while (fno != NULL)
        {
            printf(" %d,", fno->item->id);
            fno = fno->proximo;
        }
        printf("\n");
        sleep(simular_item->tempo_processamento);

        pthread_mutex_lock(&mutex_fila);
        remover_inicio(queueManager);
        pthread_mutex_unlock(&mutex_fila);

        No *no = queueManager->primeiro;
        printf("  atendimento finalizado -> fila: ");

        while (no != NULL)
        {
            printf("%d, ", no->item->id);
            no = no->proximo;
        }

        printf("\n\n");
    }
    else
    {
        printf("não há pacientes na lista");
    }
}

int main()
{
    TadConfigs *tad_configs;
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
    struct
    {
        QueueManager *queue;
        int *ultimo_id;
    } poll_args = {queueManager, &ultimo_id_lido};

    pthread_t thread;
    pthread_create(&thread, NULL, poll_thread, &poll_args);
    //------------

    while (tad_configs->configs.status != TERMINAR)
    {
        usleep(1000 * 1000);
        // arquivo_abrir();
        if (tad_configs->configs.status == SIMULAR)
        {
            simular(queueManager);
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
