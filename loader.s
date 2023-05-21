global loader               ; entry symbol for ELF

MAGIC_NUMBER equ 0x1BADB002 ; define magic number constant
FLAGS equ 0x0               ; multiboot flags
CHECKSUM equ -MAGIC_NUMBER  ; calculate checksum
                            ; (magic number + checksum + flags should equal 0)

section .text               ; start of code section
align 4                     ; code must be 4 byte aligned
  dd MAGIC_NUMBER           ; write magic number to the machine code
  dd FLAGS                  ; write flags to the machine code
  dd CHECKSUM               ; write checksum to the machine code

loader:                     ; loader label (defined as entry point in linker script)
  mov eax, 0xCAFEBABE       ; place number in eax
.loop:
  jmp .loop                 ; loop forever
