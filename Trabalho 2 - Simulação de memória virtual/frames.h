#ifndef FRAMES_H_INCLUDED
#define FRAMES_H_INCLUDED

#include "definicoes.h"
#include "processos.h"
#include "paginas.h"

Frame cria_frame(int id);
Frame *get_frame(int id);
Frame *get_frame_vazio();
Frame *get_frame_menor_timestamp();
Frame *get_frame_mais_antigo_do_processo(Processo *processo);
void atualiza_timestamp(int frame_id);
Pagina *retira_pagina(Frame *frame);
void aloca_pagina_no_frame(Frame *frame, Processo *processo, Pagina *pagina);
void print_frames();

#endif