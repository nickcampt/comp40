#ifndef IO_H
#define IO_H

#include <stdint.h>

/*
 * takes an input from stdin and puts it in register c as long as
 * it is the range 0-255
 */
void read(uint32_t c, uint32_t registers[]);

/*
 * writes whatever is in register c to stdout as long as it is
 * in the range of 0-255 
 */
void write(uint32_t c, uint32_t registers[]);

#endif
