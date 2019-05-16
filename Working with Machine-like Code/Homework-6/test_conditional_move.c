#include "segment.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

uint32_t registers[8] = {3, 4, 5, 6, 7, 8, 9, 10};

bool test_conditional_move_fail();
bool test_conditional_move_pass();

const int regA = 0; /* this is the register that gets modified conditionaly */
const int regB = 1;
const int regC = 2; /* this contains the condition */
const int fail = 1;
const int pass = 0;

int main()
{

        printf("testing the conditonal move instruction\n");
        printf("If register C is 0, then it sets register a\n");
        printf("to the value at register b\n");
        printf("2 tests here, a move that should not happen (1)\n");
        printf("and a move that should happen (2)\n");

        if (test_conditional_move_fail()) {
                printf("1: fail test passed, nothing was moved");
        }
        else fprintf(stderr, "1: fail test did not pass, this is bad\n");

        if (test_conditional_move_pass()) {
                printf("2: valid test passed, move happened");
        }
        else fprintf(stderr, "2: valid test did not pass, this is bad\n");

}

bool test_conditional_move_fail()
{
        registers[regC] = fail;
        uint32_t original = registers[regA];
        conditional_move(regA, regB, regC, registers);
        printf("register A\nWAS: %d\nNOW:%d\n", (int)original,
                                        (int)registers[regA]);
        if (registers[regA] != original) return false;
        return true;
}

bool test_conditional_move_pass()
{
        registers[regC] = pass;
        uint32_t original = registers[regA];
        conditional_move(regA, regB, regC, registers);
        printf("register A\nWAS: %d\nNOW:%d\n", (int)original,
                                        (int)registers[regA]);
        if (registers[regA] == original) return false;
        return true;
}
