#include "frames.h"

// variáveis globais definidas em main.c
extern int tempo;
extern Processo *processos;
extern Frame *frames;
extern int qtd_frames_alocados;

/*
  Descrição: Aloca e inicializa a estrura de dados para guardar informações de
  um frame

  Parâmetros:
    - int id: id do frame
 */
Frame cria_frame(int id) {
  Frame *frame = (Frame *)malloc(sizeof(Frame));
  if (frame == 0) {
    printf("Erro ao alocar memória para o frame.\n");
    exit(1);
  }

  frame->id = id;
  frame->timestamp = NOT_SET;
  frame->pid = NOT_SET;
  frame->pagina_id = NOT_SET;

  return *frame;
}

/*
  Descrição: Retorna um ponteiro para a struct que representa o frame

  Parâmetros:
    - int id: id do frame
 */
Frame *get_frame(int id) { return &frames[id]; }

/*
  Descrição: Retorna o primeiro frame que estiver vazio na memória
 */
Frame *get_frame_vazio() {
  Frame *frame_vazio, *frame_atual;

  for (int id = 0; id < QTD_FRAMES; id++) {
    frame_atual = get_frame(id);

    if (frame_atual->pagina_id == NOT_SET) {
      frame_vazio = frame_atual;
      break;
    }
  }

  printf("- Escolhi o frame: %d\n", frame_vazio->id);

  return frame_vazio;
}

/*
  Descrição: Retorna o frame com menor timestamp que está na memória
 */
Frame *get_frame_menor_timestamp() {
  Frame *frame_menor_timestamp = get_frame(0), *frame_atual;

  for (int id = 0; id < QTD_FRAMES; id++) {
    frame_atual = get_frame(id);

    if (frame_atual->timestamp < frame_menor_timestamp->timestamp) {
      frame_menor_timestamp = frame_atual;
    }
  }

  printf("- Escolhi a página %d do processo %d no frame %d.\n",
         frame_menor_timestamp->pagina_id,
         frame_menor_timestamp->pid,
         frame_menor_timestamp->id);

  return frame_menor_timestamp;
}

/*
  Descrição: Retorna o frame com menor timestamp de um processo específico

  Parâmetros:
    - Processo *processo: Ponteiro para o processo que se quer pegar um frame
 */
Frame *get_frame_mais_antigo_do_processo(Processo *processo) {
  Frame *frame_menor_timestamp, *frame_atual;

  // pega o primeiro frame desse processo só pra usar como comparação
  for (int id = 0; id < QTD_FRAMES; id++) {
    frame_atual = get_frame(id);

    if (frame_atual->pid == processo->pid) {
      frame_menor_timestamp = frame_atual;
      break;
    }
  }

  // pega o frame desse processo com menor timestamp
  for (int id = 0; id < QTD_FRAMES; id++) {
    frame_atual = get_frame(id);

    if (frame_atual->pid == processo->pid) {
      if (frame_atual->timestamp < frame_menor_timestamp->timestamp) {
        frame_menor_timestamp = frame_atual;
      }
    }
  }

  printf("- Escolhi a página %d no frame %d.\n",
         frame_menor_timestamp->pagina_id, frame_menor_timestamp->id);

  return frame_menor_timestamp;
}

/*
  Descrição: Atualiza o timestamp do frame

  Parâmetros:
    - int frame_id: id do frame
 */
void atualiza_timestamp(int frame_id) {
  //int timestamp = time(NULL);
  Frame *frame = get_frame(frame_id);

  frame->timestamp = tempo;

  printf("- Timestamp atualizado.\n");
}

/*
  Descrição: Retira uma página do frame. Faz isso atualizando as informações dos frames, do processo e da página

  Parâmetros:
    - Frame *frame: ponteiro pro frame que deseja-se esvaziar
 */
Pagina *retira_pagina(Frame *frame) {
  Processo *processo_pagina_retirada = get_processo(frame->pid);
  Pagina *pagina_retirada =
      get_pagina(processo_pagina_retirada, frame->pagina_id);

  printf("- Retirando a página %d, do processo %d, no frame %d.\n",
         pagina_retirada->id, pagina_retirada->pid, pagina_retirada->frame_id);

  // atualiza a quantidade de páginas alocadas no processo
  processo_pagina_retirada->qtd_paginas_alocadas--;
  
  pagina_retirada->bit_presenca = 0;
  pagina_retirada->frame_id = NOT_SET;

  frame->pagina_id = NOT_SET;
  frame->pid = NOT_SET;
  frame->timestamp = NOT_SET;

  qtd_frames_alocados--;

  printf("- Página %d do processo %d foi para o Swapping.\n",
         pagina_retirada->id, pagina_retirada->pid);

  return pagina_retirada;
}

/*
  Descrição: Aloca uma página no frame. Faz isso atualizando as informações dos frames, do processo e da página

  Parâmetros:
    - Frame *frame: ponteiro pro frame que deseja-se colocar uma nova página
    - Processo *processo: ponteiro pro processo dono da página
    - Pagina *pagina: ponteiro pra página que deseja-se alocar
 */
void aloca_pagina_no_frame(Frame *frame, Processo *processo, Pagina *pagina) {
  // atualiza informações no frame
  frame->pagina_id = pagina->id;
  frame->pid = processo->pid;
  atualiza_timestamp(frame->id);

  // atualiza a quantidade de páginas alocadas no processo
  processo->qtd_paginas_alocadas++;

  if (pagina->primeira_referencia == 1) {
    // a página já foi refernceiada antes
    printf("- Não é primeira referência da página.\n");
    printf("- Página %d do processo %d saiu do Swapping e foi para "
           "memória.\n",
           pagina->id, processo->pid);
  } else {
    // primeira vez que a página é referenciada
    printf("- Primeira referência da página.\n");
    printf("- Página %d do processo %d foi para memória.\n", pagina->id,
           processo->pid);
    pagina->primeira_referencia = 1;
  }

  qtd_frames_alocados++;

  pagina->frame_id = frame->id;
  pagina->bit_presenca = 1;
}


/*
  Descrição: Imprime a configuração dos frames da memória.
             O resultado impresso é no formato [id: (pid, pagina_id, timestamp)]
             Onde 'id' é o id do frame, 
                  'pid' é o pid do processo dono da página e 
                  'pagina_id' é o id da página que está no frame
 */
void print_frames() {
  Frame *frame_atual;
  int id = 0;

  printf("\nFrames: [id: (pid, pagina_id, timestamp)]\n");

  for (int i = 0; i < 16; i++) {
    printf("|");

    for (int j = 0; j < 4; j++) {
      frame_atual = get_frame(id);

      if (frame_atual->pagina_id == NOT_SET) {
        printf(" %2d:               |", frame_atual->id);
      } else {
        printf(" %2d: (%2d, %2d, %2dt) |",
          frame_atual->id,
          frame_atual->pid,
          frame_atual->pagina_id,
          frame_atual->timestamp);
      }

      id++;
    }

    printf("\n");
  }
}
