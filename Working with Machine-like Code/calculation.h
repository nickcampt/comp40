#ifndef CALCULATION_H
#define CALCULATION_H

/*
maybe these can return booleans so we can have in the execution interface
we can have something like, if false exit failure
*/

void add(uint32_t a, uint32_t b, uint32_t c, uint32_t registers[]);


void mult(uint32_t a, uint32_t b, uint32_t c, uint32_t registers[]);


void div(uint32_t a, uint32_t b, uint32_t c, uint32_t registers[]);


void nand(uint32_t a, uint32_t b, uint32_t c, uint32_t registers[]);

#endif
