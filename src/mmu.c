/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"


void mmu_inicializar() {
	
}

void mmu_inicializar_dir_kernel() {
	unsigned int* dir_paginas = (unsigned int*) 0x27000;
	unsigned int* tablas = (unsigned int*) 0x28000;
	dir_paginas[0] = ((unsigned int) tablas) | 0x3;
	for(int i = 0; i < 1024; i++) {
		tablas[i] = (i << 12) | 0x3;
	}
}







