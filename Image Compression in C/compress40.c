/*
 *      compress40.c
 *      by Nick Camp and Darby Huye
 *      Assignment 4; 3/8/2018
 *
 *      The file contains our compress40 and decompress40 functions called
 *      in 40image.c. The file also contains functions for printing a
 *      compressed image and reading a compressed image from a file.
 *
 *
 */

#include <stdio.h>
#include "compress40.h"
#include "uarray.h"
#include "uarray2.h"
#include "rgb_component_video.h"
#include "dct.h"
#include "a2plain.h"
#include "a2methods.h"
#include "assert.h"
#include "stdlib.h"


#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                exit(1);                                        \
        }                                                       \
} while (0)

/* Prints a compressed image; takes a Uarray of words */
void print_compressed(UArray_T compressed, int width, int height);

/* takes a compressed images as a file pointer and makes a UArray of words */
UArray_T process_compressed(FILE * input, unsigned width, unsigned height);

/* compresses a ppm image and prints the compressed image to standard output*/
void compress40  (FILE *input)
{
        A2Methods_T methods = uarray2_methods_plain;
        A2Methods_mapfun *map = methods->map_default;
        SET_METHODS(uarray2_methods_plain, map_row_major, "row-major");

        Pnm_ppm image = Pnm_ppmread(input, methods);

        UArray2_T partiallycompressed = compress_to_cv(image);
        int width = UArray2_width(partiallycompressed);
        int height = UArray2_height(partiallycompressed);
        UArray_T compressed = compressdct(partiallycompressed);
        Pnm_ppmfree(&image);
        print_compressed(compressed, width, height);
        UArray_free(&compressed);
        UArray2_free(&partiallycompressed);

}

/* decompresses an image and prints it (ppm format) to standard output */
void decompress40   (FILE *input)
{

        A2Methods_T methods = uarray2_methods_plain;
        A2Methods_mapfun *map = methods->map_default;
        SET_METHODS(uarray2_methods_plain, map_row_major, "row-major");

        unsigned width, height;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u",
                                                            &width, &height);
        assert(read == 2);
        UArray_T words = process_compressed(input, width, height);

        UArray2_T structs = decompressdct(words, width, height);
        Pnm_ppm decompressed = decompress_structs(structs, methods);
        Pnm_ppmwrite(stdout, decompressed);
        Pnm_ppmfree(&decompressed);
        UArray_free(&words);
        UArray2_free(&structs);



}

void print_compressed(UArray_T compressed, int width, int height)
{
    int word;
    printf("COMP40 Compressed image format 2\n%u %u", width, height);
    printf("\n");
    for(word = 0; word < UArray_length(compressed); word++) {
        uint64_t * thisword = UArray_at(compressed, word);
        putchar((int)Bitpack_getu(*thisword, 8, 24));
        putchar((int)Bitpack_getu(*thisword, 8, 16));
        putchar((int)Bitpack_getu(*thisword, 8, 8));
        putchar((int)Bitpack_getu(*thisword, 8, 0));
   }
}

/* Prints a compressed image; takes a Uarray of words */
UArray_T process_compressed(FILE * input, unsigned width, unsigned height)
{
    int word;
    unsigned char c = getc(input);
    assert(c == '\n');
    uint64_t thisword = 0;

    UArray_T words = UArray_new(width * height, sizeof(uint64_t));
    for(word = 0; word < UArray_length(words); word++) {
        c = getc(input);
        thisword = Bitpack_newu(thisword, 8, 24, (uint64_t)c);
        c = getc(input);
        thisword = Bitpack_newu(thisword, 8, 16, (uint64_t)c);
        c = getc(input);
        thisword = Bitpack_newu(thisword, 8, 8, (uint64_t)c);
        c = getc(input);
        thisword = Bitpack_newu(thisword, 8, 0, (uint64_t)c);
        uint64_t * putwordhere = UArray_at(words, word);
        *putwordhere = thisword;
        thisword = 0;
    }

    return words;

}
