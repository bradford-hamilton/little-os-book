/*
We want GRUB to load the kernel at a memory address larger than
or equal to 0x00100000 (1 megabyte (MB)), because addresses lower
than 1 MB are used by GRUB itself, BIOS and memory-mapped I/O
*/

ENTRY(loader)               /* name of the entry label */
SECTIONS {
  . = 0x00100000;           /* code should be loaded at 1MB */

  .text ALIGN (0x1000) :    /* align at 4KB */
  {
    *(.text)                /* all text sections from all files */
  }

  .rodata ALIGN (0x1000) :  /* align at 4KB */
  {
    *(.rodata*)             /* all read-only data sections from all files */
  }

  .data ALIGN (0x1000) :    /* align at 4KB */
  {
    *(.data)                /* all data sections from all files */
  }

  .bss ALIGN (0x1000) :     /* align at 4KB */
  {
    *(COMMON)               /* all COMMON sections from all files */
    *(.bss)                 /* all bss sections from all files */
  }
}
