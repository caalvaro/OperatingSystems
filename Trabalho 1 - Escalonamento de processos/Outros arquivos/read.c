#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE *arquivo;
    char linha[1024];
    char *token;
    int pid, ppid, tempo_chegada, tempo_servico, tempo_entrada_io;
    char tipo_io;
    int i = 1;

    arquivo = fopen("entrada.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro: problema ao abrir o arquivo.\n");
    }

    // percorre cada linha
    while (1)
    {
        char *teste = fgets(linha, 1024, arquivo);
        char *separador = ";";

        if (teste == NULL) break;

        // le dados do processo
        token = strtok(linha, separador);
        pid = atoi(token);

        token = strtok(NULL, separador);
        ppid = atoi(token);

        token = strtok(NULL, separador);
        tempo_chegada = atoi(token);

        token = strtok(NULL, separador);
        tempo_servico = atoi(token);

        printf("\nProcesso %d:\n\tpid - %d\n\tppid - %d\n\tchegada - %d\n\tserviço - %d\n\tIO:\n", i, pid, ppid, tempo_chegada, tempo_servico);

        token = strtok(NULL, separador);
        while (token != NULL && *token != '\n')
        {
            tipo_io = token[0];

            token = strtok(NULL, separador);
            tempo_entrada_io = atoi(token);

            printf("\t\ttipo %c tempo %d\n", tipo_io, tempo_entrada_io);
            token = strtok(NULL, separador);
        }

        i++;
    }
    
    return 0;
}
