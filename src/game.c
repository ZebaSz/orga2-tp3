/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"

unsigned int mapa_backup[80*25] = {};

zombi_info zombis[16] = {
	[0] = (zombi_info) {
	},	
	[1] = (zombi_info) {
	},	
	[2] = (zombi_info) {
	},	
	[3] = (zombi_info) {
	},	
	[4] = (zombi_info) {
	},	
	[5] = (zombi_info) {
	},	
	[6] = (zombi_info) {
	},	
	[7] = (zombi_info) {
	},	
	[8] = (zombi_info) {
	},	
	[9] = (zombi_info) {
	},	
	[10] = (zombi_info) {
	},
	[11] = (zombi_info) {
	},
	[12] = (zombi_info) {
	},
	[13] = (zombi_info) {
	},
	[14] = (zombi_info) {
	},
	[15] = (zombi_info) {
	}
};

jugador jugadores[2] = {

	[JUG_A] = (jugador) {
		1, // unsigned int yPos;
		0, // unsigned int xPos;
		0, // unsigned int type;
		0, // unsigned int score;
		0, // unsigned int current;
		20 // unsigned int remaining;
	},
	[JUG_B] = (jugador) {
		1, // unsigned int yPos;
		79,// unsigned int xPos;
		0, // unsigned int type;
		0, // unsigned int score;
		0, // unsigned int current;
		20 // unsigned int remaining;
	}
};

char * segmentos[6] = {
	"cs",
	"ds",
	"es",
	"fs",
	"gs",
	"ss"
};

char * registros_comunes[13] = {
	"eax",
	"ecx",
	"edx",
	"ebx",
	"esp",
	"ebp",
	"esi",
	"edi",
	"eip",
	"cr0",
	"cr2",
	"cr3",
	"cr4"
};

char * exceptions[20] = {
	"Divide Error",
    "RESERVED",
    "NMI Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack-Segment Fault",
    "General Protection",
    "Page Fault",
    "Reserved",
    "x87 FPU Foating-Point Error",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception"
};

unsigned short debug_info_segmentos[6] = {};
unsigned int debug_info_regsitros[13] = {};
unsigned int * debug_info_stack;
unsigned int debug_info_error;
unsigned char debug_info_jugador;
unsigned int debug_info_eflags;
unsigned char debug_info_tipo;

char* zombi_char[4] = {"G","M","C", "x"};
char* zombi_status[5] = {"-", "\\", "|", "/", "x"};
unsigned char FONDO_JUG_A =  C_FG_WHITE | C_BG_RED;
unsigned char FONDO_JUG_B =  C_FG_WHITE | C_BG_BLUE;
unsigned char ENDGAME = FALSE;

int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

unsigned int get_task_code(unsigned int jugador) {
	if(jugador == JUG_A) {
		switch(jugadores[jugador].tipo) {
			case ZOMBI_TYPE_G:
				return TASK_AG;
			case ZOMBI_TYPE_M:
				return TASK_AM;
			case ZOMBI_TYPE_C:
				return TASK_AC;
		}
	} else {
		switch(jugadores[jugador].tipo) {
			case ZOMBI_TYPE_G:
				return TASK_BG;
			case ZOMBI_TYPE_M:
				return TASK_BM;
			case ZOMBI_TYPE_C:
				return TASK_BC;
		}
	}
	return TASK_IDLE;
}

void print_zombi(int pos) {
	unsigned short attr = (pos == 0) ? FONDO_JUG_A : FONDO_JUG_B;	
	print(zombi_char[jugadores[pos].tipo], jugadores[pos].xPos , jugadores[pos].yPos, attr);
}

void print_remaining(unsigned char jugador) {
	unsigned char zombiCount = (jugador == 0) ? ZOMBICOUNT_A_OFFSET: ZOMBICOUNT_B_OFFSET;
	unsigned char attr = (jugador == 0) ? C_FG_WHITE | C_BG_RED :  C_FG_WHITE | C_BG_BLUE;
	print_int(0, zombiCount - 1, 47, attr);
	print_int(jugadores[jugador].remaining, zombiCount, 47, attr);
}


void game_jugador_mover(unsigned int value, unsigned int jugador) {
	unsigned short attr = jugador == 0 ? C_FG_WHITE | C_BG_RED : C_FG_WHITE | C_BG_BLUE;	
	print(" ", jugadores[jugador].xPos , jugadores[jugador].yPos, attr);
	int fut_yPos = mod(jugadores[jugador].yPos + value, 49);
	if (fut_yPos >= 45) {
		fut_yPos = 1;
	} else if (fut_yPos == 0) {
		fut_yPos = 44;
	}
	jugadores[jugador].yPos = fut_yPos;
	print_zombi(jugador);
}

