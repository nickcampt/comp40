#ifndef CALCULATION_H
#define CALCULATION_H

#include <stdint.h>
#include <stdlib.h>

/* adds the values of registers b and c and puts the result in reg A */
void add(uint32_t a, uint32_t b, uint32_t c, uint32_t registers[]);


/* multiplies the values of registers b and c and puts the result in reg A */
void mult(uint32_t a, uint32_t b, uint32_t c, uint32_t registers[]);


/* divides the values of registers b and c and puts the result in reg A */
void division(uint32_t a, uint32_t b, uint32_t c, uint32_t registers[]);

/* NAND's the values of registers b and c and puts the result in reg A */
void nand(uint32_t a, uint32_t b, uint32_t c, uint32_t registers[]);

#endif
