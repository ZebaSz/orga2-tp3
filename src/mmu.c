/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"

#define PDE_OFFSET(virtual) virtual >> 22
#define PTE_OFFSET(virtual) (virtual << 10) >> 22

unsigned int proxima_pagina_libre; //TODO: ver cual es la inicializacion: suponemos 0x29000 que puede ser la primera libre

int mem_mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

void mmu_inicializar() {
	proxima_pagina_libre = INICIO_PAGINAS_LIBRES;
}

unsigned int mmu_proxima_pagina_fisica_libre() {
	unsigned int pagina_libre = proxima_pagina_libre;
	proxima_pagina_libre += PAGE_SIZE;
	return pagina_libre;
}

void mmu_mapear_dir_kernel(unsigned int pd, unsigned int pt) {
	pd_entry* dir_paginas = (pd_entry*) pd;
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
	dir_paginas[0].page_addr = pt >> 12;


	pt_entry* tabla = (pt_entry*)pt;
	unsigned int i;
	for(i = 0; i < 1024; i++) {
		tabla[i].p = 1;
		tabla[i].rw = 1;
		tabla[i].us = 0;
		tabla[i].pwt = 0;
		tabla[i].pcd = 0;
		tabla[i].a = 0;
		tabla[i].d = 0;
		tabla[i].pat = 0;
		tabla[i].g = 0;
		tabla[i].avl = 0;
		tabla[i].page_addr = i;
	}
}

void mmu_inicializar_dir_kernel() {
	mmu_mapear_dir_kernel(KERNEL_PAGE_DIR, KERNEL_PAGE_TAB);
}

void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica) {
	pd_entry* dir_paginas = (pd_entry*) cr3;
	pt_entry* tablas;
	unsigned int dir_i = PDE_OFFSET(virtual);
	unsigned int tabla_i = PTE_OFFSET(virtual);
	pd_entry* dir = &dir_paginas[dir_i];
	if (dir->p) {
		tablas = (pt_entry*) ( (unsigned int) dir->page_addr << 12);
	} else {
		tablas = (pt_entry*) (mmu_proxima_pagina_fisica_libre());
		unsigned int i;
		for (i = 0; i < 1024; i++){
			tablas[i].p = 0; 
		}

		dir->p = 1;
		dir->rw = 1;
		dir->us = 1;
	    dir->pwt = 0;
	    dir->pcd = 0;
	    dir->a = 0;
	    dir->ign = 0;
	    dir->ps = 0;
	    dir->g = 0;
	    dir->avl = 0;
		dir->page_addr = (unsigned int)tablas >> 12;
	}

	tablas[tabla_i].p = 1;
	tablas[tabla_i].rw = 1;
	tablas[tabla_i].us = 1;
	tablas[tabla_i].pwt = 0;
	tablas[tabla_i].pcd = 0;
	tablas[tabla_i].a = 0;
	tablas[tabla_i].d = 0;
	tablas[tabla_i].pat = 0;
	tablas[tabla_i].g = 0;
	tablas[tabla_i].avl = 0;
	tablas[tabla_i].page_addr = (fisica >> 12);
	tlbflush();
}

void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3) {
	if(cr3 != 0) {
		unsigned int dir_i = PDE_OFFSET(virtual);
		unsigned int tabla_i = PTE_OFFSET(virtual);

		pd_entry* dir = &((pd_entry*) cr3)[dir_i];
		if(dir->p) {
			pt_entry* tablas = (pt_entry*) ( (unsigned int) dir->page_addr << 12);
			tablas[tabla_i].p = 0;
			tlbflush();
		}
	}
}

void mmu_mapear_area_zombi(unsigned int pd, unsigned char jugador, unsigned int centro) {
	char direccion = jugador == JUG_A ? 1 : -1;
	mmu_mapear_pagina(TASK_VIRT, pd,
		MAP_START + centro);

	mmu_mapear_pagina(TASK_VIRT+(1*PAGE_SIZE), pd,
		MAP_START + (centro + direccion * PAGE_SIZE));

	mmu_mapear_pagina(TASK_VIRT+(2*PAGE_SIZE), pd,
		MAP_START + mem_mod(centro + direccion * (PAGE_SIZE + MAP_MEM_WIDTH), MAP_MEM_SIZE));

	mmu_mapear_pagina(TASK_VIRT+(3*PAGE_SIZE), pd,
		MAP_START + mem_mod(centro + direccion * (PAGE_SIZE - MAP_MEM_WIDTH), MAP_MEM_SIZE));

	mmu_mapear_pagina(TASK_VIRT+(4*PAGE_SIZE), pd,
		MAP_START + mem_mod(centro + direccion * MAP_MEM_WIDTH, MAP_MEM_SIZE));

	mmu_mapear_pagina(TASK_VIRT+(5*PAGE_SIZE), pd,
		MAP_START + mem_mod(centro - direccion * MAP_MEM_WIDTH, MAP_MEM_SIZE));

	mmu_mapear_pagina(TASK_VIRT+(6*PAGE_SIZE), pd,
		MAP_START + (centro - direccion * PAGE_SIZE));

	mmu_mapear_pagina(TASK_VIRT+(7*PAGE_SIZE), pd,
		MAP_START + mem_mod(centro - direccion * (PAGE_SIZE + MAP_MEM_WIDTH), MAP_MEM_SIZE));

	mmu_mapear_pagina(TASK_VIRT+(8*PAGE_SIZE), pd,
		MAP_START + mem_mod(centro - direccion * (PAGE_SIZE - MAP_MEM_WIDTH), MAP_MEM_SIZE));
}

