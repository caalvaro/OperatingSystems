#ifndef PROCESSOS_H_INCLUDED
#define PROCESSOS_H_INCLUDED

#include "definicoes.h"

Processo cria_processo(int pid);
Processo *get_processo(int pid);
void print_paginas(Processo *processo);

#endif