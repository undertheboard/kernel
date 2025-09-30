; Multiboot header for GRUB bootloader
; This file defines the boot sector for our x86_64 OS

; Multiboot constants
%define ALIGN       1
%define MEMINFO     2
%define FLAGS       (ALIGN | MEMINFO)
%define MAGIC       0x1BADB002
%define CHECKSUM    -(MAGIC + FLAGS)

; Multiboot header
section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; Stack for initial kernel setup
section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KiB stack
stack_top:

; Entry point for the kernel
section .text
global _start
_start:
    ; Set up the stack pointer
    mov esp, stack_top
    
    ; Call the kernel main function
    extern kernel_main
    call kernel_main
    
    ; If kernel_main returns, halt the system
    cli                 ; Clear interrupts
.hang:
    hlt                 ; Halt the processor
    jmp .hang           ; Jump back if we somehow continue