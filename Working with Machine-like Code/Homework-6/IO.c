#include "IO.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <except.h>

inline void read(uint32_t c, uint32_t registers[])
{
        registers[c] = getchar();
}

inline void write(uint32_t c, uint32_t registers[])
{
        printf("%c", (char)registers[c]);
}
