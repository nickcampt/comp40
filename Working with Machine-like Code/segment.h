#ifndef SEGMENT_H
#define SEGMENT_H

#include <stdint.h>

/*
do these work the way we expect them to based off of the way we are using
registers etc. in our calculations interface? do we have to edit this
interface based off of what we know now?
*/


void map_segment(Seq_T segment_ID, int size, Seq_T removed_segments,
        uint32_t registers[]);

void unmap_segment(Seq_T segment_ID, int size, Seq_T removed_segments);

void new_index(Seq_T removed_segments);

void store_in_memory(Seq_T segment_ID, uint32_t a, uint32_t b, uint32_t c);

void load_from_memory(Seq_T segment_ID, uint32_t a, uint32_t b, uint32_t c);

void conditional_move(uint32_t a, uint32_t b, uint32_t c);


#endif
