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

	gdt[GDT_IDX_TSS_INIT].limit_0_15 = 0x0068;
	gdt[GDT_IDX_TSS_INIT].base_0_15 = base & 0xFFFF;
	gdt[GDT_IDX_TSS_INIT].base_23_16 = (base >> 16) & 0xFF;
	gdt[GDT_IDX_TSS_INIT].type = 0x9;
	gdt[GDT_IDX_TSS_INIT].s = 0x0;
	gdt[GDT_IDX_TSS_INIT].dpl = 0x0;
	gdt[GDT_IDX_TSS_INIT].p = 0x1;
	gdt[GDT_IDX_TSS_INIT].limit_16_19 = 0x0;
	gdt[GDT_IDX_TSS_INIT].avl = 0x1;
	gdt[GDT_IDX_TSS_INIT].l = 0x0;
	gdt[GDT_IDX_TSS_INIT].db = 0x0;
	gdt[GDT_IDX_TSS_INIT].g = 0x0;
	gdt[GDT_IDX_TSS_INIT].base_31_24 = (base >> 24);
}

void tss_inicializar_idle() {
	unsigned int base = (unsigned int) &tss_idle;

	gdt[GDT_IDX_TSS_IDLE].limit_0_15 = 0x0068;
	gdt[GDT_IDX_TSS_IDLE].base_0_15 = base & 0xFFFF;
	gdt[GDT_IDX_TSS_IDLE].base_23_16 = (base >> 16) & 0xFF;
	gdt[GDT_IDX_TSS_IDLE].type = 0x9;
	gdt[GDT_IDX_TSS_IDLE].s = 0x0;
	gdt[GDT_IDX_TSS_IDLE].dpl = 0x0;
	gdt[GDT_IDX_TSS_IDLE].p = 0x1;
	gdt[GDT_IDX_TSS_IDLE].limit_16_19 = 0x0;
	gdt[GDT_IDX_TSS_IDLE].avl = 0x1;
	gdt[GDT_IDX_TSS_IDLE].l = 0x0;
	gdt[GDT_IDX_TSS_IDLE].db = 0x0;
	gdt[GDT_IDX_TSS_IDLE].g = 0x0;
	gdt[GDT_IDX_TSS_IDLE].base_31_24 = (base >> 24);

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

