#include "calculation.h"
#include <math.h>


inline void add(uint32_t a, uint32_t b, uint32_t c, uint32_t registers[])
{
	registers[a] = registers[b] + registers[c];
}

inline void mult(uint32_t a, uint32_t b, uint32_t c, uint32_t registers[])
{
	registers[a] = registers[b] * registers[c];
}

inline void division(uint32_t a, uint32_t b, uint32_t c, uint32_t registers[])
{
        if (registers[c] == 0) EXIT_FAILURE;
        registers[a] = floor(registers[b] / registers[c]);
}

inline void nand(uint32_t a, uint32_t b, uint32_t c, uint32_t registers[])
{
        registers[a] = ~(registers[b] & registers[c]);
}
