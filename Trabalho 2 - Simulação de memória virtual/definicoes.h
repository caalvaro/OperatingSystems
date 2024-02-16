#ifndef DEFINICOES_H_INCLUDED
#define DEFINICOES_H_INCLUDED

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define QTD_THREADS 20
#define WORKING_SET_LIMIT 4
#define QTD_PAGINAS 50
#define QTD_FRAMES 64
#define NOT_SET -1

typedef struct _Processo Processo;
typedef struct _Pagina Pagina;
typedef struct _Frame Frame;

struct _Processo {
  int pid;
  int qtd_paginas_alocadas;
  Pagina *paginas;
};

struct _Pagina {
  int id;
  int pid;
  int bit_presenca;
  int primeira_referencia;
  int frame_id;
};

struct _Frame {
  int id;
  int timestamp;
  int pid;
  int pagina_id;
};

#endif