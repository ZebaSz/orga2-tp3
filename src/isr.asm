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

    mov eax, %1
    mov ecx, 10
    xor bx, bx

    %%char_ascii:
        xor edx, edx
        div ecx
        add edx, '0'
        mov dh, 0x07
        push dx
        inc bx
        test eax, eax
        jnz %%char_ascii

    mov cx, bx
    mov ebx, 0xb8282 + (isrmensaje_len * 2)
    %%print_exception_code
        pop dx
        mov [ebx], dx
        add ebx, 2
        loop %%print_exception_code

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

isrGpf:              db 'General Protection Fault (13)'
isrGpf_len:          equ $ - isrGpf

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0

ISR 13

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

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
        
        
