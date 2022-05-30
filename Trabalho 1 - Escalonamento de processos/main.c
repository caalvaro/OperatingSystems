#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "linked_list/linked_list.h"

#define TEMPO_A 2
#define TEMPO_B 4
#define TEMPO_C 8

IO *cria_IO(char tipo, int tempo_entrada)
{
    IO *io = (IO *) malloc(sizeof(IO));

    io->tipo = tipo;
    io->tempo_entrada = tempo_entrada;
    io->tempo_saida = -1;

    if (tipo == 'A')
    {
        io->tempo_execucao = TEMPO_A;
    }
    else if (tipo == 'B')
    {
        io->tempo_execucao = TEMPO_B;
    }
    else if (tipo == 'C')
    {
        io->tempo_execucao = TEMPO_C;
    }

    return io;
}

Processo *cria_processo(int pid, int ppid, int tempo_chegada, int tempo_servico, LIST_HEAD *lista_io)
{
    Processo *processo = (Processo *)malloc(sizeof(Processo));
    LIST_NODE *percorre_ios;

    processo->pid = pid;
    processo->ppid = ppid;
    processo->tempo_chegada = tempo_chegada;
    processo->tempo_servico = tempo_servico;
    processo->tempo_executado = 0;
    processo->tempo_ciclo_atual = 0;
    processo->lista_io= lista_io;

    percorre_ios = processo->lista_io->first_node;
    
    while (percorre_ios != NULL) {
        IO *io = (IO *) percorre_ios->elemento;
        
        io->processo = processo;

        percorre_ios = percorre_ios->next_node;
    }

    return processo;
}

LIST_HEAD *cria_lista_processos()
{
    FILE *arquivo;
    char linha[1024];
    char *token;
    int pid, ppid, tempo_chegada, tempo_servico, tempo_entrada_io;
    char tipo_io;
    LIST_HEAD *processos = create_head();

    arquivo = fopen("entrada.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro: problema ao abrir o arquivo.\n");
    }

    /* percorre cada linha do arquivo */
    while (1)
    {
        LIST_HEAD *lista_io = create_head();
        char *teste = fgets(linha, 1024, arquivo);
        char *separador = ";";

        if (teste == NULL) break;

        /* le os 4 dados do processo */
        token = strtok(linha, separador);
        pid = atoi(token);

        token = strtok(NULL, separador);
        ppid = atoi(token);

        token = strtok(NULL, separador);
        tempo_chegada = atoi(token);

        token = strtok(NULL, separador);
        tempo_servico = atoi(token);

        /* lê os dados do IO do processo */
        token = strtok(NULL, separador);
        while (token != NULL && *token != '\n')
        {   
            tipo_io = token[0];

            token = strtok(NULL, separador);
            tempo_entrada_io = atoi(token);

            /* adiciona o io na lista de io do processo */
            enqueue(lista_io, cria_IO(tipo_io, tempo_entrada_io));

            token = strtok(NULL, separador);
        }

        /* cria o processo e adiciona numa lista com todos os processos */
        enqueue(processos, cria_processo(pid, ppid, tempo_chegada, tempo_servico, lista_io));
    }
    
    return processos;
}

int main(int argc, char const *argv[])
{
    LIST_HEAD *processos = cria_lista_processos();
    LIST_HEAD *fila_alta_prioridade = create_head();
    LIST_HEAD *fila_baixa_prioridade = create_head();
    LIST_HEAD *fila_io = create_head();
    LIST_NODE *percorre_ios; /*nó usado para percorrer a lista encadeada de IOs*/
    int processos_terminados = 0;
    int tempo = 0;
    int fatia_tempo = 4;
    Processo *processo_executando = NULL;

    /* cada iteração é uma unidade de tempo
        a cada unidade de tempo, faz todas as verificações para atualizar as filas e executar os processos */
    while (processos_terminados < processos->list_size) {
        LIST_NODE *percorre_processos = processos->first_node;

        /* verifica se um novo processo chegou */
        /* percorre a lista de processos */
        while (percorre_processos != NULL) {
            Processo *processo = (Processo *) percorre_processos->elemento;

            /* se chegou no tempo atual, adiciona na fila de alta prioridade */
            if (processo->tempo_chegada == tempo) {
                printf("Tempo %d: processo %d chegou.\n", tempo, processo->pid);
                enqueue(fila_alta_prioridade, processo);
            }

            percorre_processos = percorre_processos->next_node;
        }

        percorre_ios = fila_io->first_node;
        
        /* verifica se um processo retornou do IO */
        /* percorre a fila de IO */
        while (percorre_ios != NULL) {
            IO *io = (IO *) percorre_ios->elemento;

            /* se o tempo atual é o tempo de saída, coloca o processo em uma das filas */
            if (io->tempo_saida == tempo) {
                printf("Tempo %d: processo %d ficou pronto do IO %c.\n", tempo, io->processo->pid, io->tipo);
                
                if (io->tipo == 'B' || io->tipo == 'C') {
                    enqueue(fila_alta_prioridade, io->processo);
                } else {
                    enqueue(fila_baixa_prioridade, io->processo);
                }
            }

            percorre_ios = percorre_ios->next_node;
        }

        /* critérios de parada do processo */
        if (processo_executando != NULL) {
            /* verifica se o processo terminou sua execução */
            if (processo_executando->tempo_executado == processo_executando->tempo_servico) {
                printf("Tempo %d: processo %d terminou.\n", tempo, processo_executando->pid);
                processos_terminados += 1;
                processo_executando = NULL;
            }
            /* verifica se o processo deve fazer preempção agora */
            else if (processo_executando->tempo_ciclo_atual == fatia_tempo) {
                printf("Tempo %d: processo %d parou a execução -> preempção.\n", tempo, processo_executando->pid);
                enqueue(fila_baixa_prioridade, processo_executando);
                processo_executando->tempo_ciclo_atual = 0;
                processo_executando = NULL;
            }
            /* verifica se o processo vai solicitar IO agora*/
            else {
                percorre_ios = processo_executando->lista_io->first_node;
                
                /* percorre a lista de io do processo */
                while (percorre_ios != NULL) {
                    IO *io = (IO *) percorre_ios->elemento;

                    if (io->tempo_entrada == processo_executando->tempo_executado) {
                        printf("Tempo %d: processo %d parou a execução -> IO %c.\n", tempo, io->processo->pid, io->tipo);

                        io->tempo_saida = tempo + io->tempo_execucao;
                        enqueue(fila_io, io);
                        
                        processo_executando->tempo_ciclo_atual = 0;
                        processo_executando = NULL;
                        break;
                    }

                    percorre_ios = percorre_ios->next_node;
                }
            }
        }

        /* pega um processo da fila se não tiver nenhum executando, não faz nada se as filas estiverem vazias */
        if (processo_executando == NULL) {
            if (fila_alta_prioridade->list_size != 0) {
                processo_executando = dequeue(fila_alta_prioridade)->elemento;
            }
            else if (fila_baixa_prioridade->list_size != 0) {
                processo_executando = dequeue(fila_baixa_prioridade)->elemento;
            }
        }

        /* abstração da execução do processo (atualiza os tempos) */
        if (processo_executando != NULL) {
            printf("Tempo %d: processo %d executando.\n", tempo, processo_executando->pid);
            processo_executando->tempo_executado += 1;
            processo_executando->tempo_ciclo_atual += 1;
        }

        tempo += 1;

        sleep(1);
    }
    return 0;
}
