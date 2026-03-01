#ifndef SERIAL_STDIO_HPP
#define SERIAL_STDIO_HPP

#include <stdio.h>

void serial_setup();
int serial_get_char(FILE *stream);
int serial_put_char(char c, FILE *stream);

#endif
