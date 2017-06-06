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


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 30

#define GDT_IDX_NULL_DESC           0

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)
#define GDT_IDX_KERNEL_CODE	8
#define GDT_IDX_KERNEL_DATA	9
#define GDT_IDX_USER_CODE	10
#define GDT_IDX_USER_DATA	11

#define GDT_IDX_VIDEO	15
/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define PAGE_DIR                0x00027000
#define PAGE_TAB                0x00028000
#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */
/* MMU */
#define INICIO_PAGINAS_LIBRES	0x00029000
#define PAGE_SIZE				0x1000

#endif  /* !__DEFINES_H__ */
