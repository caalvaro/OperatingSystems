#include "paginas.h"

/*
  Descrição: Aloca e inicializa a estrura de dados para guardar informações de uma página

  Parâmetros:
    - int id: id da página
    - int pid: id do processo dono da página
 */
Pagina cria_pagina(int id, int pid) {
  Pagina *pagina = (Pagina *)malloc(sizeof(Pagina));
  if (pagina == 0) {
    printf("Erro ao alocar memória para uma página.\n");
    exit(1);
  }
  
  pagina->id = id;
  pagina->pid = pid;
  pagina->bit_presenca = 0;
  pagina->primeira_referencia = 0;
  pagina->frame_id = NOT_SET;

  return *pagina;
}

/*
  Descrição: Retorna um ponteiro para a struct que representa a página

  Parâmetros:
    - Processo *processo: ponteiro pro processo dono da página
    - int id: id da página
 */
Pagina *get_pagina(Processo *processo, int id) {
  return &processo->paginas[id];
}