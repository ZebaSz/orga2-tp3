/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"



unsigned int PDE_OFFSET(unsigned int virtual) {
	return virtual >> 22;
}

unsigned int PTE_OFFSET(unsigned int virtual) {
	return (virtual << 10) >> 22;
}

unsigned int proxima_pagina_libre; //TODO: ver cual es la inicializacion: suponemos 0x29000 que puede ser la primera libre

void mmu_inicializar() {
	proxima_pagina_libre = INICIO_PAGINAS_LIBRES;
}

unsigned int mmu_proxima_pagina_fisica_libre() {
	unsigned int pagina_libre = proxima_pagina_libre;
	proxima_pagina_libre += PAGE_SIZE;
	return pagina_libre;
}

void mmu_inicializar_dir_kernel() {
	pd_entry* dir_paginas = (pd_entry*) PAGE_DIR;
	dir_paginas[0].p = 1;
	dir_paginas[0].rw = 1;
	dir_paginas[0].us = 0;
	dir_paginas[0].pwt = 0;
	dir_paginas[0].pcd = 0;
	dir_paginas[0].a = 0;
	dir_paginas[0].ign = 0;
	dir_paginas[0].ps = 0;
	dir_paginas[0].g = 0;
	dir_paginas[0].avl = 0;
	dir_paginas[0].page_addr = PAGE_TAB >> 12;


	pt_entry* tablas = (pt_entry*)PAGE_TAB;
	for(int i = 0; i < 1024; i++) {
		tablas[i].p = 1;
		tablas[i].rw = 1;
		tablas[i].us = 0;
		tablas[i].pwt = 0;
		tablas[i].pcd = 0;
		tablas[i].a = 0;
		tablas[i].d = 0;
		tablas[i].pat = 0;
		tablas[i].g = 0;
		tablas[i].avl = 0;
		tablas[i].page_addr = i;
	}
}

void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica) {
	pd_entry* dir_paginas = (pd_entry*) cr3;
	pt_entry* tablas;
	unsigned int dir_i = PDE_OFFSET(virtual);
	unsigned int tabla_i = PTE_OFFSET(virtual);
	pd_entry dir = dir_paginas[dir_i];
	if (dir.p) {
		tablas = (pt_entry*) ( (unsigned int) dir.page_addr); // ACA HAY FRUTELI
	} else {
		tablas = (pt_entry*) (mmu_proxima_pagina_fisica_libre());
		for (unsigned int i = 0; i < 1024; i++){
			tablas[i].p = 0; 
		}
	}

	tablas[tabla_i].p = 1; //TODO: preguntar por los atributos
	tablas[tabla_i].rw = 1;
	tablas[tabla_i].us = 0;
	tablas[tabla_i].pwt = 0;
	tablas[tabla_i].pcd = 0;
	tablas[tabla_i].a = 0;
	tablas[tabla_i].d = 0;
	tablas[tabla_i].pat = 0;
	tablas[tabla_i].g = 0;
	tablas[tabla_i].avl = 0;
	tablas[tabla_i].page_addr = fisica;
	tlbflush();
}

void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3) {
	pd_entry* dir_paginas = (pd_entry*) cr3;
	unsigned int dir_i = PDE_OFFSET(virtual);
	unsigned int tabla_i = PTE_OFFSET(virtual);
	pt_entry* tablas = (pt_entry*) ( (unsigned int) dir_paginas[dir_i].page_addr);
	tablas[tabla_i].p = 0; //TODO PREGUNTAR
	tlbflush();
}







