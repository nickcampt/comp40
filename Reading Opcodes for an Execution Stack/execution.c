#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <except.h>
#include "segment.h"
#include "calculation.h"
#include "IO.h"

const int OPSHIFT = 28;
const int REGSHIFT = 29;
const int VALSHIFT = 7;

bool run = true;


FILE *open_file(char *filename)
{
        FILE *fp = fopen(filename, "rb");
        if (fp == NULL){
                Except_T BAD_PTR = {"Unable to open File"};
                RAISE(BAD_PTR);
        }

        return fp;
}

void initialize_registers(uint32_t registers[])
{
        for (int i=0; i<8; i++)
                registers[i] = 0;
}

static inline void conditional_move(uint32_t registers[], uint32_t a,
                                                uint32_t b, uint32_t c)
{
        if (registers[c] != 0)
                registers[a] = registers[b];
}

static inline void load_val(uint32_t word, uint32_t registers[])
{
        uint32_t toLoad = (word << VALSHIFT) >> VALSHIFT;
        uint32_t aVal = (word << 4) >> REGSHIFT;
        registers[aVal] = toLoad;
}

static inline int interpret(Memory mem, uint32_t registers[],
        uint32_t instruction, unsigned *prog_counter)
{
        int prog_c_increase = 1;
        Except_T INVALID_INST = {"\"Invalid instruction\""};
        uint32_t a, b, c, opcode;

        opcode = instruction >> OPSHIFT;
        c = (instruction << 29) >> REGSHIFT;
        b = (instruction << 26) >> REGSHIFT;
        a = (instruction << 23) >> REGSHIFT;

        switch (opcode)
        {
                case 0:
                        conditional_move(registers,a,b,c);
                        break;
                case 1:
                        load_from_memory(mem,a,b,c,registers);
                        break;
                case 2:
                        store_in_memory(mem,a,b,c,registers);
                        break;
                case 3:
                        add(a, b, c, registers);
                        break;
                case 4:
                        mult(a, b, c, registers);
                        break;
                case 5:
                        division(a, b, c, registers);
                        break;
                case 6:
                        nand(a, b, c, registers);
                        break;
                case 7:
                        run = false;
                        break;
                case 8:
                        map_segment(mem,c,b,registers);
                        break;
                case 9:
                        unmap_segment(mem,c,registers);
                        break;
                case 10:
                        write(c, registers);
                        break;
                case 11:
                        read(c, registers);
                        break;
                case 12:
                        load_program(mem, prog_counter, b, c,registers);
                        prog_c_increase = 0;
                        break;
                case 13:
                        load_val(instruction, registers);
                        break;
                default:
                       RAISE(INVALID_INST);
        }
        return prog_c_increase;

}

void run_machine(char *filename)
{
        uint32_t registers[8];
        unsigned prog_counter = 0;
        initialize_registers(registers);
        FILE *fp = open_file(filename);
        Memory mem = initialize_memory(fp);

        uint32_t instruction;
        int inc;

        while(run)
        {
                uint32_t *data = get_prog_data(mem);
                instruction = data[prog_counter];
                inc = interpret(mem,registers,instruction,&prog_counter);
                if (inc == 1)
                        prog_counter++;
        }

        free_entire_memory(mem);
        fclose(fp);
}
