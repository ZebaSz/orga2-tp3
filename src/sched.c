/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#include "sched.h"

#define TASK_PER_PLAYER 8
#define TASK_COUNT TASK_PER_PLAYER * 2
#define TASK_OFFSET(task) task << 3;

unsigned char modo_debug;

unsigned char status_tareas[16] = {0};
unsigned int tareaActual;
unsigned int tareasAnteriores[2] = {0, 8};

// Buscar alguna tarea si no habia ninguna encolada
unsigned int sched_buscar_tarea(unsigned int jugador, unsigned int status) {
	unsigned int tarea = tareasAnteriores[jugador] + 1;
	if(tarea > (TASK_PER_PLAYER * (1 + jugador))) {
		tarea = TASK_PER_PLAYER * jugador;
	}
	while(tarea != tareasAnteriores[jugador] && status_tareas[tarea] != status) {
		++tarea;
		if(tarea > (TASK_PER_PLAYER * (1 + jugador))) {
			tarea = TASK_PER_PLAYER * jugador;
		}
	}
	return tarea;
}

unsigned short sched_proximo_indice() {
	unsigned int proxTarea = tareaActual;
	unsigned int ultimoJugador;
	if(proxTarea == TASK_COUNT) {
		ultimoJugador = 1;
	} else {
		ultimoJugador = tareaActual / TASK_PER_PLAYER;
	}
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
	if(proxTarea == tareaActual || status_tareas[proxTarea] == FALSE) {
		// no se cambia tarea si: 1) no hay tarea o 2) es la unica tarea
		proxTarea = 0;
	} else {
		// calculamos el offset del selector de la tarea en la gdt
		proxTarea += 16;
		proxTarea = TASK_OFFSET(proxTarea);
		// actualizamos la ultima tarea ejecutada por el jugador
		tareasAnteriores[proximoJugador] = tareaActual;
		// y la tarea actual en ejecucion
		tareaActual = proxTarea;
	}
	return proxTarea;
}

void sched_inicializar() {
	sched_marcar_idle();
}

void sched_marcar_idle() {
	tareaActual = TASK_COUNT; // setear la tarea como invalida
}

unsigned int sched_lanzar_tarea(unsigned int jugador) {
	unsigned int nuevaTarea = sched_buscar_tarea(jugador, TRUE);
	status_tareas[nuevaTarea] = TRUE;
	return nuevaTarea;
}

void sched_matar_tarea_actual() {
	status_tareas[tareaActual] = FALSE;
}

void sched_toggle_debug() {
	if(modo_debug == FALSE) {
		modo_debug = TRUE;
	} else {
		modo_debug = FALSE;
	}
}