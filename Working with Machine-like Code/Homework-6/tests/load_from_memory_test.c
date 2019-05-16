#include "segment.h"
#include <stdio.h>
#include <stdlib.h>
#include <bool.h>
#include <stdint.h>

uint32_t registers[8] = {3, 4, 5, 6, 7, 8, 9, 10};

const int regA = 0;
const int regB = 1;
const int regC = 2;

bool test_load_from_memory();

int main()
{
        printf("load from memory test\n");
        printf("this sets the value of register a to the word \n");
        printf("at index register C, which is in the dynamic array\n");
        printf("pointed to by what is at the segmented memory index given\n");
        printf("by register b... wow! that is word-y\n");

        if (test_load_from_memory()) {
                printf("Load from memory test passed!\n");
        }
        else printf("BAD!! load from memory did not perform as expected\n")

}

bool test_load_from_memory()
{
        uint32_t original = registers[regA];
        load_from_memory(regA, regB, regC, registers);
        printf("register A\nWAS: %d\nNOW: %d\n", );
        if (regA == original) return false;
        return true;
}
