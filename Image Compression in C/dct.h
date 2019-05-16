/*
 *      dct.h
 *      by Nick Camp and Darby Huye
 *      Assignment 4; 3/8/2018
 *
 *
 *
 *
 */

#ifndef DCT_H
#define DCT_H

#include <stdio.h>
#include <rgb_component_video.h>
#include "uarray.h"
#include "bitpack.h"

/* compresses a UArray of word_structs into a UArray of bitwords*/
UArray_T compressdct(UArray2_T structRay);

/* creates a bitword from given Y, PB, and PR values */
uint64_t makeWord(pixel_info input);

/* creates a UArray of pixel_info structs from a compressed file's words */
UArray2_T decompressdct(UArray_T wordRay, int width, int height);

/* creates a single pixel_info struct from a single given word */
pixel_info unpackWord(uint64_t word);

/* calculates the 'a' cosine coefficient from the 4 Y luma values */
unsigned calc_a(pixel_info pi);

/* calculates the 'b' cosine coefficient from the 4 Y luma values */
int calc_b(pixel_info pi);

/* calculates the 'c' cosine coefficient from the 4 Y luma values */
int calc_c(pixel_info pi);

/* calculates the 'd' cosine coefficient from the 4 Y luma values */
int calc_d(pixel_info pi);

/* creates the Y luma value for the top left pixel out of a, b, c, and d */
float calc_Y1(unsigned a, int b, int c, int d);

/* creates the Y luma value for the top right pixel out of a, b, c, and d */
float calc_Y2(unsigned a, int b, int c, int d);

/* creates the Y luma value for the bottom left pixel out of a, b, c, and d */
float calc_Y3(unsigned a, int b, int c, int d);

/* creates the Y luma value for the bottom right pixel out of a, b, c, and d*/
float calc_Y4(unsigned a, int b, int c, int d);

/* multiplies a by 511 and rounds to get the average brightness value */
unsigned quantize_a(float a);

/* quantizes b, c, or d to be within -15 and 15 */
int quantize_bcd(float bcd);

/* divides a by 511 and rounds to get the dequantized value of a */
float dequantize_a(unsigned a);

/* returns the dequantized value of b, c, or d by multiplying by 50 */
float dequantize_bcd(int bcd);

/* quantizes pb or pr using Arith40Arith40_index_of_chroma */
unsigned quantize_pbpr(float pbpr);

/* dequantizes pb or pr by using Arith40_chroma_of_index */
float dequantize_pbpr(unsigned pbpr);

#endif
