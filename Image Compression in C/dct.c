/*
 *      dct.c
 *      by Nick Camp and Darby Huye
 *      Assignment 4; 3/8/2018
 *
 *      For image compression:
 *      Uses a Discrete Cosine Transformation to transform for luminance/luma
 *      values of pixels into cosine coefficients. Stores the coefficients,
 *      along with the average chroma elements of the pixels, in words after
 *      quantization.
 *      For image decompression:
 *      Unpacks the values from words and applies an inverse discrete cosine
 *      transformation to compute Y values for the image. Results in a
 *      2D array of structs containing pixel information (that can be further)
 *      decompressed with functions in rgb_component_video.c
 *
 */

#include <dct.h>
#include "assert.h"
#include <stdlib.h>
#include <math.h>
#include "arith40.h"

const double zero_interval_width = 0.0096774;
const int size_of_word = sizeof(uint64_t);
const unsigned a_width = 6;
const unsigned a_lsb = 26;
const unsigned b_width = 6;
const unsigned b_lsb = 20;
const unsigned c_width = 6;
const unsigned c_lsb = 14;
const unsigned d_width = 6;
const unsigned d_lsb = 8;
const unsigned pb_width = 4;
const unsigned pb_lsb = 4;
const unsigned pr_width = 4;
const unsigned pr_lsb = 0;
const int bcd_MAX = 15;
const int bcd_MIN = -15;
const double abcd_DIVISOR = 4.0;
const int a_MULT = 63;
const double bcd_LOW = -0.3;
const double bcd_HIGH = 0.3;
const int bcd_INTERVALS = 50;
const int GARBAGE = -10000;

/* compresses a UArray of word_structs into a UArray of bitwords*/
UArray_T compressdct(UArray2_T structRay)
{
    int row, col, index = 0;
    int width = UArray2_width(structRay);
    int height = UArray2_height(structRay);
    UArray_T compressedRay = UArray_new(width*height, size_of_word);

    uint64_t targetWord;
    uint64_t * targetLocation;

    for(row = 0; row < height; row++){

        for(col = 0; col < width; col++){
            pixel_info * currentStruct = UArray2_at(structRay, col, row);
            targetWord = makeWord(*currentStruct);
            targetLocation = UArray_at(compressedRay, index);
            *targetLocation = targetWord;
            index++;
            free(*currentStruct);
        }
    }

    return compressedRay;
}

/* creates a bitword from given Y, PB, and PR values */
uint64_t makeWord(pixel_info input)
{
        int a, b, c, d, pb, pr;

        a = calc_a(input);
        b = calc_b(input);
        c = calc_c(input);
        d = calc_d(input);
        pb = quantize_pbpr(input -> pb);
        pr = quantize_pbpr(input -> pr);
        uint64_t word = 0;
        word = Bitpack_newu(word, a_width, a_lsb, a);
        word = Bitpack_news(word, b_width, b_lsb, b);
        word = Bitpack_news(word, c_width, c_lsb, c);
        word = Bitpack_news(word, d_width, d_lsb, d);
        word = Bitpack_newu(word, pb_width, pb_lsb, pb);
        word = Bitpack_newu(word, pr_width, pr_lsb, pr);
        return word;

}

/* calculates the 'a' cosine coefficient from the 4 Y luma values */
unsigned calc_a(pixel_info pi)
{
    float a = ((pi->y1 + pi->y2 + pi->y3 + pi -> y4)/abcd_DIVISOR);
    return quantize_a(a);
}

/* calculates the 'b' cosine coefficient from the 4 Y luma values */
int calc_b(pixel_info pi)
{
    float b = ((pi->y3 + pi -> y4 - pi->y1 - pi->y2)/abcd_DIVISOR);
    return quantize_bcd(b);
}

/* calculates the 'c' cosine coefficient from the 4 Y luma values */
int calc_c(pixel_info pi)
{
    float c = ((pi -> y4 - pi->y1 + pi->y2 - pi->y3)/abcd_DIVISOR);
    return quantize_bcd(c);
}

/* calculates the 'd' cosine coefficient from the 4 Y luma values */
int calc_d(pixel_info pi)
{
    float d = ((pi -> y4 + pi->y1 - pi->y2 - pi->y3)/abcd_DIVISOR);
    return quantize_bcd(d);
}

/* multiplies a by 511 and rounds to get the average brightness value */
unsigned quantize_a(float a)
{
    return (floor(a * a_MULT));
}

