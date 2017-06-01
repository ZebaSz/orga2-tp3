; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

extern GDT_DESC
extern IDT_DESC
extern idt_inicializar
extern mmu_inicializar
extern mmu_inicializar_dir_kernel
global start
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    ;mov ax, 1112h
    ;int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0

    ; Habilitar A20
    call checkear_A20
    cmp ax, 1
    je A20_habilitado
    call habilitar_A20

A20_habilitado:
    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; Saltar a modo protegido
    jmp 0x40:modoprotegido

BITS 32
    modoprotegido:
    ; Establecer selectores de segmentos
    xor eax, eax
    mov ax, 1001000b ; index = 9 | gdt = 0 | rpl = 0 
    mov ds, ax ; kernel data segment
    mov es, ax
    mov gs, ax

    mov ax, 1111000b ; index = 15 | gdt = 0 | rpl = 0 
    mov fs, ax ; video segment
    ; Establecer la base de la pila
    mov ebp, 0x27000
    
    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

    ; Inicializar pantalla
    
    ; Inicializar el manejador de memoria
 
    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel
    
    ; Cargar directorio de paginas


    ; Habilitar paginacion
    mov eax, 0x27000 ;PAGE_DIRECTORY
    mov cr3, eax
    
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    
    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT
    call idt_inicializar
    
    ; Cargar IDT
    lidt [IDT_DESC]

    ; ERROR DE PRUEBA
    mov dword [fs:0xFFFFFF], 0xFA50
 
    ; Configurar controlador de interrupciones

    ; Cargar tarea inicial

    ; Habilitar interrupciones
    sti

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
