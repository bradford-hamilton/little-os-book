#ifndef IO_H
#define IO_H

// Sends the given data to the given I/O port. Defined in io.s
void outb(unsigned short port, unsigned short data);

// Read a byte from an I/O port.
unsigned char inb(unsigned short port);

#endif // IO_H
