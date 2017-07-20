/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "screen.h"
#include "mmu.h"
#include "sched.h"
#include "i386.h"

typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ADE = 0x83D, ATR = 0x732 } direccion;

void game_jugador_mover(unsigned int value, unsigned int jugador);

void game_lanzar_zombi(unsigned int jugador);

void game_move_current_zombi(direccion dir);

void game_matar_zombi_actual();

void game_jugador_cambiar_zombie(unsigned int value, unsigned int jugador);

void game_jugador_tecla(unsigned int value);

void game_print_zombi_status(unsigned int zombie);

void game_print_score(unsigned int jugador);

void game_inicializar();

void game_finalizar();

void game_debug_info(unsigned int* informacion);

void game_debug_close();

typedef struct str_debug_info {
} __attribute__((__packed__)) debug_info;

typedef struct str_jugador {
	unsigned int yPos;
	unsigned int xPos;
	unsigned int tipo;
	unsigned int score;
	unsigned int current;
	unsigned int remaining;
} __attribute__((__packed__)) jugador;

typedef struct zombi_info_s {
	unsigned int xPos;
	unsigned int yPos;
	unsigned int type;
	unsigned int chirimbolo;
}__attribute__((__packed__)) zombi_info;

extern zombi_info zombis[];
extern unsigned char ENDGAME;

#endif  /* !__GAME_H__ */
