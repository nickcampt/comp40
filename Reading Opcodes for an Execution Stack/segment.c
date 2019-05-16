#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <seq.h>
#include <bitpack.h>
#include <string.h>
#include "mem.h"
#include "segment.h"

struct memory {
        Seq_T segmentID;
        Seq_T removed_segments;
        Seq_T segIndex;
        int num_words;
};

static inline uint32_t build_word(FILE *fp)
{
        uint32_t firstWord = getc(fp);
        uint32_t secondWord = getc(fp);
        uint32_t thirdWord = getc(fp);
        uint32_t forthWord = getc(fp);

        uint32_t word = (uint32_t)0;
        word = Bitpack_newu(word, 8, 24, firstWord);
        word = Bitpack_newu(word, 8, 16, secondWord);
        word = Bitpack_newu(word, 8, 8, thirdWord);
        word = Bitpack_newu(word, 8, 0, forthWord);

        return word;
}

Memory initialize_memory(FILE *fp)
{
        /* finding out how many words there are in a file */
        int fd = fileno(fp);
        struct stat fileBuffer;
        fstat(fd, &fileBuffer);
        int prog_size = fileBuffer.st_size;
        prog_size = prog_size/sizeof(uint32_t);

        uint32_t *prog_data = calloc(prog_size,sizeof(uint32_t));
        if (prog_data == NULL){
                Except_T BAD_ALLOC = {"\"Could not allocate memory\""};
                RAISE(BAD_ALLOC);
        }

        Memory mem;
        NEW(mem);
        mem->segmentID = Seq_new(1);
        mem->removed_segments = Seq_new(0);
        mem->segIndex = Seq_new(1);
        mem->num_words = prog_size;
        int total_words = mem->num_words;
        for (int i=0; i<total_words; ++i)
                prog_data[i] = build_word(fp);

        Seq_addhi(mem->segmentID, (void *)(uintptr_t)prog_data);
        Seq_addhi(mem->segIndex, (void *)(uintptr_t)prog_size);

        return mem;
}

inline uint32_t *get_prog_data(Memory mem)
{
        return Seq_get(mem->segmentID,0);
}

int prog_length(Memory mem)
{
        return mem->num_words;
}

int new_index(Memory mem)
{
        int index = 0;

        if(Seq_length(mem->removed_segments) != 0)
                index = (int)(uintptr_t)Seq_remhi(mem->removed_segments);

        else
                index = -1;
        return index;
}

void map_segment(Memory mem, uint32_t regCIndex, uint32_t regBIndex,
                                                        uint32_t registers[])
{
        int num_words = registers[regCIndex];
        int index = new_index(mem);
        uint32_t *prog_data = calloc(num_words, sizeof(uint32_t));

        if (prog_data == NULL){
                Except_T BAD_ALLOC = {"\"Could not allocate memory\""};
                RAISE(BAD_ALLOC);
        }
        if (index != -1)
        {
                Seq_put(mem->segmentID, index, (void *)(uintptr_t)prog_data);
                Seq_put(mem->segIndex, index, (void *)(uintptr_t)num_words);
                registers[regBIndex] = index;
        }

        else /* there is no previously removed segment */
        {
                Seq_addhi(mem->segmentID, (void *)(uintptr_t)prog_data);
                Seq_addhi(mem->segIndex, (void *)(uintptr_t)num_words);
                registers[regBIndex] = Seq_length(mem->segmentID) - 1;
        }
}


void unmap_segment(Memory mem, uint32_t c, uint32_t registers[])
{
        int num_words = 0;
        uint32_t *trash = Seq_get(mem->segmentID, registers[c]);
        free(trash);
        Seq_addhi(mem->removed_segments, (void *)(uintptr_t)registers[c]);
        Seq_put(mem->segIndex, (int)registers[c],
                                (void *)(uintptr_t)num_words);

}


void store_in_memory(Memory mem,uint32_t a, uint32_t b, uint32_t c,
                                                        uint32_t registers[])
{
        uint32_t *prog_data = Seq_get(mem->segmentID, registers[a]);
        prog_data[registers[b]] = registers[c];
}

void load_from_memory( Memory mem, uint32_t a, uint32_t b, uint32_t c,
                                                        uint32_t registers[])
{
        uint32_t *prog_data = Seq_get(mem->segmentID, registers[b]);
        registers[a] = prog_data[registers[c]];
}

void load_program(Memory mem, unsigned *prog_counter, uint32_t b, uint32_t c,
                                                        uint32_t registers[])
{
        *prog_counter = registers[c];
        size_t size_of_word = sizeof(uint32_t);

        if (registers[b] != 0)
        {
              uint32_t *prog_data = Seq_get(mem->segmentID, registers[b]);
             uint32_t *trash = Seq_get(mem->segmentID, 0);
             free(trash);
             int prog_size = (int)(uintptr_t)Seq_get(mem->segIndex,
                      registers[b]);
             trash = calloc(prog_size,size_of_word);
             memcpy(trash, prog_data, prog_size * size_of_word);
             Seq_put(mem->segmentID, 0, (void *)(uintptr_t)trash);
             Seq_put(mem->segIndex, 0, (void *)(uintptr_t)prog_size);
     } /* loading program that has already been loaded */
}

/*For debugging*/
void print_memory_info(Memory mem)
{
        printf("Number of segments: %d\n", Seq_length(mem->segmentID));
        printf("Number of removed_segments: %d\n",
                        Seq_length(mem->removed_segments));
}


/* frees all allocated memory.
 * first checks to see if the segment isn't already unmapped before freeing it
 */
inline void free_entire_memory(Memory mem)
{
        int length = Seq_length(mem->segmentID);
        for(int i=0; i<length; ++i)
        {
                if ((int)(uintptr_t)Seq_get(mem->segIndex, i) != 0)
                {
                        uint32_t *segTrash = Seq_get(mem->segmentID, i);
                        free(segTrash);
                }
        }
        Seq_free(&(mem->segIndex));
        Seq_free(&(mem->segmentID));
        Seq_free(&(mem->removed_segments));
        free(mem);
}
