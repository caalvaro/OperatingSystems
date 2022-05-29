#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list/linked_list.h"

#define ESPERANDO 0
#define TEMPO_A 2
#define TEMPO_B 4
#define TEMPO_C 8

IO *cria_IO(char tipo, int tempo_entrada)
{
    IO *io = (IO *)malloc(sizeof(IO));

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

    processo->pid = pid;
    processo->ppid = ppid;
    processo->tempo_chegada = tempo_chegada;
    processo->tempo_servico = tempo_servico;
    processo->tempo_executado = 0;
    processo->tempo_ciclo_atual = 0;
    processo->status = ESPERANDO;
    processo->lista_io= lista_io;

    if (processo->lista_io != NULL) {
        LIST_NODE *percorre_ios = processo->lista_io->first_node;
        
        // verifica se um processo retornou do IO
        while (percorre_ios != NULL) {
            IO *io = (IO *) percorre_ios->elemento;
            
            io->processo = processo;

            percorre_ios = percorre_ios->next_node;
        }
    }

    return processo;
}

// IO *cria_lista_io(int tamanho_lista, char *campo_io)
// {
//     IO *io, *lista_io = (IO *)calloc(tamanho_lista, sizeof(IO));
//     char *IOs = strtok(campo_io, " ");
//     int i = 0;

//     // while (IOs != NULL)
//     // {
//     //     if (strcmp(IOs, "A"))
//     //     {
//     //         io = cria_IO(*IOs, atoi(IOs + 1), 2);
//     //     }
//     //     else if (strcmp(IOs, "B"))
//     //     {
//     //         io = cria_IO(*IOs, atoi(IOs + 1), 4);
//     //     }
//     //     else if (strcmp(IOs, "C"))
//     //     {
//     //         io = cria_IO(*IOs, atoi(IOs + 1), 8);
//     //     }

//     //     // *(lista_io + i) = io;

//     //     IOs += 2;
//     // }

//     return lista_io;
// }

// LIST_HEAD *cria_lista_processos(char *nome_arquivo)
// {
//     FILE *arquivo;
//     char campo_io[1024], linha[1024];
//     int pid, tempo_chegada, tempo_servico, tamanho_io;
//     LIST_HEAD *lista_processos;
//     int teste;

//     arquivo = fopen(nome_arquivo, "r");

//     if (arquivo == NULL)
//     {
//         printf("Erro: problema ao abrir o arquivo.\n");
//     }

//     lista_processos = create_head();

//     while (1)
//     {
//         LIST_NODE *no_processo;
//         Processo *processo;
//         IO *lista_io;

//         teste = fgets(linha, 1024, arquivo);
//         linha[strcspn(linha, "\n")] = 0;

//         char *tokens = strtok(linha, " ");

//         printf("%s %s\n", tokens, tokens + 1);

//         pid = atoi(tokens);
//         tempo_chegada = atoi(tokens + 1);
//         tempo_servico = atoi(tokens + 2);
//         tamanho_io = atoi(tokens + 3);

//         printf("lendo linha %d\n", teste);

//         if (teste == 0)
//         {
//             break;
//         }

//         // lista_io = cria_lista_io(tamanho_io, campo_io);

//         processo = cria_processo(pid, 0, tempo_chegada, tempo_servico, NULL);

//         no_processo = create_node(processo);

//         append_node(lista_processos, no_processo);
//     }

//     fclose(arquivo);

//     return lista_processos;
// }

