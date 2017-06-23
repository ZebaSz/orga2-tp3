/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"

tss tss_inicial;
tss tss_idle;

tss tss_zombisA[CANT_ZOMBIS];
tss tss_zombisB[CANT_ZOMBIS];

void tss_inicializar() {
	unsigned int base = (unsigned int) &tss_inicial;
    tss_inicializar_gdt_entry(GDT_IDX_TSS_INIT, base);
}

void tss_inicializar_zombi(unsigned char jugador, unsigned char yPos, unsigned char tarea, unsigned int gdt_entry) {
    tss tss_zombi;
    unsigned int base = (unsigned int) &tss_zombi;
    tss_inicializar_gdt_entry(gdt_entry, base); 

    tss_zombi.esp = TASK_VIRT | 0xFFF; // OFFSET de la task la pila empieza donde termina la task
    tss_zombi.ebp = TASK_VIRT | 0xFFF;
    tss_zombi.eflags = 0x202;
    tss_zombi.eip = TASK_VIRT;
    tss_zombi.cr3 = mmu_inicializar_dir_zombi(jugador, yPos, tarea);
    tss_zombi.esp0 = mmu_proxima_pagina_fisica_libre();
    tss_zombi.ss0 = GDT_IDX_KERNEL_DATA;
    tss_zombi.iomap = 0xFFFF;
    tss_zombi.cs = GDT_OFF_USER_CODE; // permisos usuario
    tss_zombi.es = GDT_OFF_USER_DATA;
    tss_zombi.ss = GDT_OFF_USER_DATA;
    tss_zombi.ds = GDT_OFF_USER_DATA;
    tss_zombi.fs = GDT_OFF_USER_DATA;
    tss_zombi.gs = GDT_OFF_USER_DATA;
}

void tss_inicializar_idle() {
	unsigned int base = (unsigned int) &tss_idle;
    tss_inicializar_gdt_entry(GDT_IDX_TSS_IDLE, base);

    tss_idle.ptl = 0x0000;
    tss_idle.unused0 = 0x0000;
    tss_idle.esp0 = 0x00000000;
    tss_idle.ss0 = 0x0000;
    tss_idle.unused1 = 0x0000;
    tss_idle.esp1 = 0x00000000;
    tss_idle.ss1 = 0x0000;
    tss_idle.unused2 = 0x0000;
    tss_idle.esp2 = 0x00000000;
    tss_idle.ss2 = 0x0000;
    tss_idle.unused3 = 0x0000;
    tss_idle.cr3 = KERNEL_PAGE_DIR;
    tss_idle.eip = TASK_IDLE;
    tss_idle.eflags = 0x00000202;
    tss_idle.eax = 0x00000000;
    tss_idle.ecx = 0x00000000;
    tss_idle.edx = 0x00000000;
    tss_idle.ebx = 0x00000000;
    tss_idle.esp = KERNEL_STACK_BASE;
    tss_idle.ebp = KERNEL_STACK_BASE;
    tss_idle.esi = 0x00000000;
    tss_idle.edi = 0x00000000;
    tss_idle.es = GDT_OFF_KERNEL_DATA;
    tss_idle.unused4 = 0x0000;
    tss_idle.cs = GDT_OFF_KERNEL_CODE;
    tss_idle.unused5 = 0x0000;
    tss_idle.ss = GDT_OFF_KERNEL_DATA;
    tss_idle.unused6 = 0x0000;
    tss_idle.ds = GDT_OFF_KERNEL_DATA;
    tss_idle.unused7 = 0x0000;
    tss_idle.fs = GDT_OFF_KERNEL_DATA;
    tss_idle.unused8 = 0x0000;
    tss_idle.gs = GDT_OFF_KERNEL_DATA;
    tss_idle.unused9 = 0x0000;
    tss_idle.ldt = 0x0000;
    tss_idle.unused10 = 0x0000;
    tss_idle.dtrap = 0x0000;
    tss_idle.iomap = 0xFFFF;
}

void tss_inicializar_gdt_entry(unsigned int index, unsigned int base) {
    gdt[index].limit_0_15 = 0x0068;
    gdt[index].base_0_15 = base & 0xFFFF;
    gdt[index].base_23_16 = (base >> 16) & 0xFF;
    gdt[index].type = 0x9;
    gdt[index].s = 0x0;
    gdt[index].dpl = 0x0;
    gdt[index].p = 0x1;
    gdt[index].limit_16_19 = 0x0;
    gdt[index].avl = 0x1;
    gdt[index].l = 0x0;
    gdt[index].db = 0x0;
    gdt[index].g = 0x0;
    gdt[index].base_31_24 = (base >> 24);
}
