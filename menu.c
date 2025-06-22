#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "tad_configs.h"
#include "fila/novo_cliente_fila.h"
#include "queue_list/queue.h"
// main.c

int menu()
{
    printf("1. Aguardar\n2. Simular\n3. Terminar\n4. Ler\n5. Adicionar\n0. Sair\n");
    printf("Informe a opção desejada: ");
    int aux;
    scanf("%d", &aux);
    return aux;
}
int main()
{
    TadConfigs *tad_configs;
    Item *novo;

    int op;
    // Criar TAD e abrir arquivo
    novo = item_inicializar();
    tad_configs = configs_inicializar();
    if (!tad_configs)
    {
        printf("Erro ao criar TAD\n");
        return 1;
    }
    do
    {
        op = menu();
        switch (op)
        {
        case 1:
        {
            configs_atualizar(tad_configs, AGUARDAR, 1);
            break;
        }
        case 2:
        {
            configs_atualizar(tad_configs, SIMULAR, 1);
            break;
        }
        case 3:
        {
            configs_atualizar(tad_configs, TERMINAR, 1);
            break;
        }
        case 4:
        {
            // Carregar configurações
            Item *temp = malloc(sizeof(Item));
            configs_ler(tad_configs);
            configs_mostrar(tad_configs);

            item_ler(temp);
            printf(" ");
            printf("\npaciente: {");
            printf("\n  id: %d", temp->id);
            printf("\n  prioridade: %d", temp->prioridade);
            printf("\n  tempo_processamento: %d", temp->tempo_processamento);
            printf("\n  finalizado: %d", temp->finalizado);
            printf("\n}\n");
            printf("\n");
            free(temp);
            // Exibir configurações (carrega do arquivo existente se existir)
            break;
        }
        case 5:
        {
            srand(time(NULL)); // Inicializa a semente aleatória com base no tempo
            int prioridade;
            printf("Informe a prioridade: ");
            scanf("%d", &prioridade);
            novo->id = novo->id + 1;
            novo->tempo_processamento = (rand() % (10 - 5 + 1)) + 5;
            novo->prioridade = prioridade;
            novo->finalizado = 0;

            item_salvar(novo);

            printf("id: %d\n", novo->id);
            printf("prioridade: %d\n", novo->prioridade);

            break;
        }
        case 0:
        {
            configs_destruir(tad_configs);
            printf("Até a próxima!\n");
            break;
        }
        default:
        {
            printf("Opção inválida!\n");
        }
        }
    } while (op != 0);
    return 0;
}
