#ifndef SEGMENT_H
#define SEGMENT_H
#include <stdio.h>
#include <stdint.h>

typedef struct memory *Memory;

/*
 * allocated memory for the segments, and builds the words from the
 * given instruction file, butting them into the prog_data array.
 */
Memory initialize_memory(FILE *fp);

/*
 * map segment makes a new segment with a number of words equal to the value
 * at register c This segment is given an identifier and this identifier is
 * placed in register b.
 */
void map_segment(Memory mem, uint32_t regCIndex, uint32_t regBIndex,
                                                        uint32_t registers[]);

/*
 * frees a given segment
 */
void unmap_segment(Memory mem, uint32_t regCIndex, uint32_t registers[]);

/*
 * returns an identifer for a segment
 */
int new_index(Memory mem);

/*
 * places a value into a segment, desginated by register a
 * index b is set to the value at register c
 */
void store_in_memory(Memory mem,uint32_t a, uint32_t b, uint32_t c,
                                                        uint32_t registers[]);

void load_from_memory( Memory mem, uint32_t a, uint32_t b, uint32_t c,
                                                        uint32_t registers[]);

void load_program(Memory mem, unsigned *prog_counter, uint32_t b, uint32_t c,
                                                        uint32_t registers[]);

uint32_t *get_prog_data(Memory mem);

void free_entire_memory(Memory mem);

int prog_length(Memory mem);

void print_memory_info(Memory mem);




#endif