/* quantizes b, c, or d to be within -15 and 15 */
int quantize_bcd(float num)
{
    if(num < bcd_LOW) return bcd_MIN;
    if(num > bcd_HIGH) return bcd_MAX;
    if(num < zero_interval_width && num > -zero_interval_width) return 0;
    if(num > 0) return ceil(num * (bcd_INTERVALS));
    if(num < 0) return floor(num * bcd_INTERVALS);

    /* a garbage value, this should never actually be returned */
    return GARBAGE;

}

/* quantizes pb or pr using Arith40Arith40_index_of_chroma */
unsigned quantize_pbpr(float num)
{
    return Arith40_index_of_chroma(num);
}



/* FUNCTIONS FOR DCT DECOMPRESSION */

/* creates a UArray of pixel_info structs from a compressed file's words */
UArray2_T decompressdct(UArray_T wordRay, int width, int height)
{
    int rayIndex;
    pixel_info targetStruct;
    int row, col;
    UArray2_T structRay = UArray2_new(width, height, sizeof(pixel_info));
    for (rayIndex = 0; rayIndex < UArray_length(wordRay); rayIndex++) {
        uint64_t * targetWord = UArray_at(wordRay, rayIndex);
        targetStruct = unpackWord(*targetWord);
        col = rayIndex % width;
        row = rayIndex / width;
        pixel_info * targetLocation = UArray2_at(structRay, col, row);
        *targetLocation = targetStruct;
    }
    return structRay;
}

/* creates a single pixel_info struct from a single given word */
pixel_info unpackWord(uint64_t word)
{
    pixel_info wordStruct;
    NEW(wordStruct);
    uint64_t a, pb, pr;
    int64_t b, c, d;
    a = Bitpack_getu(word, a_width, a_lsb);
    b = Bitpack_gets(word, b_width, b_lsb);
    c = Bitpack_gets(word, c_width, c_lsb);
    d = Bitpack_gets(word, d_width, d_lsb);
    pb = Bitpack_getu(word, pb_width, pb_lsb);
    pr = Bitpack_getu(word, pr_width, pr_lsb);
    wordStruct -> y1 = calc_Y1(a, b, c, d);
    wordStruct -> y2 = calc_Y2(a, b, c, d);
    wordStruct -> y3 = calc_Y3(a, b, c, d);
    wordStruct -> y4 = calc_Y4(a, b, c, d);
    wordStruct -> pb = dequantize_pbpr(pb);
    wordStruct -> pr = dequantize_pbpr(pr);

    return wordStruct;
}

/* creates the Y luma value for the top left pixel out of a, b, c, and d */
float calc_Y1(unsigned a, int b, int c, int d)
{
    float af, bf, cf, df; /* f for float~~~~ */
    af = dequantize_a(a);
    bf = dequantize_bcd(b);
    cf = dequantize_bcd(c);
    df = dequantize_bcd(d);
    return (af - bf - cf + df);
}

/* creates the Y luma value for the top right pixel out of a, b, c, and d */
float calc_Y2(unsigned a, int b, int c, int d)
{
    float af, bf, cf, df;
    af = dequantize_a(a);
    bf = dequantize_bcd(b);
    cf = dequantize_bcd(c);
    df = dequantize_bcd(d);
    return (af - bf + cf - df);
}

/* creates the Y luma value for the bottom left pixel out of a, b, c, and d */
float calc_Y3(unsigned a, int b, int c, int d)
{
    float af, bf, cf, df;
    af = dequantize_a(a);
    bf = dequantize_bcd(b);
    cf = dequantize_bcd(c);
    df = dequantize_bcd(d);
    return (af + bf - cf - df);
}

/* creates the Y luma value for the bottom right pixel out of a, b, c, and d*/
float calc_Y4(unsigned a, int b, int c, int d)
{
    float af, bf, cf, df;
    af = dequantize_a(a);
    bf = dequantize_bcd(b);
    cf = dequantize_bcd(c);
    df = dequantize_bcd(d);
    if((af + bf + cf + df) > 1) return 1;
    return (af + bf + cf + df);
}

/* divides a by 511 and rounds to get the dequantized value of a */
float dequantize_a(unsigned a)
{
    return (((float)a)/a_MULT);
}

/* returns the dequantized value of b, c, or d by multiplying by 50 */
float dequantize_bcd(int num)
{
    return ((float)num)/bcd_INTERVALS;
}

/* dequantizes pb or pr by using Arith40_chroma_of_index */
float dequantize_pbpr(unsigned pbpr)
{
    return Arith40_chroma_of_index(pbpr);
}
