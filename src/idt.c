/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"
#include "colors.h"

#include "tss.h"

idt_entry idt[255] = { };

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);

        ...
    }
*/


#define IDT_ENTRY(numero)                                                                                        \
    idt[numero].offset_0_15 = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel = (unsigned short) 0x8 << 3;                                                              \
    idt[numero].attr = (unsigned short) 0x8E00;                                                                  \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);



void idt_inicializar() {
    // Excepciones
    IDT_ENTRY(0);
    IDT_ENTRY(1);
    IDT_ENTRY(2);
    IDT_ENTRY(3);
    IDT_ENTRY(4);
    IDT_ENTRY(5);
    IDT_ENTRY(6);
    IDT_ENTRY(7);
    IDT_ENTRY(8);
    IDT_ENTRY(9);
    IDT_ENTRY(10);
    IDT_ENTRY(11);
    IDT_ENTRY(12);
    IDT_ENTRY(13);
    IDT_ENTRY(14);
    IDT_ENTRY(15);
    IDT_ENTRY(16);
    IDT_ENTRY(17);
    IDT_ENTRY(18);
    IDT_ENTRY(19);
    IDT_ENTRY(32);
    IDT_ENTRY(33);
    IDT_ENTRY(102);
    // int 0x66 debe ser accesible desde ring 3
    idt[102].attr = (unsigned short) 0xEE00;
}

void idt_debug_mode(unsigned int* informacion) {
    print_int(tareaActual, 40,44,C_FG_BLACK | C_BG_LIGHT_GREY);

    print("TAREA", 40, 40, C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(*informacion, 8, 40, 42, C_FG_BLACK | C_BG_LIGHT_GREY);
    informacion++;

    print("eip", 0, 16, C_FG_BLACK | C_BG_LIGHT_GREY);
    print_hex(*informacion, 8, 8, 16, C_FG_BLACK | C_BG_LIGHT_GREY);
    informacion++;

    char * segmentos[] = {"cs", "ds", "es", "fs", "gs", "ss"};
    for(int i = 5; i>=0; i--) {
        print(segmentos[i], 20, i*2, C_FG_BLACK | C_BG_LIGHT_GREY);
        print_hex(*informacion, 4, 28, i*2, C_FG_BLACK | C_BG_LIGHT_GREY);
        informacion++;
    }

    char * registros_comunes[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
    for(int i = 7; i>=0; i--) {
        print(registros_comunes[i], 0, i*2, C_FG_BLACK | C_BG_LIGHT_GREY);
        print_hex(*informacion, 8, 8, i*2, C_FG_BLACK | C_BG_LIGHT_GREY);
        informacion++;
    }

    char * stack = "STACK";
    print(stack, 20, 13, C_FG_BLACK | C_BG_LIGHT_GREY);
    for(int i = 1; i < 5; i++) {
        print_hex(*informacion, 8, 20, 15 + i, C_FG_BLACK | C_BG_LIGHT_GREY);
        informacion++;
    }




    
}
