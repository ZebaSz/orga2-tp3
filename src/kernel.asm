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
extern print_nombre_grupo
global start
extern resetear_pic
extern habilitar_pic
extern tss_inicializar
extern tss_inicializar_idle
extern game_inicializar
extern sched_inicializar
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
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

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
    call mapa_inicializar
    call print_nombre_grupo
    call game_inicializar
    ; Inicializar el manejador de memoria
    call mmu_inicializar
    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel
    ; Cargar directorio de paginas
    mov eax, 0x27000
    mov cr3, eax
    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ; Inicializar tss
    call tss_inicializar
    ; Inicializar tss de la tarea Idle
    call tss_inicializar_idle
    ; Inicializar el scheduler
    call sched_inicializar
    ; Inicializar la IDT
    call idt_inicializar
    ; Cargar IDT
    lidt [IDT_DESC]
    ; Configurar controlador de interrupciones
    call resetear_pic
    call habilitar_pic
    ; Cargar tarea inicial
    mov ax, 0x64
    ltr ax
    ; Habilitar interrupciones
    sti
    ; Saltar a la primera tarea: Idle
    jmp 0x68:0
    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $


%define SIZE_W                  78
%define SIZE_H                  44

%define C_BG_BLACK              (0x0 << 12)
%define C_BG_BLUE               (0x1 << 12)
%define C_BG_GREEN              (0x2 << 12)
%define C_BG_CYAN               (0x3 << 12)
%define C_BG_RED                (0x4 << 12)
%define C_BG_MAGENTA            (0x5 << 12)
%define C_BG_BROWN              (0x6 << 12)
%define C_BG_LIGHT_GREY         (0x7 << 12)

mapa_inicializar:
    mov ebx, (80 * 2)

    xor ecx, ecx
    mapa_primera_fila:
        mov word [fs:2*ecx], C_BG_BLACK
        inc ecx
        cmp ecx, 80
        jne mapa_primera_fila

    mov ecx, 1
    mapa_col_jug:
        mov eax, ecx
        mul ebx
        mov word [fs:eax], C_BG_RED
        mov word [fs:eax + ebx - 2], C_BG_BLUE

        inc ecx
        cmp ecx, 45
        jne mapa_col_jug

    mov ecx, 1
    mapa_area_juego:
        mov eax, ecx
        mul ebx
        mov edx, 1
        mapa_area_juego_fil:
            mov word [fs:eax + 2 * edx], C_BG_GREEN
            inc edx
            cmp edx, 79
            jne mapa_area_juego_fil

        inc ecx
        cmp ecx, 45
        jne mapa_area_juego

    mov ecx, 45
    mapa_tablero_bg:
        mov eax, ecx
        mul ebx
        xor edx, edx
        mapa_tablero_negro:
            mov word [fs:eax + 2 * edx], C_BG_BLACK
            inc edx
            cmp edx, 80
            jne mapa_tablero_negro

        mov edx, 35
        mapa_tablero_rojo:
            mov word [fs:eax + 2 * edx], C_BG_RED
            inc edx
            cmp edx, 40
            jne mapa_tablero_rojo
        mapa_tablero_azul
            mov word [fs:eax + 2 * edx], C_BG_BLUE
            inc edx
            cmp edx, 45
            jne mapa_tablero_azul

        inc ecx
        cmp ecx, 50
        jne mapa_tablero_bg
    ret

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"