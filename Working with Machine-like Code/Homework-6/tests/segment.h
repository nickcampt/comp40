#ifndef SEGMENT_H
#define SEGMENT_H

#include <stdint.h>

/*
do these work the way we expect them to based off of the way we are using
registers etc. in our calculations interface? do we have to edit this
interface based off of what we know now?
*/


void map_segment(uint32_t regCIndex, uint32_t regBIndex, uint32_t registers[]);

void unmap_segment(uint32_t regCIndex);

void new_index();

void store_in_memory(uint32_t a, uint32_t b, uint32_t c, uint32_t registers[]);

void load_from_memory(uint32_t a, uint32_t b, uint32_t c,
         uint32_t registers[]);

void conditional_move(uint32_t a, uint32_t b, uint32_t c,
        uint32_t registers[]);

void print_removed_segments();

void load_program(uint32_t b);

#endif
