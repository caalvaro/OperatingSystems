#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE *arquivo;
    char linha[1024];
    char *token;

    arquivo = fopen("entrada.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro: problema ao abrir o arquivo.\n");
    }

    while (1)
    {
        char *teste = fgets(linha, 1024, arquivo);

        if (teste == NULL) break;

        token = strtok(linha, " ");

        while (token)
        {
            printf("token: %s\n", token);

            token = strtok(NULL, " ");
        }
    }
    
    return 0;
}
