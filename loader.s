global loader               ; entry symbol for ELF

MAGIC_NUMBER equ 0x1BADB002 ; define magic number constant
FLAGS equ 0x0               ; multiboot flags
CHECKSUM equ -MAGIC_NUMBER  ; calculate checksum
; magic number + checksum + flags should equal 0

; We could point esp to a random area in memory since, so far, the only thing in
; the memory is GRUB, BIOS, the OS kernel and some memory-mapped I/O. This is not
; a good idea - we don’t know how much memory is available or if the area esp
; would point to is used by something else. A better idea is to reserve a piece
; of uninitialized memory in the bss section in the ELF file of the kernel. It
; is better to use the bss section instead of the data section to reduce the size
; of the OS executable. Since GRUB understands ELF, GRUB will allocate any memory
; reserved in the bss section when loading the OS.
KERNEL_STACK_SIZE equ 4096  ; stack size in bytes

section .bss
align 4                     ; align at 4 bytes
kernel_stack:               ; label pointing to beginning of memory
  resb KERNEL_STACK_SIZE    ; reserve stack for the kernel

; The stack pointer is then set up by pointing esp to the end of the kernel_stack memory:
mov esp, kernel_stack + KERNEL_STACK_SIZE

section .text               ; start of code section
align 4                     ; code must be 4 byte aligned
  dd MAGIC_NUMBER           ; write magic number to the machine code
  dd FLAGS                  ; write flags to the machine code
  dd CHECKSUM               ; write checksum to the machine code

loader:                     ; loader label (defined as entry point in linker script)
  mov eax, 0xCAFEBABE       ; place number in eax
.loop:
  jmp .loop                 ; loop forever
