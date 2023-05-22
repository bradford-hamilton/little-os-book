#include "io.h"

void kmain() {}

// ---------------------------------------------------------

char* frame_buf = (char*)0x000B8000;

// Writes a character with the given foreground and background to position i
// in the framebuffer.
void frame_buf_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
  frame_buf[i] = c;
  frame_buf[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

// ---------------------------------------------------------

// I/O ports
#define FRAME_BUF_COMMAND_PORT 0x3D4
#define FRAME_BUF_DATA_PORT 0x3D5

// I/O port commands
#define FRAME_BUF_HIGH_BYTE_COMMAND 14
#define FRAME_BUF_LOW_BYTE_COMMAND 15

// Moves the cursor of the framebuffer to the given position
void frame_buf_move_cursor(unsigned short pos)
{
  outb(FRAME_BUF_COMMAND_PORT, FRAME_BUF_HIGH_BYTE_COMMAND);
  outb(FRAME_BUF_DATA_PORT, ((pos >> 8) & 0x00FF));
  outb(FRAME_BUF_COMMAND_PORT, FRAME_BUF_LOW_BYTE_COMMAND);
  outb(FRAME_BUF_DATA_PORT, pos & 0x00FF);
}

// ---------------------------------------------------------

// All the I/O ports are calculated relative to the data port. This is because
// all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
// order, but they start at different values.
#define SERIAL_COM1_BASE 0x3F8
#define SERIAL_DATA_PORT(base) (base)
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base) (base + 5)

// Tells the serial port to expect first the highest 8 bits on the data port,
// then the lowest 8 bits will follow
#define SERIAL_LINE_ENABLE_DLAB 0x80

// Sets the speed of the data being sent. The default speed of a serial
// port is 115200 bits/s. The argument is a divisor of that number, hence
// the resulting speed becomes (115200 / divisor) bits/s.
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
  outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
  outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
  outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

// Configures the line of the given serial port. The port is set to have a
// data length of 8 bits, no parity bits, one stop bit and break control
// disabled.
void serial_configure_line(unsigned short com)
{
  /*
      d	   - Enables (d = 1) or disables (d = 0) DLAB
      b	   - If break control is enabled (b = 1) or disabled (b = 0)
      prty - The number of parity bits to use
      s 	 - The number of stop bits to use (s = 0 equals 1, s = 1 equals 1.5 or 2)
      dl   - Describes the length of the data
      -----------------------------------------------------------------------------
      Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
      Content: | d | b | prty  | s | dl  |
      Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
  */
  outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

// Checks whether the transmit FIFO queue is empty or not for the given COM port.
int serial_is_transmit_fifo_empty(unsigned int com)
{
  // 0x20 = 0010 0000
  return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

