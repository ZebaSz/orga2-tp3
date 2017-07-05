/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"

typedef struct pte_s {
    unsigned char   p:1;
    unsigned char   rw:1;
    unsigned char   us:1;
    unsigned char   pwt:1;
    unsigned char   pcd:1;
    unsigned char   a:1;
    unsigned char   d:1;
    unsigned char   pat:1;
    unsigned char   g:1;
    unsigned char   avl:3;
    unsigned int    page_addr:20;
} pt_entry;

typedef struct pde_s {
    unsigned char   p:1;
    unsigned char   rw:1;
    unsigned char   us:1;
    unsigned char   pwt:1;
    unsigned char   pcd:1;
    unsigned char   a:1;
    unsigned char   ign:1;
    unsigned char   ps:1;
    unsigned char   g:1;
    unsigned char   avl:3;
    unsigned int    page_addr:20;
} pd_entry;


void mmu_inicializar();

unsigned int mmu_proxima_pagina_fisica_libre();

void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica);

void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3);

unsigned int mmu_inicializar_dir_zombi(unsigned char jugador, unsigned char yPos, unsigned char tarea);

void mmu_mover_zombi(unsigned char jugador, unsigned char xPos, unsigned char yPos, unsigned short dir);

void mmu_desmapeo_zombi(unsigned char jugador, unsigned char xPos, unsigned char yPos, unsigned char tarea);

void memcpy(unsigned int src, unsigned int dest, unsigned int len);

#endif	/* !__MMU_H__ */




