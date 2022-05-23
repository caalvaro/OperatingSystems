#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list/linked_list.h"

#define ESPERANDO 0

IO cria_IO(char tipo, int tempo_entrada, int tempo_execucao) {
    IO *io = (IO *) malloc(sizeof(IO));

    io->tipo = tipo;
    io->tempo_entrada = tempo_entrada;
    io->tempo_execucao = tempo_execucao;

    return *io;
}

Processo* cria_processo(int pid, int ppid, int tempo_chegada, int tempo_servico, IO *lista_io_processo) {
    Processo *processo = (Processo *) malloc(sizeof(Processo));
    
    processo->pid = pid;
    processo->ppid = ppid;
    processo->tempo_chegada = tempo_chegada;
    processo->tempo_servico = tempo_servico;
    processo->status = ESPERANDO;
    processo->lista_io_processo = lista_io_processo;

    return processo;
}

IO* cria_lista_io(int tamanho_lista, char *campo_io) {
    IO io, *lista_io = (IO *) calloc(tamanho_lista, sizeof(IO));
    char *IOs = strtok(campo_io, " ");
    int i = 0;

    while(IOs != NULL) {
        if (strcmp(IOs, "A")) {
            io = cria_IO(*IOs, atoi(IOs + 1), 2);
        }
        else if (strcmp(IOs, "B")) {
            io = cria_IO(*IOs, atoi(IOs + 1), 4);
        }
        else if (strcmp(IOs, "C")) {
            io = cria_IO(*IOs, atoi(IOs + 1), 8);
        }

        *(lista_io + i) = io;

        IOs += 2;
    }

    return lista_io;
}

LIST_HEAD *cria_lista_processos(char *nome_arquivo) {
    FILE* arquivo;
    char campo_io[1024], linha[1024];
    int pid, tempo_chegada, tempo_servico, tamanho_io;
    LIST_HEAD *lista_processos;
    int teste;

    arquivo = fopen(nome_arquivo, "r");
 
    if (arquivo == NULL) {
        printf("Erro: problema ao abrir o arquivo.\n");
    }

    lista_processos = create_head();
 
    while (1) {
        LIST_NODE *no_processo;
        Processo *processo;
        IO *lista_io;

        teste = fgets(linha, 1024, arquivo);
        linha[strcspn(linha, "\n")] = 0;

        char *tokens = strtok(linha, " ");

        printf("%s %s\n", tokens, tokens+1);

        pid = atoi(tokens);
        tempo_chegada = atoi(tokens+1);
        tempo_servico = atoi(tokens+2);
        tamanho_io = atoi(tokens+3);

        printf("lendo linha %d\n", teste);

        if (teste == 0) {
            break;
        }

        //lista_io = cria_lista_io(tamanho_io, campo_io);

        processo = cria_processo(pid, 0, tempo_chegada, tempo_servico, NULL);

        no_processo = create_node(processo);

        append_node(lista_processos, no_processo);
    }
 
    fclose(arquivo);

    return lista_processos;
}

int main(int argc, char const *argv[]) {
    LIST_HEAD *processos = cria_lista_processos("entrada.txt");
    LIST_NODE *percorre = processos->first_node;

    while (percorre != NULL) {
        printf("\nPid %d, chegada %d, serviço %d", 
            percorre->processo->pid,
            percorre->processo->tempo_chegada,
            percorre->processo->tempo_servico);

        percorre = percorre->next_node;
    }

    return 0;
}
