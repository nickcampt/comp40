/*
 * umlab.c
 *
 * The functions defined in this lab should be linked against umlabwrite.c
 * to produce a unit test writing program. Any additional functions and unit
 * tests written for the lab go here.
 */

#include <stdint.h>
#include <stdio.h>
#include <bitpack.h>
#include <assert.h>
#include <seq.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        Um_instruction inst = (uint32_t)0;
        inst = Bitpack_newu(inst, 4, 28, op);
        inst = Bitpack_newu(inst, 3, 6, ra);
        inst = Bitpack_newu(inst, 3, 3, rb);
        inst = Bitpack_newu(inst, 3, 0, rc);

        return inst;
}


Um_instruction loadval(unsigned ra, unsigned val)
{
        Um_instruction inst = (uint32_t)0;
        inst = Bitpack_newu(inst, 4,28, 13);
        inst = Bitpack_newu(inst, 3,25, ra);
        inst = Bitpack_newu(inst, 25,0, val);

        return inst;
}


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void)
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction add(Um_register a, Um_register b, Um_register c)
{
        return three_register(ADD, a, b, c);
}
static inline Um_instruction mult(Um_register a, Um_register b, Um_register c)
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction div(Um_register a, Um_register b, Um_register c)
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, Um_register c)
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction map(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction unmap(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction load_p(Um_register b, Um_register c)
{
        return three_register(LOADP, 0, b, c);
}

static inline Um_instruction seg_load(Um_register a,
        Um_register b, Um_register c)
{
        return three_register(SLOAD, a, b, c);
}

static inline Um_instruction seg_store(Um_register a,
        Um_register b, Um_register c)
{
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction c_move(Um_register a,
         Um_register b, Um_register c)
{
        return three_register(CMOV, a, b, c);
}

/* Functions for working with streams */

static inline void emit(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

extern void Um_write_sequence(FILE *output, Seq_T stream)
{
        for(int i=0; i < Seq_length(stream); ++i)
        {
                Um_instruction inst = (Um_instruction)(uintptr_t)Seq_get
                                                                    (stream,i);
                putc(Bitpack_getu(inst,8,24), output);
                putc(Bitpack_getu(inst,8,16), output);
                putc(Bitpack_getu(inst,8,8), output);
                putc(Bitpack_getu(inst,8,0), output);
        }
}




/* Unit tests for the UM */
void emit_print_test(Seq_T stream)
{
        emit(stream, loadval(r1,48));
        emit(stream, loadval(r2,6));
        emit(stream, add(r3,r1,r2));
        emit(stream, output(r3));
        emit(stream, halt());

}
void emit_add_test(Seq_T stream)
{
        emit(stream, loadval(r3,50));
        emit(stream, loadval(r2,50));
        emit(stream, add(r1,r2,r3));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_halt_test(Seq_T stream)
{
        emit(stream, halt());
}

void emit_verbose_halt_test(Seq_T stream)
{
        emit(stream, halt());
        emit(stream, loadval(r1, 'B'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'a'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, 'd'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '!'));
        emit(stream, output(r1));
        emit(stream, loadval(r1, '\n'));
        emit(stream, output(r1));
}

void emit_mult_test(Seq_T stream)
{
        emit(stream, loadval(r2,5));
        emit(stream, loadval(r3,10));
        emit(stream, mult(r1, r2, r3));
        emit(stream, output(r1));
        emit(stream, halt());

}

void emit_div_test(Seq_T stream)
{
        emit(stream, loadval(r3,2));
        emit(stream, loadval(r2,100));
        emit(stream, div(r1,r2,r3));
        emit(stream, output(r1));
        emit(stream, halt());
}

void emit_nand_test(Seq_T stream)
{
        emit(stream, loadval(r2, 10));
        emit(stream, loadval(r3, 21));
        emit(stream, nand(r1, r3, r2));
        emit(stream, loadval(r4, 10));
        emit(stream, loadval(r5, 21));
        emit(stream, nand(r6, r4, r5));
        emit(stream, nand(r7, r6, r1));
        emit(stream, output(r7));
        emit(stream, halt());
}

void div_by_zero(Seq_T stream)
{
        emit(stream, loadval(r2, 5));
        emit(stream, loadval(r3, 2));
        emit(stream, mult(r1, r2, r3));
        emit(stream, loadval(r5, 0));
        emit(stream, loadval(r7, 2));
        emit(stream, div(r1, r7, r5));
        emit(stream, output(r1));
}

void test_map(Seq_T stream)
{
        emit(stream, loadval(r3, 5));
        emit(stream, map(r2, r3));
        emit(stream, loadval(r5, 70));
        emit(stream, output(r5));
        emit(stream, halt());
}

void test_unmap(Seq_T stream)
{
        emit(stream, loadval(r3, 5));
        emit(stream, map(r2, r3));
        emit(stream, unmap(r2));
        emit(stream, loadval(r5, 70));
        emit(stream, output(r5));
        emit(stream, halt());
}

void test_seg_store(Seq_T stream)
{
        emit(stream, loadval(r3, 5));
        emit(stream, map(r2, r3));
        /* map a segment of 5 words and stores its identifier into r2 */
        emit(stream, loadval(r5, 105));
        emit(stream, loadval(r6, 2));
        emit(stream, seg_store(r2, r6, r5));
        emit(stream, loadval(r5, 70));
        emit(stream, output(r5));
        emit(stream, halt());
}

void test_seg_load(Seq_T stream)
{
        emit(stream, loadval(r3, 5));
        emit(stream, map(r2, r3));
        /* map a segment of 5 words and stores its identifier into r2 */
        emit(stream, loadval(r5, 101));
        emit(stream, loadval(r6, 1));
        emit(stream, seg_store(r2, r6, r5));
        emit(stream, seg_load(r7, r2, r6));
        emit(stream, output(r7));
        emit(stream, halt());
}

void test_cmov(Seq_T stream)
{
        emit(stream, loadval(r1, 0));
        emit(stream, loadval(r3, 1));
        emit(stream, loadval(r2, 78));
        emit(stream, loadval(r4, 79));
        emit(stream, c_move(r4, r2, r1));
        emit(stream, output(r4));
        emit(stream, c_move(r4, r2, r3));
        emit(stream, output(r4));
        emit(stream, halt());
}

void test_load_p(Seq_T stream)
{
        emit(stream, loadval(r1, 48));
        emit(stream, loadval(r2, 49));
        emit(stream, loadval(r3, 6)); /* where to jump to */
        emit(stream, loadval(r4, 0));
        emit(stream, load_p(r4, r3));
        emit(stream, output(r1));
        emit(stream, output(r2));
        emit(stream, halt());

}

void test_50_mil(Seq_T stream)
{
        int tests = 10000000;
        for (int i = 0; i < tests; i++){
                emit(stream, loadval(r1, 20));
                emit(stream, loadval(r2, 10));
                emit(stream, add(r1, r1, r2));
                emit(stream, add(r2, r2, r1));
                emit(stream, div(r2, r2, r1));
        }
        emit(stream, loadval(r5, 65));
        emit(stream, output(r5));
        emit(stream, halt());
}

void test_input(Seq_T stream)
{
        emit(stream, input(r1));
        emit(stream, output(r1));
        emit(stream, halt());
}

void fail_input(Seq_T stream)
{
        emit(stream, input(r1));
        emit(stream, output(r1));
        emit(stream, halt());
}
