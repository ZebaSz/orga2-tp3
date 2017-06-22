/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

jugador jugadores[2];
char* zombies[3] = {"G","M","C"};
unsigned char FONDO_JUG_A =  C_FG_WHITE | C_BG_RED;
unsigned char FONDO_JUG_B =  C_FG_WHITE | C_BG_BLUE;


int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

void print_zombie(int pos) {
	unsigned short attr = (pos == 0) ? FONDO_JUG_A : FONDO_JUG_B;	
	print(zombies[jugadores[pos].tipo_zombie], jugadores[pos].pos_x , jugadores[pos].pos_y, attr);
}

void game_jugador_mover(unsigned int value, unsigned int jugador) {
	unsigned short attr = jugador == 0 ? C_FG_WHITE | C_BG_RED : C_FG_WHITE | C_BG_BLUE;	
	print(" ", jugadores[jugador].pos_x , jugadores[jugador].pos_y, attr);
	int fut_pos_y = mod(jugadores[jugador].pos_y + value, 49);
	if (fut_pos_y >= 45) {
		fut_pos_y = 1;
	} else if (fut_pos_y == 0) {
		fut_pos_y = 44;
	}
	jugadores[jugador].pos_y = fut_pos_y;
	print_zombie(jugador);
}

void game_lanzar_zombi(unsigned int jugador) {
}

void game_move_current_zombi(direccion dir) {
}

void game_jugador_cambiar_zombie(unsigned int value, unsigned int jugador) {
	jugadores[jugador].tipo_zombie = mod(jugadores[jugador].tipo_zombie + value, 3);
	print_zombie(jugador);
}

void game_jugador_tecla(unsigned int value) {
	switch(value) {
		case KEY_CODE_W:
			game_jugador_mover(-1, JUG_A);
			break;
		case KEY_CODE_S:
			game_jugador_mover(1, JUG_A);
			break;
		case KEY_CODE_A:
			game_jugador_cambiar_zombie(-1, JUG_A);
			break;
		case KEY_CODE_D:
			game_jugador_cambiar_zombie(1, JUG_A);
			break;
		case KEY_CODE_I:
			game_jugador_mover(-1, JUG_B);
			break;
		case KEY_CODE_K:
			game_jugador_mover(1, JUG_B);
			break;
		case KEY_CODE_J:
			game_jugador_cambiar_zombie(-1, JUG_B);
			break;
		case KEY_CODE_L:
			game_jugador_cambiar_zombie(1, JUG_B);
			break;
		case KEY_CODE_lShift:
			game_lanzar_zombi(JUG_A);
			break;
		case KEY_CODE_rShift:
			game_lanzar_zombi(JUG_B);
			break;
		default:
			return;			
	}
}

void game_inicializar() {
	jugadores[JUG_A].pos_y = 1;
	jugadores[JUG_A].pos_x = 0;
	jugadores[JUG_A].tipo_zombie = 0;
	jugadores[JUG_B].pos_y = 1;
	jugadores[JUG_B].pos_x = 79;
	jugadores[JUG_B].tipo_zombie = 0;
	print_zombie(JUG_A);
	print_zombie(JUG_B);
}

void game_print_zombi_status(unsigned int jugador, unsigned int zombie, const char* status) {
	unsigned int x_off = jugador == JUG_A ? ZOMBIS_A_OFFSET : ZOMBIS_B_OFFSET;
	unsigned short attr = (jugador == JUG_A ? C_FG_RED : C_FG_BLUE) | C_BG_BLACK;
	print(status, x_off + (zombie * 2), 48, attr);
}

void game_inicializar_tablero() {
	for (int i = 0; i < CANT_ZOMBIS; ++i) {
		print_int(i+1, 2*i + ZOMBIS_A_OFFSET, 46, C_FG_WHITE | C_BG_BLACK);
		game_print_zombi_status(JUG_A, i, "x");
		print_int(i+1, 2*i + ZOMBIS_B_OFFSET, 46, C_FG_WHITE | C_BG_BLACK);
		game_print_zombi_status(JUG_B, i, "x");
	}
	print_int(MAX_ZOMBIS, ZOMBICOUNT_A_OFFSET, 47, C_FG_WHITE | C_BG_RED);
	print_int(MAX_ZOMBIS, ZOMBICOUNT_B_OFFSET, 47, C_FG_WHITE | C_BG_BLUE);
	print_int(0, SCORE_A_OFFSET, 47, C_FG_WHITE | C_BG_RED);
	print_int(0, SCORE_B_OFFSET, 47, C_FG_WHITE | C_BG_BLUE);
}
