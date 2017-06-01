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







