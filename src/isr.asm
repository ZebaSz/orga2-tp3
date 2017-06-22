; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

debug_flag:             db 0x0

%define VIDEO           0xB8000

%define DEBUG_W         30
%define DEBUG_H         36

debug_map_buffer:       resw (DEBUG_H * DEBUG_W)

%define debug_off       00b
%define debug_on        01b
%define debug_shown     10b

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proximo_indice
extern sched_toggle_debug
extern sched_lanzar_tarea
extern sched_matar_tarea_actual

;; Juego
extern game_jugador_mover
extern game_lanzar_zombi
extern game_move_current_zombi


extern game_jugador_tecla

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    call matar_tarea
    call fin_intr_pic1

    mov [sched_tarea_selector], word 0x68
    jmp far [sched_tarea_offset]

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler

isrnumero:           dd 0x00000000
isrClock:            db '|/-\'

isrmensaje:          db 'Exception! Su excepcion es:'
isrmensaje_len:      equ $ - isrmensaje

isrmensaje_0:        db 'Divide-by-zero (0)'
isrmensaje_0_len:    equ $ - isrmensaje_0

isrmensaje_13:       db 'General Protection Fault (13)'
isrmensaje_13_len:   equ $ - isrmensaje_13

isrmensaje_1:        db 'reserved (1)'
isrmensaje_1_len:    equ $ - isrmensaje_1

isrmensaje_2:        db 'NMI Interrupt (2)'
isrmensaje_2_len:    equ $ - isrmensaje_2

isrmensaje_3:        db 'Breakpoint (3)'
isrmensaje_3_len:    equ $ - isrmensaje_3

isrmensaje_4:        db 'Overflow (4)'
isrmensaje_4_len:    equ $ - isrmensaje_4

isrmensaje_5:        db 'BOUND Range Exceeded (5)'
isrmensaje_5_len:    equ $ - isrmensaje_5

isrmensaje_6:        db 'Invalid Opcode (Undefined Opcode) (6)'
isrmensaje_6_len:    equ $ - isrmensaje_6

isrmensaje_7:        db 'Device Not Available (No Math Coprocessor) (7)'
isrmensaje_7_len:    equ $ - isrmensaje_7

isrmensaje_8:        db 'Double Fault (8)'
isrmensaje_8_len:    equ $ - isrmensaje_8

isrmensaje_9:        db 'Coprocessor Segment Overrun (9)'
isrmensaje_9_len:    equ $ - isrmensaje_9

isrmensaje_10:       db 'Invalid TSS (10)'
isrmensaje_10_len:   equ $ - isrmensaje_10

isrmensaje_11:       db 'Segment not present (11)'
isrmensaje_11_len:   equ $ - isrmensaje_11

isrmensaje_12:       db 'Stack-Segment Fault (12)'
isrmensaje_12_len:   equ $ - isrmensaje_12

isrmensaje_14:       db 'Page Fault (14)'
isrmensaje_14_len:   equ $ - isrmensaje_14

isrmensaje_15:       db 'reserved (15)'
isrmensaje_15_len:   equ $ - isrmensaje_15

isrmensaje_16:       db 'x87 FPU Foating-Point Error (Math Fault) (16)'
isrmensaje_16_len:   equ $ - isrmensaje_16

isrmensaje_17:       db 'Alignment Check (17)'
isrmensaje_17_len:   equ $ - isrmensaje_17

isrmensaje_18:       db 'Machine Check (18)'
isrmensaje_18_len:   equ $ - isrmensaje_18

isrmensaje_19:        db 'SIMD Floating-Point Exception (19)'
isrmensaje_19_len:    equ $ - isrmensaje_19

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19

global _isr102

_isr102:
    mov eax, 0x42
    iret


;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

global _isr32

_isr32:
    pushad
    call proximo_reloj

    call sched_proximo_indice

    cmp ax, 0
    je .nojump
        mov [sched_tarea_selector], ax
        call fin_intr_pic1
        jmp far [sched_tarea_offset]
        jmp .end

    .nojump:
    call fin_intr_pic1

    .end:
    popad
    iret

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

%define key_debug       0x15 ; Y

%define key_a_up        0x11 ; w
%define key_a_dn        0x1f ; s
%define key_a_lf        0x20 ; d
%define key_a_rt        0x1e ; a
%define key_a_sh        0x2a ; LShift

%define key_b_up        0x17 ; i
%define key_b_dn        0x25 ; k
%define key_b_lf        0x26 ; l
%define key_b_rt        0x24 ; j
%define key_b_sh        0x36 ; RShift

global _isr33

_isr33:
    pushad
    call fin_intr_pic1

    in al, 0x60
    cmp al, key_debug
    je .toggle_debug ;  MODO DEBUG DESHABILITADO

    cmp al, key_a_up
    je .move_jugador
    cmp al, key_a_dn
    je .move_jugador
    cmp al, key_a_lf
    je .move_jugador
    cmp al, key_a_rt
    je .move_jugador
    cmp al, key_b_up
    je .move_jugador
    cmp al, key_b_dn
    je .move_jugador
    cmp al, key_b_lf
    je .move_jugador
    cmp al, key_b_rt
    je .move_jugador

    cmp al, key_a_sh
    je .newzombie_a
    cmp al, key_b_sh
    je .newzombie_b

    jmp .keyboard_end

    .toggle_debug:
        xchg bx, bx

        mov al, [debug_flag]
        test al, (debug_shown | debug_on)
        jz .enable_debug
        test al, debug_shown
        jz .keyboard_end
        ; disable_debug
        ; copiar buffer de video viejo
        mov byte [debug_flag], debug_off
        call sched_toggle_debug
        jmp .keyboard_end

        .enable_debug:
        mov byte [debug_flag], debug_on
        call sched_toggle_debug
        jmp .keyboard_end

    .move_jugador:
        mov al, al
        push eax
        call game_jugador_tecla
        add esp, 4
        jmp .keyboard_end

    .newzombie_a:
        mov ebx, 0
        push ebx
        call game_lanzar_zombi
        pop ebx
        jmp .keyboard_end

    .newzombie_b:
        mov ebx, 1
        push ebx
        call game_lanzar_zombi
        pop ebx
        jmp .keyboard_end

    .keyboard_end:
    popad
    iret
;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

%define IZQ 0xAAA
%define DER 0x441
%define ADE 0x83D
%define ATR 0x732


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
        pushad
        inc DWORD [isrnumero]
        mov ebx, [isrnumero]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrnumero], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                imprimir_texto_mp ebx, 1, 0x0f, 49, 79
                popad
        ret
        
        
matar_tarea:
    ; MATAR TAREA AQUI
    call sched_matar_tarea_actual

    ; mostrar cartel debug
    mov al, [debug_flag]
    test al, debug_on
    jz .tarea_muerta
    ;     ; copiamos mapa viejo a buffer
    ;     mov esi, 80 * 2 ; ancho de una fila de video
    ;     mov edx, VIDEO + (2 * (80 * 7 + 25)) ; apuntamos a la esquina de la memoria de video
    ;     xor ebx, ebx
    ;     .copiar_fila
    ;         xor ecx, ecx
    ;         mov eax, edx
    ;         .copiar_punto
    ;             mov edi, [eax + 2 * ecx]
    ;             mov [eax + 2 * ecx], 

    ;     ; dibujamos dialogo debug
    ;     mov eax, VIDEO + (2 * (80 * 7 + 25)) ; apuntamos a la esquina de la memoria de video
    ;     xor ebx, ebx
    ;     .dibujar_fila
    ;         xor ecx, ecx

    mov byte [debug_flag], debug_shown

    .tarea_muerta:
    ret