/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

#define TASK_PER_PLAYER 8
#define TASK_COUNT TASK_PER_PLAYER * 2
#define TASK_OFFSET(task) (task + 16) << 3

unsigned char modo_debug;

unsigned char status_tareas[16] = {FALSE};
unsigned int tareaActual;
unsigned int ultimoJugador;
unsigned int tareasAnteriores[2] = {TASK_PER_PLAYER - 1, TASK_COUNT - 1};

// Buscar alguna tarea si no habia ninguna encolada
unsigned int sched_buscar_tarea(unsigned int jugador, unsigned int status) {
	unsigned int tarea = tareasAnteriores[jugador] + 1;
	if(tarea >= (TASK_PER_PLAYER * (1 + jugador))) {
		tarea = TASK_PER_PLAYER * jugador;
	}
	for (int i = 0; i < TASK_PER_PLAYER && status_tareas[tarea] != status; ++i) {
		++tarea;
		if(tarea >= (TASK_PER_PLAYER * (1 + jugador))) {
			tarea = TASK_PER_PLAYER * jugador;
		}
	}
	return tarea;
}

unsigned short sched_proximo_indice() {
	unsigned int proxTarea = tareaActual;
	unsigned int proximoJugador = 1 - ultimoJugador;

	if(modo_debug == FALSE) {
		// buscamos alguna tarea del proximo jugador
		proxTarea = sched_buscar_tarea(proximoJugador, TRUE);
		if(status_tareas[proxTarea] == FALSE) {
			// si no tiene tareas, buscamos una del jugador actual
			proximoJugador = ultimoJugador;
			proxTarea = sched_buscar_tarea(proximoJugador, TRUE);
		}
	}
	if(proxTarea == TASK_COUNT || TASK_OFFSET(proxTarea) == rtr() || status_tareas[proxTarea] == FALSE) {
		// no se cambia tarea si: 1) no hay tarea o 2) es la unica tarea
		proxTarea = 0;
	} else {
		// actualizamos la ultima tarea ejecutada por el jugador
		tareasAnteriores[ultimoJugador] = tareaActual;
		// y la tarea actual en ejecucion
		tareaActual = proxTarea;
		print_int(proxTarea, 10,0 , C_FG_RED);

		// calculamos el offset del selector de la tarea en la gdt
		proxTarea = TASK_OFFSET(proxTarea);
		ultimoJugador = proximoJugador;
	}
	return proxTarea;
}

void sched_inicializar() {
	sched_marcar_idle();
}

void sched_marcar_idle() {
	tareaActual = TASK_COUNT; // setear la tarea como invalida
	ultimoJugador = 1;
}

unsigned int sched_tarea_actual() {
	if(status_tareas[tareaActual] == FALSE) {
		tareaActual = TASK_COUNT;
	}
	return tareaActual;
}

unsigned int sched_lanzar_tarea(unsigned int jugador) {
	unsigned int nuevaTarea = sched_buscar_tarea(jugador, FALSE);
	status_tareas[nuevaTarea] = TRUE;
	return nuevaTarea;
}

// llamar esto último, la tarea muere acá
void sched_matar_tarea_actual() {
	status_tareas[tareaActual] = FALSE;
    __asm __volatile("ljmp $0x68, $0" : : ); // ESTO ES UN JUMP FAR, TAREA MUERE DEFINITIVAMENTE ACA
}

void sched_toggle_debug() {
	if(modo_debug == FALSE) {
		modo_debug = TRUE;
	} else {
		modo_debug = FALSE;
	}
}