void mmu_desmapear_area_zombi(unsigned int pd, unsigned int centro) {
	mmu_unmapear_pagina(TASK_VIRT, pd);

	mmu_unmapear_pagina(TASK_VIRT+(1*PAGE_SIZE), pd);

	mmu_unmapear_pagina(TASK_VIRT+(2*PAGE_SIZE), pd);

	mmu_unmapear_pagina(TASK_VIRT+(3*PAGE_SIZE), pd);

	mmu_unmapear_pagina(TASK_VIRT+(4*PAGE_SIZE), pd);

	mmu_unmapear_pagina(TASK_VIRT+(5*PAGE_SIZE), pd);

	mmu_unmapear_pagina(TASK_VIRT+(6*PAGE_SIZE), pd);

	mmu_unmapear_pagina(TASK_VIRT+(7*PAGE_SIZE), pd);

	mmu_unmapear_pagina(TASK_VIRT+(8*PAGE_SIZE), pd);
}

unsigned int mmu_inicializar_dir_zombi(unsigned char jugador, unsigned char yPos, unsigned char tarea) {
	unsigned int pd = mmu_proxima_pagina_fisica_libre();
	unsigned int pt = mmu_proxima_pagina_fisica_libre();
	mmu_mapear_dir_kernel(pd, pt);

	char direccion = jugador == JUG_A ? 1 : -1;

	unsigned int centro = yPos * MAP_MEM_WIDTH;
	if(jugador == JUG_B) {
		centro += MAP_MEM_WIDTH - PAGE_SIZE;
	}

	mmu_mapear_pagina(TASK_VIRT, pd,
		MAP_START + centro);

	mmu_mapear_pagina(TASK_VIRT+(1*PAGE_SIZE), pd,
		MAP_START + (centro + direccion * PAGE_SIZE));

	mmu_mapear_pagina(TASK_VIRT+(2*PAGE_SIZE), pd,
		MAP_START + mem_mod(centro + direccion * (PAGE_SIZE + MAP_MEM_WIDTH), MAP_MEM_SIZE));

	mmu_mapear_pagina(TASK_VIRT+(3*PAGE_SIZE), pd,
		MAP_START + mem_mod(centro + direccion * (PAGE_SIZE - MAP_MEM_WIDTH), MAP_MEM_SIZE));

	mmu_mapear_pagina(TASK_VIRT+(4*PAGE_SIZE), pd,
		MAP_START + mem_mod(centro + direccion * MAP_MEM_WIDTH, MAP_MEM_SIZE));

	mmu_mapear_pagina(TASK_VIRT+(5*PAGE_SIZE), pd,
		MAP_START + mem_mod(centro - direccion * MAP_MEM_WIDTH, MAP_MEM_SIZE));
	/* NO MAPPEAR ESTAS PAGINAS AL PRINCIPIO, BORDE DEL MAPA

	mmu_mapear_pagina(TASK_VIRT+(6*PAGE_SIZE), pd,
		MAP_START + (centro - direccion * PAGE_SIZE));

	mmu_mapear_pagina(TASK_VIRT+(7*PAGE_SIZE), pd,
		MAP_START + mem_mod(centro - direccion * (PAGE_SIZE + MAP_MEM_WIDTH), MAP_MEM_SIZE));

	mmu_mapear_pagina(TASK_VIRT+(8*PAGE_SIZE), pd,
		MAP_START + mem_mod(centro - direccion * (PAGE_SIZE - MAP_MEM_WIDTH), MAP_MEM_SIZE);

	*/
	unsigned int pila0 = mmu_proxima_pagina_fisica_libre();
	mmu_mapear_pagina(TASK_STACK_KERNEL, pd, pila0);

	pd_entry* dir_paginas = (pd_entry*) pd;
	pt_entry* tablas = (pt_entry*) ( (unsigned int) dir_paginas[32].page_addr << 12);
	tablas[10].us = 0; // corregir permisos pila0

	return pd;
}

void memcpy(unsigned int src, unsigned int dest, unsigned int len) {
	unsigned int cr3 = rcr3();

	mmu_mapear_pagina(dest, cr3, dest);

	char* srcp = (char*)src;
	char* destp = (char*)dest;
	unsigned int i;
	for (i = 0; i < len; ++i) {
		destp[i] = srcp[i];
	}
	mmu_unmapear_pagina(dest, cr3);
}

void memmov(unsigned int src, unsigned int dest, unsigned int len) {
	unsigned int cr3 = rcr3();

	mmu_mapear_pagina(src, cr3, src);
	mmu_mapear_pagina(dest, cr3, dest);
	char* srcp = (char*)src;
	char* destp = (char*)dest;
	unsigned int i;
	for (i = 0; i < len; ++i) {
		destp[i] = srcp[i];
	}
	mmu_unmapear_pagina(src, cr3);
	mmu_unmapear_pagina(dest, cr3);
}

void mmu_mover_zombi(unsigned char jugador, unsigned char xPos, unsigned char yPos, unsigned char newXPos, unsigned char newYpos) {
	unsigned int centro = yPos * MAP_MEM_WIDTH + xPos * PAGE_SIZE;
	unsigned int nuevoCentro = newYpos * MAP_MEM_WIDTH + newXPos * PAGE_SIZE;
	memmov(MAP_START + centro, MAP_START + nuevoCentro, PAGE_SIZE);
	unsigned int pd = rcr3();
	mmu_desmapear_area_zombi(pd, centro);
	mmu_mapear_area_zombi(pd, jugador, nuevoCentro);
}
