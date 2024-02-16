#include "definicoes.h"
#include "frames.h"
#include "paginas.h"
#include "processos.h"

pthread_mutex_t mutex;
int tempo = 0; // guarda o tempo para usar como timestamp
Processo *processos; // ponteiro pro início do array de processos
Frame *frames; // ponteiro pro início do array de frames
int qtd_frames_alocados = 0; // quantidade de frames que estão sendo usados na memória

void *processo_thread(void *arg) {
  int pid = *(int *)arg;
  Processo *processo = get_processo(pid);

  while (1) {
    sleep(3);
    pthread_mutex_lock(&mutex);
    
    printf("\n\nProcesso %d:\n", processo->pid);
    print_paginas(processo);

    // escolhe página aleatória
    int pagina_referenciada_id = rand() % QTD_PAGINAS;
    Pagina *pagina_referenciada = get_pagina(processo, pagina_referenciada_id);

    printf("- Escolhi a página para alocar na memória: %d\n",
           pagina_referenciada_id);

    // se a página já está na memória
    if (pagina_referenciada->bit_presenca == 1) {
      printf("- Página %d do processo %d já está na memória.\n",
             pagina_referenciada->id, processo->pid);

      // atualiza timestamp do frame que está a página
      atualiza_timestamp(pagina_referenciada->frame_id);
    }
    // se a página ainda não está na memória
    else if (pagina_referenciada->bit_presenca == 0) {
      printf("- Página %d do processo %d não está na memória.\n",
             pagina_referenciada->id, processo->pid);

      // se o processo atingiu o working set limit
      if (processo->qtd_paginas_alocadas == WORKING_SET_LIMIT) {
        printf("- Atingi o working set limit.\n"
               "- Escolhendo uma página minha para tirar...\n");

        // retira uma página do processo com menor timestamp
        Frame *frame_menor_timestamp =
            get_frame_mais_antigo_do_processo(processo);

        // retira a página do frame e joga pro swap
        retira_pagina(frame_menor_timestamp);

        // aloca a página no frame
        aloca_pagina_no_frame(frame_menor_timestamp,
                              processo,
                              pagina_referenciada);
      }
      // se o processo ainda não atingiu o working set limit
      else if (processo->qtd_paginas_alocadas < WORKING_SET_LIMIT) {
        printf("- Não atingi o working set limit.\n");
        
        // verifica se a memória tá cheia
        if (qtd_frames_alocados == QTD_FRAMES) {
          printf("- Memoria cheia!\n"
                 "- Escolhendo uma página qualquer para tirar...\n");

          Frame *frame_menor_timestamp = get_frame_menor_timestamp();

          // joga a página pro swap
          retira_pagina(frame_menor_timestamp);

          // aloca a página no frame
          aloca_pagina_no_frame(frame_menor_timestamp,
                                processo,
                                pagina_referenciada);
        }
        // quando a memória não tá cheia
        else if (qtd_frames_alocados < QTD_FRAMES) {
          printf("- Memória não está cheia!\n- Escolhendo um frame vazio...\n");

          Frame *frame_vazio = get_frame_vazio();

          // aloca a página num frame qualquer que esteja vazio
          aloca_pagina_no_frame(frame_vazio,
                                processo,
                                pagina_referenciada);
        }
      }
    }
    
    print_frames();

    tempo++;
    pthread_mutex_unlock(&mutex);
  }

  pthread_exit(NULL);
}

int main(void) {
  int i;
  pthread_t thread_ids[QTD_THREADS]; // guarda os ponteiros para as threads
  int ids_processos[QTD_THREADS]; // guarda os ids dos processos

  frames = (Frame *)calloc(QTD_FRAMES, 2*sizeof(Frame *));
  processos = (Processo *)calloc(QTD_THREADS, 2*sizeof(Processo *));
  srand(time(NULL)); // configura o seed da função random

  pthread_mutex_init(&mutex, NULL);

  // cria os frames
  for (i = 0; i < QTD_FRAMES; i++) {
    *(frames+i) = cria_frame(i);
  }

  // cria os processos e inicializa as threads
  for (i = 0; i < QTD_THREADS; i++) {
    ids_processos[i] = i;

    processos[i] = cria_processo(i);

    if (pthread_create(&thread_ids[i], (void *)NULL, processo_thread,
                       (void *)&ids_processos[i]))
      printf("Error -- pthread_create\n");

    sleep(3);
  }
  
  pthread_exit(NULL);
}