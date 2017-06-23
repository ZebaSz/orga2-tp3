/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "screen.h"
#include "tss.h"

#include "i386.h"

unsigned short sched_proximo_indice();

void sched_inicializar();

void sched_marcar_idle();

void sched_toggle_debug();

unsigned int sched_lanzar_tarea(unsigned int jugador);

void sched_matar_tarea_actual();

extern unsigned int tareaActual;


#endif	/* !__SCHED_H__ */
