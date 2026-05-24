#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"

struct pstat {
  int inuse[NPROC];   // se o slot está em uso (processo existe)
  int pid[NPROC];     // PID de cada processo
  int tickets[NPROC]; // número de tickets de cada processo
  int ticks[NPROC];   // quantas vezes cada processo foi escalonado
};

#endif // _PSTAT_H_
