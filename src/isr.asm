; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_proximo_indice

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    imprimir_texto_mp isrmensaje, isrmensaje_len, 0x07, 4, 0
    imprimir_texto_mp isrmensaje_%1, isrmensaje_%1_len, 0x07, 5, 0
    mov eax, %1
    jmp $

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

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

global _isr32

_isr32:
    call fin_intr_pic1
    call proximo_reloj
    iret

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33

_isr33:
    pushad
    call fin_intr_pic1
    in al, 0x60
    ;XCHG BX, BX TODO: en al esta el scancode hay que usarlo para imprimir en pantalla
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
        
        
