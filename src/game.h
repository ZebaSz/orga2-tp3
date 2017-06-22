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
#include "i386.h"

typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ADE = 0x83D, ATR = 0x732 } direccion;

void game_jugador_mover(unsigned int value, unsigned int jugador);

void game_lanzar_zombi(unsigned int jugador);

void game_move_current_zombi(direccion dir);

void game_jugador_cambiar_zombie(unsigned int value, unsigned int jugador);

void game_jugador_tecla(unsigned int value);

void game_inicializar();

typedef struct str_jugador {
	unsigned int tipo_zombie;
	unsigned int pos_y; 
	unsigned int pos_x;
} __attribute__((__packed__)) jugador;

#endif  /* !__GAME_H__ */