int main(int argc, char const *argv[])
{
    printf("0.\n");
    LIST_HEAD *processos = create_head();
    printf("1.\n");
    IO *io_1_p1 = cria_IO('A', 3);
    printf("2.\n");
    IO *io_2_p1 = cria_IO('C', 8);
    printf("3.\n");
    IO *io_1_p2 = cria_IO('A', 2);
    printf("4.\n");
    IO *io_2_p2 = cria_IO('B', 4);
    printf("5.\n");
    IO *io_3_p2 = cria_IO('C', 7);
    printf("6.\n");
    IO *io_1_p5 = cria_IO('B', 5);
    printf("7.\n");

    LIST_HEAD *io_p1 = create_head();
    printf("8.\n");
    enqueue(io_p1, io_1_p1);
    printf("9.\n");
    enqueue(io_p1, io_2_p1);
    printf("10.\n");

    Processo *processo1 = cria_processo(1, 0, 0, 10, io_p1);
    printf("11.\n");
    enqueue(processos, processo1);

    printf("Criou processo 1.\n");

    LIST_HEAD *io_p2 = create_head();
    enqueue(io_p2, io_1_p2);
    enqueue(io_p2, io_2_p2);
    enqueue(io_p2, io_3_p2);

    Processo *processo2 = cria_processo(2, 0, 5, 16, io_p2);
    enqueue(processos, processo2);

    printf("Criou processo 2.\n");

    LIST_HEAD *io_p5 = create_head();
    enqueue(io_p5, io_1_p5);

    Processo *processo5 = cria_processo(5, 0, 11, 8, io_p5);
    enqueue(processos, processo5);
    
    printf("Criou processo 5.\n");

    Processo *processo3 = cria_processo(3, 0, 7, 11, NULL);
    enqueue(processos, processo3);

    printf("Criou processo 3.\n");

    Processo *processo4 = cria_processo(4, 0, 7, 5, NULL);
    enqueue(processos, processo4);

    printf("Criou processo 4.\n");

    LIST_HEAD *filaAltaPrioridade = create_head();
    LIST_HEAD *filaBaixaPrioridade = create_head();
    LIST_HEAD *filaIO = create_head();

    printf("Criou filas.\n");
    
    int processosTerminados = 0;
    int tempo = 0;
    int fatia_tempo = 4;
    Processo *processoExecutando = NULL;

    printf("Criou processos.\n");

    // cada iteração é uma unidade de tempo
    while (processosTerminados < processos->list_size) {
        LIST_NODE *percorre_processos = processos->first_node;

        // verifica se um novo processo chegou
        while (percorre_processos != NULL) {
            Processo *processo = (Processo *) percorre_processos->elemento;
            if (processo->tempo_chegada == tempo) {
                printf("Tempo %d: processo %d chegou.\n", tempo, processo->pid);
                enqueue(filaAltaPrioridade, processo);
            }

            percorre_processos = percorre_processos->next_node;
        }

        //printf("Verificou processos.\n");

        
        LIST_NODE *percorre_ios = filaIO->first_node;
        
        // verifica se um processo retornou do IO
        while (percorre_ios != NULL) {
            IO *io = (IO *) percorre_ios->elemento;
            if (io->tempo_saida == tempo) {
                printf("Tempo %d: processo %d ficou pronto do IO %c.\n", tempo, io->processo->pid, io->tipo);
                
                if (io->tipo == 'B' || io->tipo == 'C') {
                    enqueue(filaAltaPrioridade, io->processo);
                } else {
                    enqueue(filaBaixaPrioridade, io->processo);
                }
            }

            percorre_ios = percorre_ios->next_node;
        }

        //printf("Verificou retorno io.");


        // critérios de parada
        if (processoExecutando != NULL) {
            // verifica quantos processos já terminaram
            if (processoExecutando->tempo_executado == processoExecutando->tempo_servico) {
                printf("Tempo %d: processo %d terminou.\n", tempo, processoExecutando->pid);
                processosTerminados += 1;
                processoExecutando = NULL;
            }
            // verificando preempção
            else if (processoExecutando->tempo_ciclo_atual == fatia_tempo) {
                printf("Tempo %d: processo %d parou a execução -> preempção.\n", tempo, processoExecutando->pid);
                enqueue(filaBaixaPrioridade, processoExecutando);
                processoExecutando->tempo_ciclo_atual = 0;
                processoExecutando = NULL;
            }
            // verifica solicitação de IO
            else {
                if (processoExecutando->lista_io != NULL) {
                    percorre_ios = processoExecutando->lista_io->first_node;
                    
                    while (percorre_ios != NULL) {
                        IO *io = (IO *) percorre_ios->elemento;

                        if (io->tempo_entrada == processoExecutando->tempo_executado) {
                            printf("Tempo %d: processo %d parou a execução -> IO %c.\n", tempo, io->processo->pid, io->tipo);

                            io->tempo_saida = tempo + io->tempo_execucao;
                            enqueue(filaIO, io);
                            
                            processoExecutando->tempo_ciclo_atual = 0;
                            processoExecutando = NULL;
                            break;
                        }

                        percorre_ios = percorre_ios->next_node;
                    }
                }
            }
        }

        //printf("Verificou parada.\n");


        // executa um processo se não tiver executando
        if (processoExecutando == NULL) {
            if (filaAltaPrioridade->list_size != 0) {
                processoExecutando = dequeue(filaAltaPrioridade)->elemento;
            }
            else if (filaBaixaPrioridade->list_size != 0) {
                processoExecutando = dequeue(filaBaixaPrioridade)->elemento;
            }
        }

        //printf("testou se não está executando nada.\n");

        // executa o processo
        if (processoExecutando != NULL) {
            printf("Tempo %d: processo %d executando.\n", tempo, processoExecutando->pid);
            processoExecutando->tempo_executado += 1;
            processoExecutando->tempo_ciclo_atual += 1;
        }

        //printf("executou processos.\n");

        tempo += 1;
    }
    return 0;
}