void game_print_rastro(unsigned int x, unsigned int y) {
	print("*",  x, y, C_FG_DARK_GREY| C_BG_GREEN);
}

void game_print_zombi_mapa(unsigned int zombi) {
	unsigned int jugador = zombi / 8;
	unsigned short attr;
	if(zombis[zombi].type == 3) {
		attr = (jugador == JUG_A ? C_FG_RED : C_FG_BLUE) | C_BG_GREEN;
	} else {
		attr = (jugador == JUG_A ? C_BG_RED : C_BG_BLUE) | C_FG_WHITE;
	}
	print(zombi_char[zombis[zombi].type],  zombis[zombi].xPos, zombis[zombi].yPos, attr);
}

void game_lanzar_zombi(unsigned int jugador) {
	if (jugadores[jugador].remaining > 0 && jugadores[jugador].current < CANT_ZOMBIS) {
		--jugadores[jugador].remaining;
		++jugadores[jugador].current;

		print_remaining(jugador);
		unsigned int zombi = sched_lanzar_tarea(jugador);
		tss_inicializar_zombi(jugador, jugadores[jugador].yPos-1, zombi % 8, 16 + zombi);

		unsigned int offset = (jugadores[jugador].yPos-1) * MAP_MEM_WIDTH;
		if(jugador == JUG_B) {
			offset += MAP_MEM_WIDTH - PAGE_SIZE;
		}
		memcpy(get_task_code(jugador), MAP_START + offset, PAGE_SIZE);

		//zombi = jugador == JUG_A ? zombi : zombi - 8;
		zombis[zombi].xPos = (jugador == JUG_A ? jugadores[jugador].xPos + 1 : jugadores[jugador].xPos-1);
		zombis[zombi].yPos = jugadores[jugador].yPos;
		zombis[zombi].type = jugadores[jugador].tipo;
		zombis[zombi].chirimbolo = 0;
		unsigned short attr = (jugador == JUG_A ? C_FG_RED : C_FG_BLUE) | C_BG_GREEN;	
		print(zombi_char[zombis[zombi].type],  zombis[zombi].xPos, zombis[zombi].yPos, attr);
	}
}

void game_move_current_zombi(direccion dir) {
	//llamar sched eso me da el juegador y la posicion
	unsigned int tarea = sched_tarea_actual();
	unsigned int owner = tarea / 8;
	// imprimir rastro zombi
	game_print_rastro(zombis[tarea].xPos, zombis[tarea].yPos);
	// calcular nueva pos
	unsigned int newXPos = zombis[tarea].xPos;
	unsigned int newYPos = zombis[tarea].yPos;
	int mov = owner == JUG_A ? 1 : -1;
	switch(dir) {
		case IZQ:
			newYPos -= mov;
			break;
		case DER:
			newYPos += mov;
			break;
		case ADE:
			newXPos += mov;
			break;
		case ATR:
			newXPos -= mov;
			break;
	}
	// ajustar para vuelta al mapa
	newYPos = mod(newYPos - 1, 44) + 1;
	if(newXPos == 0 || newXPos == 79) {
		unsigned int puntoPara = newXPos == 0 ? JUG_B : JUG_A;
		if(++jugadores[puntoPara].score == 10) {
			game_finalizar();
		}
		game_print_score(puntoPara);
		game_matar_zombi_actual();
		sched_matar_tarea_actual();
	} else {
		// mapear correctamente
		mmu_mover_zombi(owner, zombis[tarea].xPos-1, zombis[tarea].yPos-1, newXPos-1, newYPos-1);
		// mover zombi
		zombis[tarea].xPos = newXPos;
		zombis[tarea].yPos = newYPos;
		// imprimir zombi
		game_print_zombi_mapa(tarea);
	}
}

void game_matar_zombi_actual() {
	unsigned int tarea = sched_tarea_actual();
	--jugadores[tarea / 8].current;
	zombis[tarea].chirimbolo = 4;

	// guardamos el tipo del zombi por si es necesario
	debug_info_tipo = zombis[tarea].type;

	zombis[tarea].type = 3; //ded
	game_print_zombi_status(tarea);
	game_print_zombi_mapa(tarea);
	if(jugadores[JUG_A].remaining == 0 && jugadores[JUG_B].remaining == 0 &&
		jugadores[JUG_A].current == 0 && jugadores[JUG_B].current == 0) {
		game_finalizar();
	}
}

