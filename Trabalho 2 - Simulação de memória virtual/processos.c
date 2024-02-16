#include "processos.h"
#include "paginas.h"

extern Processo *processos;

/*
  Descrição: Aloca e inicializa as estruras de dados associadas ao processo

  Parâmetros:
    - int pid: pid do processo que será criado
 */
Processo cria_processo(int pid) {
  Processo *processo = (Processo *)malloc(sizeof(Processo));
  Pagina *paginas = (Pagina *)calloc(QTD_PAGINAS, sizeof(Pagina *));
  
  if (processo == 0) {
    printf("Erro ao alocar memória para o processo.\n");
    exit(1);
  }
  
  if (paginas == 0) {
    printf("Erro ao alocar memória para o array de pagina.\n");
    exit(1);
  }

  processo->pid = pid;
  processo->qtd_paginas_alocadas = 0;
  processo->paginas = paginas;

  for (int i = 0; i < QTD_PAGINAS; i++) {
    paginas[i] = cria_pagina(i, processo->pid);
  }

  return *processo;
}

/*
  Descrição: Retorna um ponteiro para a struct que representa o processo

  Parâmetros:
    - int pid: pid do processo que será retornado
 */
Processo *get_processo(int pid) {
  return &processos[pid];
}

/*
  Descrição: Printa quais páginas do processo estão alocadas em algum frame

  Parâmetros:
    - Processo *processo: ponteiro para a struct do processo cujas páginas serão mostradas
 */
void print_paginas(Processo *processo) {
  Pagina *pagina;

  printf("- Quantidade de páginas na memória: %d\n", processo->qtd_paginas_alocadas);

  if (processo->qtd_paginas_alocadas > 0) {
    printf("Páginas do processo em memória: |");

    for (int id = 0; id < QTD_PAGINAS; id++) {
      pagina = get_pagina(processo, id);
      
      if (pagina->bit_presenca == 1) {
        printf(" %d |", pagina->id);
      }
    }
  
    printf("\n");
  }

  printf("\n");
}