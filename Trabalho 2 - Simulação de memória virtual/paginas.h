#ifndef PAGINAS_H_INCLUDED
#define PAGINAS_H_INCLUDED
#include "definicoes.h"

Pagina cria_pagina(int id, int pid);
Pagina *get_pagina(Processo *processo, int id);

#endif