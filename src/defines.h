/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_ZOMBIS             8

#define SIZE_W                  78
#define SIZE_H                  44

#define JUG_A                   0
#define JUG_B                   1

/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 30

#define GDT_IDX_NULL_DESC       0
#define GDT_IDX_KERNEL_CODE     8
#define GDT_IDX_KERNEL_DATA     9
#define GDT_IDX_USER_CODE       10
#define GDT_IDX_USER_DATA       11

#define GDT_IDX_TSS_INIT        12
#define GDT_IDX_TSS_IDLE        13

#define GDT_IDX_VIDEO           15

#define GDT_IDX_TSS_A1          16
#define GDT_IDX_TSS_A2          17
#define GDT_IDX_TSS_A3          18
#define GDT_IDX_TSS_A4          19
#define GDT_IDX_TSS_A5          20
#define GDT_IDX_TSS_A6          21
#define GDT_IDX_TSS_A7          22
#define GDT_IDX_TSS_A8          23
#define GDT_IDX_TSS_B1          24
#define GDT_IDX_TSS_B2          25
#define GDT_IDX_TSS_B3          26
#define GDT_IDX_TSS_B4          27
#define GDT_IDX_TSS_B5          28
#define GDT_IDX_TSS_B6          29
#define GDT_IDX_TSS_B7          30
#define GDT_IDX_TSS_B8          31

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC       (GDT_IDX_NULL_DESC      << 3)
#define GDT_OFF_KERNEL_CODE     (GDT_IDX_KERNEL_CODE    << 3)
#define GDT_OFF_KERNEL_DATA     (GDT_IDX_KERNEL_DATA    << 3)
#define GDT_OFF_USER_CODE       (GDT_IDX_USER_CODE      << 3)
#define GDT_OFF_USER_DATA       (GDT_IDX_USER_DATA      << 3)

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define KERNEL_STACK_BASE       0x00027000
#define KERNEL_PAGE_DIR         0x00027000
#define KERNEL_PAGE_TAB         0x00028000
#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */

/* MMU */
/* -------------------------------------------------------------------------- */
#define INICIO_PAGINAS_LIBRES	0x00029000
#define PAGE_SIZE				0x1000

/* TAREAS */
/* -------------------------------------------------------------------------- */
#define TASK_AG                 0x10000
#define TASK_AM                 0x11000
#define TASK_AC                 0x12000
#define TASK_BG                 0x13000
#define TASK_BM                 0x14000
#define TASK_BC                 0x15000
#define TASK_IDLE               0x16000

#define TASK_VIRT               0x08000000

/* MAPA */
/* -------------------------------------------------------------------------- */
#define MAP_START               0x400000
#define MAP_WIDTH               78
#define MAP_HEIGHT              44

#define MAP_MEM_WIDTH           (MAP_WIDTH * PAGE_SIZE)
#define MAP_MEM_SIZE            (MAP_WIDTH * MAP_HEIGHT * PAGE_SIZE)


#endif  /* !__DEFINES_H__ */