void game_debug_close() {
	//vuelve el mapa a como estaba antes
	unsigned int* video = (unsigned int*)VIDEO;
	unsigned int i;
    for(i = 0; i< 80*25; i++) {
        video[i] = mapa_backup[i];
    }
}

void game_debug_info(unsigned int* informacion) {
	//guardo info de la tarea actual
	debug_info_jugador = sched_tarea_actual() / 8;

	//guardo la info de los registros
	int i, j;
	for(i = 0;i < 6; i ++) {
		debug_info_segmentos[i] = informacion[5 - i];
	}
	for(i = 0;i < 13; i++) {
		debug_info_regsitros[i] = informacion[18 - i];
	}
	debug_info_error = informacion[19];

	// salteamos informacion[20], error code
	debug_info_regsitros[8] = informacion[21]; // corregimos EIP
	debug_info_segmentos[0] = informacion[22]; // corregimos CS
	debug_info_eflags = informacion[23];
	debug_info_regsitros[4] = informacion[24]; // corregimos ESP
	debug_info_stack = (unsigned int*) informacion[24];
	debug_info_segmentos[5] = informacion[25]; // corregimos SS


	//copio mapa dsp de matar tarea
	unsigned int* video = (unsigned int*) VIDEO;
	for(i = 0; i< 80*25; i++) {
   		mapa_backup[i] = video[i];
	}

	char * info_jugador;
	char * info_tipo;
    switch(debug_info_tipo) {
        case ZOMBI_TYPE_G:
          	info_tipo = "Guerrero";
            break;
        case ZOMBI_TYPE_M:
            info_tipo = "Mago";
            break;
        case ZOMBI_TYPE_C:
            info_tipo = "Clerigo";
            break;    
    }

    info_jugador = debug_info_jugador == 0 ? "Zombie A " : "Zombie B ";


    //dibujo bordes del debug
	for(i = 0; i < DEBUG_WIDTH; i++) {
    	for(j = 0; j< DEBUG_HEIGHT; j++) {
    		print(" ", i + DEBUG_CORNER_X, j + DEBUG_CORNER_Y, C_BG_BLACK);
    	}
    }

    //pinto interior
    for(i = 1; i < DEBUG_WIDTH - 1; i++) {
    	for(j = 2; j< DEBUG_HEIGHT - 1; j++) {
    		print(" ", i + DEBUG_CORNER_X , j + DEBUG_CORNER_Y, C_BG_LIGHT_GREY);
    	}
    }
    //pinto titulo
    for(i = 1; i< DEBUG_WIDTH - 1; i++) {
    	print(" ", i + DEBUG_CORNER_X , 1 + DEBUG_CORNER_Y, C_BG_BLUE);
    }
    
    //imprimo excepcion, tipo de zombie y tipo de jugador
    print(info_jugador, DEBUG_INFO_X, DEBUG_INFO_Y, C_FG_WHITE | C_BG_BLUE);
    print(info_tipo, DEBUG_INFO_X + 9, DEBUG_INFO_Y, C_FG_WHITE | C_BG_BLUE);
    print(exceptions[debug_info_error], DEBUG_INFO_X + 20, DEBUG_INFO_Y, C_FG_WHITE | C_BG_BLUE);


    //imprimo segmentos
    for(i = 0; i < 6; i++) {
        print(segmentos[i], DEBUG_REGISTROS_X + 20, DEBUG_REGISTROS_Y + i*2, C_FG_BLACK | C_BG_LIGHT_GREY);
        print_hex(debug_info_segmentos[i], 4, DEBUG_REGISTROS_X + 24, DEBUG_REGISTROS_Y + i*2, C_FG_WHITE | C_BG_LIGHT_GREY);
    }

    //imprimo registros comunes
    for(i = 0; i < 13; i++) {
        print(registros_comunes[i], DEBUG_REGISTROS_X, DEBUG_REGISTROS_Y + i*2, C_FG_BLACK | C_BG_LIGHT_GREY);
        print_hex(debug_info_regsitros[i], 8, DEBUG_REGISTROS_X + 4, DEBUG_REGISTROS_Y + i*2, C_FG_WHITE | C_BG_LIGHT_GREY);
    }

    //imprimo stack
    print("Stack: ", DEBUG_REGISTROS_X + 20, DEBUG_REGISTROS_Y + 12, C_FG_BLACK | C_BG_LIGHT_GREY);
    for(i = 0; i < 4; i++) {
        print("[        ]", DEBUG_REGISTROS_X + 29, i + DEBUG_REGISTROS_Y + 14, C_FG_BLACK | C_BG_LIGHT_GREY);    
        print_hex((unsigned int)debug_info_stack, 8, DEBUG_REGISTROS_X + 20, i + DEBUG_REGISTROS_Y + 14, C_FG_WHITE | C_BG_LIGHT_GREY);
        print_hex(*debug_info_stack, 8, DEBUG_REGISTROS_X + 30, i + DEBUG_REGISTROS_Y + 14, C_FG_WHITE | C_BG_LIGHT_GREY);
        debug_info_stack++;
    }

    //imprimo eflags
    print("eflags", DEBUG_REGISTROS_X + 20, DEBUG_REGISTROS_Y + 20, C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(debug_info_eflags, 8, DEBUG_REGISTROS_X + 27, DEBUG_REGISTROS_Y +20, C_FG_WHITE | C_BG_LIGHT_GREY);


}

void game_jugador_cambiar_zombi(unsigned int value, unsigned int jugador) {
	jugadores[jugador].tipo = mod(jugadores[jugador].tipo + value, 3);
	print_zombi(jugador);
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
			game_jugador_cambiar_zombi(-1, JUG_A);
			break;
		case KEY_CODE_D:
			game_jugador_cambiar_zombi(1, JUG_A);
			break;
		case KEY_CODE_I:
			game_jugador_mover(-1, JUG_B);
			break;
		case KEY_CODE_K:
			game_jugador_mover(1, JUG_B);
			break;
		case KEY_CODE_J:
			game_jugador_cambiar_zombi(-1, JUG_B);
			break;
		case KEY_CODE_L:
			game_jugador_cambiar_zombi(1, JUG_B);
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

void game_print_clock(unsigned int zombi) {
	if(zombi < 16) {
		++zombis[zombi].chirimbolo;
		if(zombis[zombi].chirimbolo > 3) {
			zombis[zombi].chirimbolo = 0;
		}
		game_print_zombi_status(zombi);
	}
}

void game_print_zombi_status(unsigned int zombi) {
	unsigned int jugador = zombi / 8;
	unsigned int x_off = jugador == JUG_A ? ZOMBIS_A_OFFSET : ZOMBIS_B_OFFSET;
	unsigned short attr = (jugador == JUG_A ? C_FG_RED : C_FG_BLUE) | C_BG_BLACK;
	unsigned int chirimbolo = zombis[zombi].chirimbolo;
	print(zombi_status[chirimbolo], x_off + ((zombi % 8)* 2), 48, attr);
}

void game_print_score(unsigned int jugador) {
	unsigned int x_off = jugador == JUG_A ? SCORE_A_OFFSET : SCORE_B_OFFSET;
	unsigned short attr = (jugador == JUG_A ? C_BG_RED : C_BG_BLUE) | C_FG_WHITE;
	print_int(jugadores[jugador].score, x_off, 47, attr);
}

void game_inicializar() {
	print_zombi(JUG_A);
	print_zombi(JUG_B);
	int i;
	for (i = 0; i < CANT_ZOMBIS; ++i) {
		zombis[i].chirimbolo = 4;
		zombis[i+8].chirimbolo = 4;

		print_int(i+1, 2*i + ZOMBIS_A_OFFSET, 46, C_FG_WHITE | C_BG_BLACK);
		game_print_zombi_status(i);
		print_int(i+1, 2*i + ZOMBIS_B_OFFSET, 46, C_FG_WHITE | C_BG_BLACK);
		game_print_zombi_status(i + 8);
	}
	game_print_score(JUG_A);
	game_print_score(JUG_B);
	print_remaining(JUG_A);
	print_remaining(JUG_B);
}

void game_finalizar() {
	ENDGAME = TRUE;
	print("FIN DEL JUEGO!", 33, 18, C_FG_WHITE | C_BG_BLACK);
	if(jugadores[JUG_B].score == jugadores[JUG_A].score) {
		print("EMPATE!", 37, 20, C_FG_WHITE | C_BG_BLACK);
	} else {
		int ganador;
		if(jugadores[JUG_B].score < jugadores[JUG_A].score) {
			ganador = JUG_A;
		} else {
			ganador = JUG_B;
		}
		print("GANADOR: ", 35, 20, C_FG_WHITE | C_BG_BLACK);
		print(ganador == JUG_A ? "A" : "B", 44, 20, C_FG_WHITE | (ganador == JUG_A ? C_BG_RED : C_BG_BLUE));
	}
	sched_matar_tarea_actual();
}